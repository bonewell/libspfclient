#ifndef WEB_SOCKET_RPC_H_
#define WEB_SOCKET_RPC_H_

#include <memory>

#include "rpc.h"

namespace spf {
class WebSocketRpc : public Rpc {
public:
  WebSocketRpc(std::string const& host, unsigned short port);
  ~WebSocketRpc();
  std::string invoke(std::string const& request) override;
  void async_invoke(std::string const& request, Handler handler) override;

private:
  class Impl;
  std::unique_ptr<Impl> impl_;
};
}  // namespace spf

#endif /* WEB_SOCKET_RPC_H_ */
