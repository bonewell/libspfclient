#include "web_socket_microservice.h"

#include <boost/beast.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/beast/core/buffers_to_string.hpp>

#include <iostream>

namespace net = boost::asio;
using tcp = net::ip::tcp;
namespace beast = boost::beast;
namespace ws = beast::websocket;

namespace {
void fail(beast::error_code ec, char const* what) {
  std::cerr << what << ": " << ec.message() << '\n';
}
}  // namespace

WebSocketMicroservice::WebSocketMicroservice(const std::string &host,
    int port) {
  tcp::resolver resolver{ioc_};
  auto const points = resolver.resolve(host, std::to_string(port));
  beast::get_lowest_layer(wsock_).expires_after(std::chrono::seconds(30));
  net::connect(beast::get_lowest_layer(wsock_).socket(), points);
  beast::get_lowest_layer(wsock_).expires_never();
  wsock_.set_option(ws::stream_base::timeout::suggested(beast::role_type::client));
  wsock_.handshake(host, "/");
}

WebSocketMicroservice::~WebSocketMicroservice() {
}

std::string WebSocketMicroservice::invoke(std::string const& request) {
  wsock_.write(net::buffer(request));
  beast::flat_buffer ibuf;
  wsock_.read(ibuf);
  return beast::buffers_to_string(ibuf.data());
}

void WebSocketMicroservice::async_invoke(std::string const& request,
    std::function<void(std::string)> handler) {
//  net::spawn(ioc_, [this, request, handler](auto yield) {
//    beast::error_code ec;
//    auto obuf = net::buffer(request);
//    wsock_.async_write(obuf, yield[ec]);
//    if (ec) return fail(ec, "write");
//    beast::flat_buffer ibuf;
//    wsock_.async_read(ibuf, yield[ec]);
//    if (ec == ws::error::closed) return;
//    if (ec) return fail(ec, "read");
//    auto response = beast::buffers_to_string(ibuf.data());
//    handler(response);
//  });
}
