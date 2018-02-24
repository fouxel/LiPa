#include "sim/sim.h"
#include <iostream>
#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP

int main(int argc, char **argv) {
  sim::Sim sim;
  return sim.run(argc, argv);
}
