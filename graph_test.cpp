#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "mock_microservice.h"
#include "graph.h"

using ::testing::_;
using ::testing::Eq;
using ::testing::ContainerEq;
using ::testing::Return;

using namespace spf;

TEST(Graph, AddVertex) {
  MockMicroservice mock;
  Graph g{mock};

  EXPECT_CALL(mock, invoke(R"({"action":"AddVertex"})"))
      .WillOnce(Return(R"({"id":"1"})"));

  EXPECT_THAT(g.addVertex(), Eq(1));
}

TEST(Graph, AddVertexWithError) {
  MockMicroservice mock;
  Graph g{mock};

  EXPECT_CALL(mock, invoke(_)).WillOnce(Return(R"({"error":"Internal error"})"));

  EXPECT_THROW(g.addVertex(), Error);
}

TEST(Graph, RemoveVertex) {
  MockMicroservice mock;
  Graph g{mock};

  EXPECT_CALL(mock, invoke(R"({"action":"RemoveVertex","id":"1"})"))
      .WillOnce(Return(R"({})"));

  EXPECT_NO_THROW(g.removeVertex(1));
}

TEST(Graph, RemoveVertexWithError) {
  MockMicroservice mock;
  Graph g{mock};

  EXPECT_CALL(mock, invoke(_)).WillOnce(Return(R"({"error":"Wrong ID"})"));

  EXPECT_THROW(g.removeVertex(100), Error);
}

TEST(Graph, SetEdge) {
  MockMicroservice mock;
  Graph g{mock};

  EXPECT_CALL(mock,
      invoke(R"({"action":"AddEdge","from":"1","to":"2","weight":"7"})"))
          .WillOnce(Return(R"({})"));

  EXPECT_NO_THROW(g.setEdge(1, 2, 7));
}

TEST(Graph, SetEdgeWithError) {
  MockMicroservice mock;
  Graph g{mock};

  EXPECT_CALL(mock, invoke(_)).WillOnce(Return(R"({"error":"Wrong vertex ID"})"));

  EXPECT_THROW(g.setEdge(100, 2, 7), Error);
}

TEST(Graph, RemoveEdge) {
  MockMicroservice mock;
  Graph g{mock};

  EXPECT_CALL(mock,
      invoke(R"({"action":"RemoveEdge","from":"1","to":"2"})"))
          .WillOnce(Return(R"({})"));

  EXPECT_NO_THROW(g.removeEdge(1, 2));
}

TEST(Graph, RemoveEdgeWithError) {
  MockMicroservice mock;
  Graph g{mock};

  EXPECT_CALL(mock, invoke(_)).WillOnce(Return(R"({"error":"Wrong vertex ID"})"));

  EXPECT_THROW(g.removeEdge(100, 2), Error);
}

TEST(Graph, GetPath) {
  MockMicroservice mock;
  Graph g{mock};

  EXPECT_CALL(mock, invoke(R"({"action":"GetPath","from":"0","to":"4"})"))
    .WillOnce(Return(R"({"ids":["0","2","4"]})"));

  EXPECT_THAT(g.path(0, 4), ContainerEq(std::list<Id>{0, 2, 4}));
}

TEST(Graph, GetPathWithError) {
  MockMicroservice mock;
  Graph g{mock};

  EXPECT_CALL(mock, invoke(_)).WillOnce(Return(R"({"error":"Wrong ID"})"));

  EXPECT_THROW(g.path(0, 4), Error);
}

TEST(Graph, AddVertexAsync) {
  MockMicroservice mock;
  Graph g{mock};

  EXPECT_CALL(mock, async_invoke(R"({"action":"AddVertex"})", _))
      .WillOnce([](auto, auto callback) {
        callback(R"({"id":"1"})", {});
      });

  g.addVertex([](Id id, auto) {
    EXPECT_THAT(id, Eq(1));
  });
}

TEST(Graph, AddVertexAsyncWithError) {
  MockMicroservice mock;
  Graph g{mock};

  EXPECT_CALL(mock, async_invoke(_, _))
      .WillOnce([](auto, auto callback) {
        callback(R"({"vertex":"1"})", {});
      });

  g.addVertex([](Id id, auto error) {
    EXPECT_THAT(bool{error}, Eq(true));
  });
}

TEST(Graph, RemoveVertexAsync) {
  MockMicroservice mock;
  Graph g{mock};

  EXPECT_CALL(mock, async_invoke(R"({"action":"RemoveVertex","id":"1"})", _))
      .WillOnce([](auto, auto callback) {
        callback(R"({})", {});
      });

  g.removeVertex(1, [](auto error) {
    EXPECT_THAT(bool{error}, Eq(false));
  });
}

TEST(Graph, RemoveVertexAsyncWithError) {
  MockMicroservice mock;
  Graph g{mock};

  EXPECT_CALL(mock, async_invoke(_, _))
      .WillOnce([](auto, auto callback) {
        callback(R"({"error":"Internal error"})", {});
      });

  g.removeVertex(100, [](auto error) {
    EXPECT_THAT(bool{error}, Eq(true));
  });
}

TEST(Graph, SetEdgeAsync) {
  MockMicroservice mock;
  Graph g{mock};

  EXPECT_CALL(mock,
      async_invoke(R"({"action":"AddEdge","from":"0","to":"1","weight":"7"})", _))
      .WillOnce([](auto, auto callback) {
        callback(R"({})", {});
      });

  g.setEdge(0, 1, 7, [](auto error) {
    EXPECT_THAT(bool{error}, Eq(false));
  });
}

TEST(Graph, SetEdgeAsyncWithError) {
  MockMicroservice mock;
  Graph g{mock};

  EXPECT_CALL(mock, async_invoke(_, _))
      .WillOnce([](auto, auto callback) {
        callback(R"({"error":"Internal error"})", {});
      });

  g.setEdge(0, 1, -3.4, [](auto error) {
    EXPECT_THAT(bool{error}, Eq(true));
  });
}

TEST(Graph, RemoveEdgeAsync) {
  MockMicroservice mock;
  Graph g{mock};

  EXPECT_CALL(mock,
      async_invoke(R"({"action":"RemoveEdge","from":"0","to":"1"})", _))
      .WillOnce([](auto, auto callback) {
        callback(R"({})", {});
      });

  g.removeEdge(0, 1, [](auto error) {
    EXPECT_THAT(bool{error}, Eq(false));
  });
}

TEST(Graph, RemoveEdgeAsyncWithError) {
  MockMicroservice mock;
  Graph g{mock};

  EXPECT_CALL(mock, async_invoke(_, _))
      .WillOnce([](auto, auto callback) {
        callback(R"({"error":"Internal error"})", {});
      });

  g.removeEdge(0, 1, [](auto error) {
    EXPECT_THAT(bool{error}, Eq(true));
  });
}

TEST(Graph, GetPathAsync) {
  MockMicroservice mock;
  Graph g{mock};

  EXPECT_CALL(mock,
      async_invoke(R"({"action":"GetPath","from":"0","to":"4"})", _))
      .WillOnce([](auto, auto callback) {
        callback(R"({"ids":["0","2","4"]})", {});
      });

  g.path(0, 4, [](auto ids, auto) {
    EXPECT_THAT(ids, ContainerEq(std::list<Id>{0, 2, 4}));
  });
}

TEST(Graph, GetPathAsyncWithError) {
  MockMicroservice mock;
  Graph g{mock};

  EXPECT_CALL(mock, async_invoke(_, _))
      .WillOnce([](auto, auto callback) {
        callback(R"({"vertexes":["0","4"]})", {});
      });

  g.path(0, 4, [](auto, auto error) {
    EXPECT_THAT(bool{error}, Eq(true));
  });
}
