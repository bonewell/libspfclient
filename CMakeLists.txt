cmake_minimum_required(VERSION 3.10)

project(spfclient)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

add_library(objlib OBJECT
  "graph.cpp"
)
set_property(TARGET objlib PROPERTY POSITION_INDEPENDENT_CODE 1)
target_compile_features(objlib PRIVATE cxx_std_17)
target_link_libraries(objlib PRIVATE
  CONAN_PKG::boost
)
target_include_directories(objlib
  INTERFACE ${CMAKE_SOURCE_DIR}
)

add_library(${PROJECT_NAME} SHARED $<TARGET_OBJECTS:objlib>)
add_library(SPF::client ALIAS ${PROJECT_NAME})
add_library(${PROJECT_NAME}_static STATIC $<TARGET_OBJECTS:objlib>)
add_library(SPF::client_static ALIAS ${PROJECT_NAME}_static)

add_executable(${PROJECT_NAME}_cmd
  "main.cpp"
)
target_compile_features(${PROJECT_NAME}_cmd PRIVATE cxx_std_17)
target_link_libraries(${PROJECT_NAME}_cmd PRIVATE
  SPF::client_static
)