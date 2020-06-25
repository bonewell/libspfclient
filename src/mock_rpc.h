#ifndef MOCK_RPC_H_
#define MOCK_RPC_H_

#include <gmock/gmock.h>

#include "spf/error.h"
#include "spf/rpc.h"

namespace spf {
class MockRpc : public Rpc {
public:
  MOCK_METHOD(std::string, invoke,
      (std::string const& request), (override));
  MOCK_METHOD(void, async_invoke,
      (std::string const& request,
          std::function<void(std::string, Error)> handler), (override));
};
}  // namespace spf

#endif /* MOCK_RPC_H_ */
