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
  explicit Graph(Microservice& microservice);
  Id addVertex();
  bool setEdge(Id from, Id to, Weight weight);
  std::list<Id> path(Id from, Id to);

private:
  ptree::ptree invoke(ptree::ptree const& request);
  Microservice& service_;
};

#endif /* GRAPH_H_ */
