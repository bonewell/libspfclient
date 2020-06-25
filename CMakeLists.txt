cmake_minimum_required(VERSION 3.10)

project(spfclient)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

add_library(objlib OBJECT
  "src/graph.cpp"
  "src/web_socket_rpc.cpp"
)
target_compile_features(objlib PRIVATE cxx_std_17)
target_link_libraries(objlib PRIVATE
  CONAN_PKG::boost
)
target_include_directories(objlib
  PUBLIC include
)
set_property(TARGET objlib PROPERTY POSITION_INDEPENDENT_CODE 1)

add_library(${PROJECT_NAME}_static STATIC $<TARGET_OBJECTS:objlib>)
add_library(SPF::client_static ALIAS ${PROJECT_NAME}_static)
target_include_directories(${PROJECT_NAME}_static
  PUBLIC include
)

add_library(${PROJECT_NAME} SHARED $<TARGET_OBJECTS:objlib>)
add_library(SPF::client ALIAS ${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PRIVATE
  CONAN_PKG::boost
)
target_include_directories(${PROJECT_NAME}
  PUBLIC include
)
install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION lib
)
install(DIRECTORY include/spf DESTINATION include)

enable_testing()

set(UNIT_TEST ${PROJECT_NAME}_unittest)
add_executable(${UNIT_TEST}
  "src/graph_test.cpp"
)
target_compile_features(${UNIT_TEST} PRIVATE cxx_std_17)
target_link_libraries(${UNIT_TEST} PRIVATE
  CONAN_PKG::gtest
  SPF::client_static
)
add_test(NAME ${UNIT_TEST} COMMAND ${UNIT_TEST})

add_executable(${PROJECT_NAME}_cmd
  "src/main.cpp"
)
target_compile_features(${PROJECT_NAME}_cmd PRIVATE cxx_std_17)
target_link_libraries(${PROJECT_NAME}_cmd PRIVATE
  CONAN_PKG::boost
  SPF::client_static
)
