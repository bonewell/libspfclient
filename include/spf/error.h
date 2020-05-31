#ifndef ERROR_H_
#define ERROR_H_

#include <stdexcept>
#include <string>

namespace spf {
class Error : public std::runtime_error {
public:
  using std::runtime_error::runtime_error;
  Error() : std::runtime_error{""} {}
  operator bool() { return !std::string{what()}.empty(); };
};
}  // namespace spf

#endif /* ERROR_H_ */
