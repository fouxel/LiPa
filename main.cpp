#include "controller/sim/sim.h"
#include "controller/uart/uartcontroller.h"
#include <iostream>
#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP

int main(int argc, char **argv) {
  UartController uart;
  sim::Sim sim;
  return sim.run(argc, argv);
}
