#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "mock_microservice.h"
#include "graph.h"

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

TEST(Graph, SetEdge) {
  MockMicroservice mock;
  Graph g{mock};

  EXPECT_CALL(mock,
      invoke(R"({"action":"AddEdge","from":"1","to":"2","weight":"7"})"))
          .WillOnce(Return(R"({})"));

  EXPECT_THAT(g.setEdge(1, 2, 7), Eq(true));
}

TEST(Graph, GetPath) {
  MockMicroservice mock;
  Graph g{mock};

  EXPECT_CALL(mock, invoke(R"({"action":"GetPath","from":"0","to":"4"})"))
    .WillOnce(Return(R"({"ids":["0","2","4"]})"));

  EXPECT_THAT(g.path(0, 4), ContainerEq(std::list<Id>{0, 2, 4}));
}
