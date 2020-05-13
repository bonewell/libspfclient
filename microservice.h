#ifndef MICROSERVICE_H_
#define MICROSERVICE_H_

#include <functional>
#include <string>

class Microservice {
public:
  virtual ~Microservice() = default;

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
  virtual void async_invoke(std::string const& request,
                            std::function<void(std::string)> handler) = 0;
};

#endif /* MICROSERVICE_H_ */
