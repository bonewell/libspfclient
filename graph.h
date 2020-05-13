#ifndef GRAPH_H_
#define GRAPH_H_

#include <boost/property_tree/ptree.hpp>
#include <list>

namespace ptree = boost::property_tree;

using Id = long long unsigned int;
using Weight = double;

class Microservice;

class Graph {
public:
  using Error = std::runtime_error;

  /**
   * Connects to microservice.
   * @param microservice.
   */
  explicit Graph(Microservice& microservice);

  /**
   * Adds new vertex in graph.
   * @return id of the vertex.
   */
  Id addVertex();

  /**
   * Removes vertex and its edges.
   * @param id - the vertex.
   */
  void removeVertex(Id id);

  /**
   * Adds a new edge or update distance of the already existed one.
   * @param from - begin the edge.
   * @param to - end of the edge.
   * @param distance - weight of the edge.
   */
  void setEdge(Id from, Id to, Weight weight);

  /**
   * Remove edge.
   * @param from - begin the edge.
   * @param to - end of the edge.
   */
  void removeEdge(Id from, Id to);

  /**
   * Gets path from a source to a target vertex.
   * @param from - the source vertex.
   * @param to - the target vertex.
   * @param force - calculate distances also if true.
   * @return list of the vertex by order.
   */
  std::list<Id> path(Id from, Id to);

private:
  ptree::ptree invoke(ptree::ptree const& request);
  Microservice& service_;
};

#endif /* GRAPH_H_ */
