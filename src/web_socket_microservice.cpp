#include "spf/web_socket_microservice.h"

#include <boost/beast.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/beast/core/buffers_to_string.hpp>

namespace net = boost::asio;
using tcp = net::ip::tcp;
namespace beast = boost::beast;
namespace ws = beast::websocket;

namespace spf {
WebSocketMicroservice::WebSocketMicroservice(std::string const& host, unsigned short port) {
  tcp::resolver resolver{ioc_};
  auto const points = resolver.resolve(host, std::to_string(port));
  beast::get_lowest_layer(wsock_).expires_after(std::chrono::seconds(30));
  net::connect(beast::get_lowest_layer(wsock_).socket(), points);
  beast::get_lowest_layer(wsock_).expires_never();
  wsock_.set_option(ws::stream_base::timeout::suggested(beast::role_type::client));
  wsock_.handshake(host, "/");
  thread_ = std::thread([this] { ioc_.run(); });
}

WebSocketMicroservice::~WebSocketMicroservice() {
  work_.reset();
  thread_.join();
  wsock_.close({ws::close_code::normal});
}

std::string WebSocketMicroservice::invoke(std::string const& request) {
  wsock_.write(net::buffer(request));
  beast::flat_buffer ibuf;
  wsock_.read(ibuf);
  return beast::buffers_to_string(ibuf.data());
}

void WebSocketMicroservice::async_invoke(std::string const& request, Handler handler) {
  queue_.emplace(request, handler);
  if (queue_.size() == 1) async_invoke_internal();
}

void WebSocketMicroservice::async_invoke_internal() {
  net::spawn(strand_, [this](net::yield_context yield) {
    auto const& [request, handler] = queue_.front();
    beast::error_code ec;
    wsock_.async_write(net::buffer(request), yield[ec]);
    if (ec) return handler("", ec);
    beast::flat_buffer ibuf;
    wsock_.async_read(ibuf, yield[ec]);
    if (ec == ws::error::closed) return;
    if (ec) return handler("", ec);
    auto response = beast::buffers_to_string(ibuf.data());
    handler(response, {});
    queue_.pop();
    if (!queue_.empty()) async_invoke_internal();
  });
}

}  // namespace spf
