#ifndef GRAPH_H_
#define GRAPH_H_

#include <functional>
#include <list>

#include "error.h"
#include "types.h"

namespace spf {
class Microservice;

/**
 * Represents Graph on server.
 */
class Graph {
public:
  /**
   * Creates graph on remote microservice.
   * @param microservice.
   */
  explicit Graph(Microservice& microservice);

  /**
   * Adds new vertex in graph.
   * @return id of the vertex.
   */
  Id addVertex();

  /**
   * Adds new vertex in graph asynchronously.
   * @param callback - callback to invoke.
   */
  void addVertex(std::function<void(Id, Error)> callback);

  /**
   * Removes vertex and its edges.
   * @param id - the vertex.
   */
  void removeVertex(Id id);

  /**
   * Removes vertex and its edges.
   * @param id - the vertex.
   * @param callback - callback to invoke.
   */
  void removeVertex(Id id, std::function<void(Error)> callback);

  /**
   * Adds a new edge or update distance of the already existed one.
   * @param from - begin the edge.
   * @param to - end of the edge.
   * @param distance - weight of the edge.
   */
  void setEdge(Id from, Id to, Weight weight);

  /**
   * Adds a new edge or update distance of the already existed one.
   * @param from - begin the edge.
   * @param to - end of the edge.
   * @param distance - weight of the edge.
   * @param callback - callback to invoke.
   */
  void setEdge(Id from, Id to, Weight weight,
      std::function<void(Error)> callback);

  /**
   * Remove edge.
   * @param from - begin the edge.
   * @param to - end of the edge.
   */
  void removeEdge(Id from, Id to);

  /**
   * Remove edge.
   * @param from - begin the edge.
   * @param to - end of the edge.
   * @param callback - callback to invoke.
   */
  void removeEdge(Id from, Id to, std::function<void(Error)> callback);

  /**
   * Gets path from a source to a target vertex.
   * @param from - the source vertex.
   * @param to - the target vertex.
   * @param force - calculate distances also if true.
   * @return list of the vertex by order.
   */
  std::list<Id> path(Id from, Id to);

  /**
   * Gets path from a source to a target vertex.
   * @param from - the source vertex.
   * @param to - the target vertex.
   * @param force - calculate distances also if true.
   * @param callback - callback to invoke.
   */
  void path(Id from, Id to,
      std::function<void(std::list<Id>, Error)> callback);

private:
  Microservice& service_;
};
}  // namespace spf

#endif /* GRAPH_H_ */
