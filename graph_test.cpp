#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "mock_microservice.h"
#include "graph.h"

using ::testing::_;
using ::testing::Eq;
using ::testing::ContainerEq;
using ::testing::Return;

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

  EXPECT_THROW(g.addVertex(), Graph::Error);
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

  EXPECT_THROW(g.removeVertex(100), Graph::Error);
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

  EXPECT_THROW(g.setEdge(100, 2, 7), Graph::Error);
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

  EXPECT_THROW(g.removeEdge(100, 2), Graph::Error);
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

  EXPECT_THROW(g.path(0, 4), Graph::Error);
}
