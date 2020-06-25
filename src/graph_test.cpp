#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "mock_rpc.h"
#include "spf/graph.h"

using ::testing::_;
using ::testing::Eq;
using ::testing::ContainerEq;
using ::testing::Return;

using namespace spf;

TEST(Graph, AddVertex) {
  MockRpc mock;
  Graph g{mock};

  EXPECT_CALL(mock, invoke(R"({"action":"AddVertex"})"))
      .WillOnce(Return(R"({"id":"1"})"));

  EXPECT_THAT(g.addVertex(), Eq(1));
}

TEST(Graph, AddVertexWithError) {
  MockRpc mock;
  Graph g{mock};

  EXPECT_CALL(mock, invoke(_)).WillOnce(Return(R"({"error":"Internal error"})"));

  EXPECT_THROW(g.addVertex(), Error);
}

TEST(Graph, RemoveVertex) {
  MockRpc mock;
  Graph g{mock};

  EXPECT_CALL(mock, invoke(R"({"action":"RemoveVertex","id":"1"})"))
      .WillOnce(Return(R"({})"));

  EXPECT_NO_THROW(g.removeVertex(1));
}

TEST(Graph, RemoveVertexWithError) {
  MockRpc mock;
  Graph g{mock};

  EXPECT_CALL(mock, invoke(_)).WillOnce(Return(R"({"error":"Wrong ID"})"));

  EXPECT_THROW(g.removeVertex(100), Error);
}

TEST(Graph, SetEdge) {
  MockRpc mock;
  Graph g{mock};

  EXPECT_CALL(mock,
      invoke(R"({"action":"AddEdge","from":"1","to":"2","weight":"7"})"))
          .WillOnce(Return(R"({})"));

  EXPECT_NO_THROW(g.setEdge(1, 2, 7));
}

TEST(Graph, SetEdgeWithError) {
  MockRpc mock;
  Graph g{mock};

  EXPECT_CALL(mock, invoke(_)).WillOnce(Return(R"({"error":"Wrong vertex ID"})"));

  EXPECT_THROW(g.setEdge(100, 2, 7), Error);
}

TEST(Graph, RemoveEdge) {
  MockRpc mock;
  Graph g{mock};

  EXPECT_CALL(mock,
      invoke(R"({"action":"RemoveEdge","from":"1","to":"2"})"))
          .WillOnce(Return(R"({})"));

  EXPECT_NO_THROW(g.removeEdge(1, 2));
}

TEST(Graph, RemoveEdgeWithError) {
  MockRpc mock;
  Graph g{mock};

  EXPECT_CALL(mock, invoke(_)).WillOnce(Return(R"({"error":"Wrong vertex ID"})"));

  EXPECT_THROW(g.removeEdge(100, 2), Error);
}

TEST(Graph, GetPath) {
  MockRpc mock;
  Graph g{mock};

  EXPECT_CALL(mock, invoke(R"({"action":"GetPath","from":"0","to":"4"})"))
    .WillOnce(Return(R"({"ids":["0","2","4"]})"));

  EXPECT_THAT(g.path(0, 4), ContainerEq(std::list<Id>{0, 2, 4}));
}

TEST(Graph, GetPathWithError) {
  MockRpc mock;
  Graph g{mock};

  EXPECT_CALL(mock, invoke(_)).WillOnce(Return(R"({"error":"Wrong ID"})"));

  EXPECT_THROW(g.path(0, 4), Error);
}

TEST(Graph, AddVertexAsync) {
  MockRpc mock;
  Graph g{mock};

  EXPECT_CALL(mock, async_invoke(R"({"action":"AddVertex"})", _))
      .WillOnce([](auto, auto callback) {
        callback(R"({"id":"1"})", {});
      });

  Id expected;
  g.addVertex([&expected](Id id, auto) { expected = id; });
  EXPECT_THAT(expected, Eq(1));
}

TEST(Graph, AddVertexAsyncWithError) {
  MockRpc mock;
  Graph g{mock};

  EXPECT_CALL(mock, async_invoke(_, _))
      .WillOnce([](auto, auto callback) {
        callback(R"({"vertex":"1"})", {});
      });

  Error expected;
  g.addVertex([&expected](Id, auto error) { expected = error; });
  EXPECT_THAT(bool{expected}, Eq(true));
}

TEST(Graph, RemoveVertexAsync) {
  MockRpc mock;
  Graph g{mock};

  EXPECT_CALL(mock, async_invoke(R"({"action":"RemoveVertex","id":"1"})", _))
      .WillOnce([](auto, auto callback) {
        callback(R"({})", {});
      });

  Error expected;
  g.removeVertex(1, [&expected](auto error) { expected = error; });
  EXPECT_THAT(bool{expected}, Eq(false));
}

TEST(Graph, RemoveVertexAsyncWithError) {
  MockRpc mock;
  Graph g{mock};

  EXPECT_CALL(mock, async_invoke(_, _))
      .WillOnce([](auto, auto callback) {
        callback(R"({"error":"Internal error"})", {});
      });

  Error expected;
  g.removeVertex(100, [&expected](auto error) { expected = error; });
  EXPECT_THAT(bool{expected}, Eq(true));
}

TEST(Graph, SetEdgeAsync) {
  MockRpc mock;
  Graph g{mock};

  EXPECT_CALL(mock,
      async_invoke(R"({"action":"AddEdge","from":"0","to":"1","weight":"7"})", _))
      .WillOnce([](auto, auto callback) {
        callback(R"({})", {});
      });

  Error expected;
  g.setEdge(0, 1, 7, [&expected](auto error) { expected = error; });
  EXPECT_THAT(bool{expected}, Eq(false));
}

TEST(Graph, SetEdgeAsyncWithError) {
  MockRpc mock;
  Graph g{mock};

  EXPECT_CALL(mock, async_invoke(_, _))
      .WillOnce([](auto, auto callback) {
        callback(R"({"error":"Internal error"})", {});
      });

  Error expected;
  g.setEdge(0, 1, -3.4, [&expected](auto error) { expected = error; });
  EXPECT_THAT(bool{expected}, Eq(true));
}

TEST(Graph, RemoveEdgeAsync) {
  MockRpc mock;
  Graph g{mock};

  EXPECT_CALL(mock,
      async_invoke(R"({"action":"RemoveEdge","from":"0","to":"1"})", _))
      .WillOnce([](auto, auto callback) {
        callback(R"({})", {});
      });

  Error expected;
  g.removeEdge(0, 1, [&expected](auto error) { expected = error; });
  EXPECT_THAT(bool{expected}, Eq(false));
}

TEST(Graph, RemoveEdgeAsyncWithError) {
  MockRpc mock;
  Graph g{mock};

  EXPECT_CALL(mock, async_invoke(_, _))
      .WillOnce([](auto, auto callback) {
        callback(R"({"error":"Internal error"})", {});
      });

  Error expected;
  g.removeEdge(0, 1, [&expected](auto error) { expected = error; });
  EXPECT_THAT(bool{expected}, Eq(true));
}

TEST(Graph, GetPathAsync) {
  MockRpc mock;
  Graph g{mock};

  EXPECT_CALL(mock,
      async_invoke(R"({"action":"GetPath","from":"0","to":"4"})", _))
      .WillOnce([](auto, auto callback) {
        callback(R"({"ids":["0","2","4"]})", {});
      });

  std::list<Id> expected;
  g.path(0, 4, [&expected](auto ids, auto) { expected = ids; });
  EXPECT_THAT(expected, ContainerEq(std::list<Id>{0, 2, 4}));
}

TEST(Graph, GetPathAsyncWithError) {
  MockRpc mock;
  Graph g{mock};

  EXPECT_CALL(mock, async_invoke(_, _))
      .WillOnce([](auto, auto callback) {
        callback(R"({"vertexes":["0","4"]})", {});
      });

  Error expected;
  g.path(0, 4, [&expected](auto, auto error) { expected = error; });
  EXPECT_THAT(bool{expected}, Eq(true));
}
