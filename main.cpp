#include <iostream>
#include "sim/sim.h"

int main(int argc, char **argv) {
	sim::Sim sim;
	return sim.run(argc, argv);
}
