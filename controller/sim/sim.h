#ifndef SIM_H
#define SIM_H

#include "../icontroller.h"

namespace sim {
class Sim : public IController
{
public:
    Sim();

    int run(int argc, char **argv) override;
};
}

#endif // SIM_H
