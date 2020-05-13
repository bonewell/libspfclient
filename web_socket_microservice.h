#ifndef WEB_SOCKET_MICROSERVICE_H_
#define WEB_SOCKET_MICROSERVICE_H_

#include <boost/beast.hpp>

#include "microservice.h"

namespace net = boost::asio;
namespace beast = boost::beast;
namespace ws = beast::websocket;

class WebSocketMicroservice: public Microservice {
public:
  WebSocketMicroservice(std::string const& host, int port);
  ~WebSocketMicroservice();
  std::string invoke(std::string const& request) override;
  void async_invoke(std::string const& request,
                    std::function<void(std::string)> handler) override;
  void run() { ioc_.run(); }
private:
  net::io_context ioc_;
  ws::stream<beast::tcp_stream> wsock_{ioc_};
};

#endif /* WEB_SOCKET_MICROSERVICE_H_ */
