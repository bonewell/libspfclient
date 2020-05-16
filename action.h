#ifndef ACTION_H_
#define ACTION_H_

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <functional>
#include <list>
#include <type_traits>

#include "error.h"
#include "microservice.h"
#include "types.h"

namespace ptree = boost::property_tree;
namespace json = boost::property_tree::json_parser;

namespace spf {
class ActionBase {
public:
  virtual ~ActionBase() = default;

  ptree::ptree invoke(Microservice& service) {
    auto res = response(service.invoke(request()));
    check(res);
    return res;
  }

  void async_invoke(Microservice& service,
      std::function<void(ptree::ptree)> response_handler,
      std::function<void(Error)> error_handler) try {
    service.async_invoke(request(),
        [this, response_handler, error_handler](auto output, auto error) {
      if (error) return error_handler(Error{error.message()});
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
  ptree::ptree request_;

private:
  std::string request() {
    std::ostringstream oss;
    json::write_json_helper(oss, request_, 0, false);
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
      throw Error{*error};
    }
  }
};

template<typename T>
class Action : public ActionBase {
public:
  using Output = T;

  virtual ~Action() = default;
  virtual Output get(ptree::ptree response) = 0;

  Output execute(Microservice& service) {
    return get(invoke(service));
  }

  void execute(Microservice& service,
      std::function<void(Output, Error)> callback) {
    async_invoke(service,
        [this, callback](auto res) { callback(get(std::move(res)), {}); },
        [callback](auto error) { callback({}, error); });
  }
};

template<>
class Action<void> : public ActionBase {
public:
  void execute(Microservice& service) {
    invoke(service);
  }

  void execute(Microservice& service,
      std::function<void(Error)> callback) {
    async_invoke(service,
        [callback](auto) { callback({}); },
        [callback](auto error) { callback(error); });
  }
};

class AddVertex : public Action<Id> {
public:
  AddVertex() {
    request_.put("action", "AddVertex");
  }
  Id get(ptree::ptree response) override {
    return response.get<Id>("id");
  }
};

class RemoveVertex : public Action<void> {
public:
  explicit RemoveVertex(Id id) {
    request_.put("action", "RemoveVertex");
    request_.put("id", id);
  }
};

class AddEdge : public Action<void> {
public:
  AddEdge(Id from, Id to, Weight weight) {
    request_.put("action", "AddEdge");
    request_.put("from", from);
    request_.put("to", to);
    request_.put("weight", weight);
  }
};

class RemoveEdge : public Action<void> {
public:
  RemoveEdge(Id from, Id to) {
    request_.put("action", "RemoveEdge");
    request_.put("from", from);
    request_.put("to", to);
  }
};

class GetPath : public Action<std::list<Id>> {
public:
  GetPath(Id from, Id to) {
    request_.put("action", "GetPath");
    request_.put("from", from);
    request_.put("to", to);
  }
  std::list<Id> get(ptree::ptree response) override {
    std::list<Id> path;
    for (auto const& i : response.get_child("ids")) {
      path.push_back(i.second.get_value<Id>());
    }
    return path;
  }
};
}  // namespace spf

#endif /* ACTION_H_ */
