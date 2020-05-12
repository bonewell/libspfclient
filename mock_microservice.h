#ifndef MOCK_MICROSERVICE_H_
#define MOCK_MICROSERVICE_H_

#include <gmock/gmock.h>

#include "microservice.h"

class MockMicroservice : public Microservice {
public:
  MOCK_METHOD(std::string, invoke,
      (std::string const& request), (override));
  MOCK_METHOD(void, async_invoke,
      (std::string const& request, std::function<void(std::string)> handler), (override));
};

#endif /* MOCK_MICROSERVICE_H_ */
