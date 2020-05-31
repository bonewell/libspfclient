#include "spf/graph.h"

#include "spf/microservice.h"
#include "action.h"

namespace spf {
Graph::Graph(Microservice& service) : service_{service} {}

Id Graph::addVertex() {
  return AddVertex{}.execute(service_);
}

void Graph::removeVertex(Id id) {
  RemoveVertex{id}.execute(service_);
}

void Graph::setEdge(Id from, Id to, Weight weight) {
  AddEdge{from, to, weight}.execute(service_);
}

void Graph::removeEdge(Id from, Id to) {
  RemoveEdge{from, to}.execute(service_);
}

std::list<Id> Graph::path(Id from, Id to) {
  return GetPath{from, to}.execute(service_);
}

void Graph::addVertex(std::function<void(Id, Error)> callback) {
  AddVertex{}.execute(service_, AddVertex::Id{callback});
}

void Graph::removeVertex(Id id, std::function<void(Error)> callback) {
  RemoveVertex{id}.execute(service_, callback);
}

void Graph::setEdge(Id from, Id to, Weight weight,
    std::function<void(Error)> callback) {
  AddEdge{from, to, weight}.execute(service_, callback);
}

void Graph::removeEdge(Id from, Id to, std::function<void(Error)> callback) {
  RemoveEdge{from, to}.execute(service_, callback);
}

void Graph::path(Id from, Id to,
    std::function<void(std::list<Id>, Error)> callback) {
  GetPath{from, to}.execute(service_, GetPath::Path{callback});
}
}  // namespace spf
