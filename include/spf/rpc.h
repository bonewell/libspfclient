#ifndef RPC_H_
#define RPC_H_

#include <functional>
#include <string>

#include "error.h"

namespace spf {
class Rpc {
public:
  using Handler = std::function<void(std::string, Error)>;

  virtual ~Rpc() = default;

  /**
   * Invokes microservice synchronously.
   * @param request - request to serve.
   * @return response.
   */
  virtual std::string invoke(std::string const& request) = 0;

  /**
   * Invokes microservice anynchronously.
   * @param request - request to serve.
   * @param handler - callback to get response.
   */
  virtual void async_invoke(std::string const& request, Handler handler) = 0;
};
}  // namespace spf

#endif /* RPC_H_ */
