#ifndef UARTCONTROLLER_H
#define UARTCONTROLLER_H

#include "../icontroller.h"

class UartController : public IController
{
public:
  UartController();
  ~UartController();
  
  int run(int argc, char **argv) override;
};

#endif // UARTCONTROLLER_H
