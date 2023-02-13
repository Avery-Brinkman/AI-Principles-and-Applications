cmake_minimum_required(VERSION 3.8)

project("hw2")

add_executable(hw2 "main.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET hw2 PROPERTY CXX_STANDARD 20)
endif()
