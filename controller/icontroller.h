#ifndef CONTROLLER_H
#define CONTROLLER_H

class IController
{
public:
    virtual ~IController() {}

    virtual int run(int argc, char **argv) = 0;
};

#endif // CONTROLLER_H
