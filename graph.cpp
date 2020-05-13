#include "graph.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <iostream>
#include <sstream>

#include "microservice.h"

namespace ptree = boost::property_tree;
namespace json = boost::property_tree::json_parser;

Graph::Graph(Microservice& service) : service_{service} {}

ptree::ptree Graph::invoke(ptree::ptree const& request) {
  std::ostringstream oss;
  json::write_json_helper(oss, request, 0, false);

  auto output = service_.invoke(oss.str());

  std::istringstream iss{output};
  ptree::ptree response;
  json::read_json(iss, response);

  auto error = response.get_optional<std::string>("error");
  if (error) {
    throw Error{*error};
  }
  return response;
}

Id Graph::addVertex() {
  ptree::ptree request;
  request.put("action", "AddVertex");
  auto response = invoke(request);
  return response.get<Id>("id");
}

void Graph::removeVertex(Id id) {
  ptree::ptree request;
  request.put("action", "RemoveVertex");
  request.put("id", id);
  invoke(request);
}

void Graph::setEdge(Id from, Id to, Weight weight) {
  ptree::ptree request;
  request.put("action", "AddEdge");
  request.put("from", from);
  request.put("to", to);
  request.put("weight", weight);
  invoke(request);
}

void Graph::removeEdge(Id from, Id to) {
  ptree::ptree request;
  request.put("action", "RemoveEdge");
  request.put("from", from);
  request.put("to", to);
  invoke(request);
}

std::list<Id> Graph::path(Id from, Id to) {
  ptree::ptree request;
  request.put("action", "GetPath");
  request.put("from", from);
  request.put("to", to);
  auto response = invoke(request);
  std::list<Id> path;
  for (auto const& i : response.get_child("ids")) {
    path.push_back(i.second.get_value<Id>());
  }
  return path;
}
