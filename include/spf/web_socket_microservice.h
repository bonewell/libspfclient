#ifndef WEB_SOCKET_MICROSERVICE_H_
#define WEB_SOCKET_MICROSERVICE_H_

#include <boost/asio.hpp>
#include <boost/beast.hpp>

#include <queue>
#include <string>
#include <thread>

#include "microservice.h"

namespace net = boost::asio;
namespace beast = boost::beast;
namespace ws = beast::websocket;

namespace spf {
class WebSocketMicroservice: public Microservice {
public:
  using Handler = std::function<void(std::string, Error)>;
  WebSocketMicroservice(std::string const& host, unsigned short port);
  ~WebSocketMicroservice();
  std::string invoke(std::string const& request) override;
  void async_invoke(std::string const& request, Handler handler) override;

private:
  void async_invoke_internal();
  using Action = std::pair<std::string, Handler>;
  std::queue<Action> queue_;
  net::io_context ioc_;
  net::executor_work_guard<net::io_context::executor_type> work_{ioc_.get_executor()};
  net::strand<net::io_context::executor_type> strand_{ioc_.get_executor()};
  ws::stream<beast::tcp_stream> wsock_{strand_};
  std::thread thread_;
};
}  // namespace spf

#endif /* WEB_SOCKET_MICROSERVICE_H_ */
