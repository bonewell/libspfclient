cmake_minimum_required(VERSION 3.10)

project(spfclient)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

add_library(objlib OBJECT
  "graph.cpp"
  "web_socket_microservice.cpp"
)
set_property(TARGET objlib PROPERTY POSITION_INDEPENDENT_CODE 1)
target_compile_features(objlib PRIVATE cxx_std_17)
target_link_libraries(objlib PRIVATE
  CONAN_PKG::boost
)
target_include_directories(objlib
  INTERFACE ${CMAKE_SOURCE_DIR}
)

# add_library(${PROJECT_NAME} SHARED $<TARGET_OBJECTS:objlib>)
# add_library(SPF::client ALIAS ${PROJECT_NAME})
add_library(${PROJECT_NAME}_static STATIC $<TARGET_OBJECTS:objlib>)
add_library(SPF::client_static ALIAS ${PROJECT_NAME}_static)

enable_testing()

set(UNIT_TEST ${PROJECT_NAME}_unittest)
add_executable(${UNIT_TEST}
  "graph.cpp"
  "graph_test.cpp"
)
target_compile_features(${UNIT_TEST} PRIVATE cxx_std_17)
target_link_libraries(${UNIT_TEST} PRIVATE
  CONAN_PKG::gtest
  CONAN_PKG::boost
)
add_test(NAME ${UNIT_TEST} COMMAND ${UNIT_TEST})

add_executable(${PROJECT_NAME}_cmd
  "main.cpp"
)
target_compile_features(${PROJECT_NAME}_cmd PRIVATE cxx_std_17)
target_link_libraries(${PROJECT_NAME}_cmd PRIVATE
  CONAN_PKG::boost
  SPF::client_static
)