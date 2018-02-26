cmake_minimum_required(VERSION 3.0)
project(uart_controller)

add_library(uart_controller STATIC
      uartcontroller.h
      uartcontroller.cpp
)
