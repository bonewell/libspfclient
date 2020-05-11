#ifndef GRAPH_H_
#define GRAPH_H_

#include <list>

using Id = long long unsigned int;
using Weight = double;

class Graph {
public:
  Id addVertex();
  bool setEdge(Id from, Id to, Weight weight);
  std::list<Id> path(Id from, Id to);

private:
//  std::string
};

#endif /* GRAPH_H_ */
