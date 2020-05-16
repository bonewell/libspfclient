#include <boost/program_options.hpp>

#include <iostream>

#include "graph.h"
#include "web_socket_microservice.h"

namespace po = boost::program_options;

using namespace spf;

void run(Microservice& service) {
  Graph g{service};
  g.addVertex(); g.addVertex(); g.addVertex(); g.addVertex();
  g.addVertex(); g.addVertex();
  g.setEdge(0, 1, 7); g.setEdge(0, 2, 9); g.setEdge(0, 5, 14);
  g.setEdge(1, 0, 7); g.setEdge(1, 2, 10); g.setEdge(1, 3, 15);
  g.setEdge(2, 0, 9); g.setEdge(2, 1, 10); g.setEdge(2, 5, 2);
  g.setEdge(3, 1, 15); g.setEdge(3, 2, 11); g.setEdge(3, 4, 6);
  g.setEdge(4, 3, 6); g.setEdge(4, 5, 9);
  g.setEdge(5, 0, 14); g.setEdge(5, 2, 2); g.setEdge(5, 4, 9);

  auto path = g.path(0, 4);

  std::cout << "Path: ";
  for (Id id: path) std::cout << id << ' ';
  std::cout << '\n';
}

void async_run(Microservice& service) {
  Graph g{service};

  auto add_error = [](Id id, auto error) {
    if (error) {
      std::cout << error.what() << '\n';
    } else {
      std::cout << "Vertex: " << id << '\n';
    }
  };
  g.addVertex(add_error); g.addVertex(add_error); g.addVertex(add_error);
  g.addVertex(add_error); g.addVertex(add_error); g.addVertex(add_error);

  auto set_error = [](auto error) {
    if (error) std::cout << error.what() << '\n';
  };
  g.setEdge(0, 1, 7, set_error);
  g.setEdge(0, 2, 9, set_error);
  g.setEdge(0, 5, 14, set_error);
  g.setEdge(1, 0, 7, set_error);
  g.setEdge(1, 2, 10, set_error);
  g.setEdge(1, 3, 15, set_error);
  g.setEdge(2, 0, 9, set_error);
  g.setEdge(2, 1, 10, set_error);
  g.setEdge(2, 5, 2, set_error);
  g.setEdge(3, 1, 15, set_error);
  g.setEdge(3, 2, 11, set_error);
  g.setEdge(3, 4, 6, set_error);
  g.setEdge(4, 3, 6, set_error);
  g.setEdge(4, 5, 9, set_error);
  g.setEdge(5, 0, 14, set_error);
  g.setEdge(5, 2, 2, set_error);
  g.setEdge(5, 4, 9, set_error);

  g.path(0, 4, [](auto path, auto error) {
    if (!error) {
      std::cout << "Path: ";
      for (Id id: path) std::cout << id << ' ';
      std::cout << '\n';
    } else {
      std::cout << error.what() << '\n';
    }
  });
}

int main(int argc, char* argv[]) {
  bool async{false};
  std::string host{"localhost"};
  unsigned short port{8080};

  po::options_description args("Using");
  args.add_options()
    ("help", "produce help message")
    ("async", po::value<bool>(&async)->default_value(false), "asynchronous client")
    ("host", po::value<std::string>(&host)->default_value("localhost"), "host to connect")
    ("port", po::value<unsigned short>(&port)->default_value(8080), "port to connect");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, args), vm);
  po::notify(vm);

  if (vm.count("help")) {
      std::cout << args << "\n";
      return 1;
  }

  WebSocketMicroservice service{host, port};
  if (async) {
    async_run(service);
  } else {
    run(service);
  }

  return 0;
}
