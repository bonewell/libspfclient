#ifndef ACTION_H_
#define ACTION_H_

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <functional>
#include <list>
#include <type_traits>

#include "spf/error.h"
#include "spf/rpc.h"
#include "spf/types.h"

namespace ptree = boost::property_tree;
namespace json = boost::property_tree::json_parser;

namespace {
std::string request(ptree::ptree input) {
  std::ostringstream oss;
  json::write_json_helper(oss, input, 0, false);
  return oss.str();
}

ptree::ptree response(std::string output) {
  std::istringstream iss{output};
  ptree::ptree response;
  json::read_json(iss, response);
  return response;
}

void check(ptree::ptree const& response) {
  auto error = response.get_optional<std::string>("error");
  if (error) {
    throw spf::Error{*error};
  }
}
}  // namespace

namespace spf {
template <typename T>
class Response {
public:
  using Output = T;
  using Callback = std::function<void(Output, Error)>;
  Response(Callback callback) : callback_{callback} {}
  virtual ~Response() = default;
  void operator()(ptree::ptree output, Error error) const {
    try {
      callback_(get(std::move(output)), std::move(error));
    } catch (std::exception const& error) {
      callback_({}, Error{error.what()});
    } catch (...) {
      callback_({}, Error{"Unknown error"});
    }
  }
private:
  virtual Output get(ptree::ptree output) const = 0;
  Callback callback_;
};

class ActionBase {
public:
  virtual ~ActionBase() = default;

  ptree::ptree invoke(Rpc& service) {
    auto res = response(service.invoke(request(input_)));
    check(res);
    return res;
  }

  void async_invoke(Rpc& service,
      std::function<void(ptree::ptree)> response_handler,
      std::function<void(Error)> error_handler) try {
    service.async_invoke(request(input_),
        [response_handler, error_handler](auto output, auto error) {
      if (error) return error_handler(error);
      try {
        auto res = response(output);
        check(res);
        response_handler(std::move(res));
      } catch(std::exception const& error) {
        error_handler(Error{error.what()});
      } catch (...) {
        error_handler(Error{"Wrong response"});
      }
    });
  } catch (std::exception const& error) {
    error_handler(Error{error.what()});
  } catch (...) {
    error_handler(Error{"Unknown error"});
  }

protected:
  ptree::ptree input_;
};

template<typename T>
class Action : public ActionBase {
public:
  using Output = T;

  virtual ~Action() = default;
  virtual Output get(ptree::ptree output) const = 0;

  Output execute(Rpc& service) {
    return get(invoke(service));
  }

  template<typename C>
  void execute(Rpc& service,
      C callback) {
    async_invoke(service,
        [callback](auto res) { callback(std::move(res), {}); },
        [callback](auto error) { callback({}, error); });
  }
};

template<>
class Action<void> : public ActionBase {
public:
  void execute(Rpc& service) {
    invoke(service);
  }

  void execute(Rpc& service,
      std::function<void(Error)> callback) {
    async_invoke(service,
        [callback](auto) { callback({}); },
        [callback](auto error) { callback(error); });
  }
};

class AddVertex : public Action<Id> {
public:
  AddVertex() {
    input_.put("action", "AddVertex");
  }
  struct Id : public Response<Output> {
    using Response<Output>::Response;
    static Output value(ptree::ptree output) {
      return output.get<Output>("id");
    };
    Output get(ptree::ptree output) const override {
      return Id::value(std::move(output));
    }
  };
private:
  Output get(ptree::ptree output) const override {
    return Id::value(std::move(output));
  };
};

class RemoveVertex : public Action<void> {
public:
  explicit RemoveVertex(Id id) {
    input_.put("action", "RemoveVertex");
    input_.put("id", id);
  }
};

class AddEdge : public Action<void> {
public:
  AddEdge(Id from, Id to, Weight weight) {
    input_.put("action", "AddEdge");
    input_.put("from", from);
    input_.put("to", to);
    input_.put("weight", weight);
  }
};

class RemoveEdge : public Action<void> {
public:
  RemoveEdge(Id from, Id to) {
    input_.put("action", "RemoveEdge");
    input_.put("from", from);
    input_.put("to", to);
  }
};

class GetPath : public Action<std::list<Id>> {
public:
  GetPath(Id from, Id to) {
    input_.put("action", "GetPath");
    input_.put("from", from);
    input_.put("to", to);
  }
  struct Path : public Response<Output> {
    using Response<Output>::Response;
    static Output value(ptree::ptree output) {
      Output path;
      for (auto const& i : output.get_child("ids")) {
        path.push_back(i.second.get_value<Id>());
      }
      return path;
    };
    Output get(ptree::ptree output) const override {
      return Path::value(output);
    }
  };
private:
  Output get(ptree::ptree output) const override {
    return Path::value(output);
  }
};
}  // namespace spf

#endif /* ACTION_H_ */
