#include "sim/sim.h"
#include <iostream>

int main(int argc, char **argv) {
  sim::Sim sim;
  return sim.run(argc, argv);
}
