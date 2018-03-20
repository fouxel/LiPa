cmake_minimum_required(VERSION 3.0)
project(uart_controller)

include_directories(../../dl)

add_library(uart_controller STATIC
      uartcontroller.h
      uartcontroller.cpp
      uartnormalizer.h
      uartnormalizer.cpp
)

