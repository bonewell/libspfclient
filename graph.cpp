#include "graph.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <iostream>
#include <sstream>

namespace ptree = boost::property_tree;
namespace json = boost::property_tree::json_parser;

std::string send_data(std::string request) {
  std::cout << "request: " << request << '\n';
  return "{}";
}

ptree::ptree send(ptree::ptree request) {
  std::ostringstream oss;
  json::write_json_helper(oss, request, 0, false);

  auto output = send_data(oss.str());

  std::istringstream iss{output};
  ptree::ptree response;
  json::read_json(iss, response);
  return response;
}

Id Graph::addVertex() {
  ptree::ptree request;
  request.put("action", "AddVertex");
  auto response = send(request);
  return response.get<Id>("id");
}

bool Graph::setEdge(Id from, Id to, Weight weight) {
  ptree::ptree request;
  request.put("action", "SetEdge");
  request.put("from", from).put("to", to).put("weight", weight);
  auto response = send(request);
  return true;
}

std::list<Id> Graph::path(Id from, Id to) {
  ptree::ptree request;
  request.put("action", "GetPath");
  request.put("from", from).put("to", to);
  auto response = send(request);
  std::list<Id> path;
  for (auto const& i : response.get_child("ids")) {
    path.push_back(i.second.get_value<Id>());
  }
  return path;
}
