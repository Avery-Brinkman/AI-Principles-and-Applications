cmake_minimum_required(VERSION 3.8)

project("hw2")

set(SRC_FILES
  src/GameBoard.cpp
  src/main.cpp
)

set(INCLUDE_FILES
  include/GameBoard.h
)

add_executable(hw2 ${SRC_FILES} ${INCLUDE_FILES})
target_include_directories(hw2 PRIVATE include)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET hw2 PROPERTY CXX_STANDARD 20)
endif()
