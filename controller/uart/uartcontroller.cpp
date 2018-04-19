/*
 * Copyright 2018 <copyright holder> <email>
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 */

#include "uartcontroller.h"
#include "uartnormalizer.h"
#include "qlearningmodel.h"
#include "types.h"

#include <errno.h>
#include <fcntl.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <iostream>

int set_interface_attribs(int fd, int speed)
{
    struct termios tty;

    if (tcgetattr(fd, &tty) < 0) {
        printf("Error from tcgetattr: %s\n", strerror(errno));
        return -1;
    }

    cfsetospeed(&tty, (speed_t)speed);
    cfsetispeed(&tty, (speed_t)speed);

    tty.c_cflag |= (CLOCAL | CREAD);    /* ignore modem controls */
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;         /* 8-bit characters */
    tty.c_cflag &= ~PARENB;     /* no parity bit */
    tty.c_cflag &= ~CSTOPB;     /* only need 1 stop bit */
    tty.c_cflag &= ~CRTSCTS;    /* no hardware flowcontrol */

    /* setup for non-canonical mode */
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
    tty.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    tty.c_oflag &= ~OPOST;

    /* fetch bytes as they become available */
    tty.c_cc[VMIN] = 1;
    tty.c_cc[VTIME] = 1;

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        printf("Error from tcsetattr: %s\n", strerror(errno));
        return -1;
    }
    return 0;
}

void set_mincount(int fd, int mcount)
{
    struct termios tty;

    if (tcgetattr(fd, &tty) < 0) {
        printf("Error tcgetattr: %s\n", strerror(errno));
        return;
    }

    tty.c_cc[VMIN] = mcount ? 1 : 0;
    tty.c_cc[VTIME] = 5;        /* half second timer */

    if (tcsetattr(fd, TCSANOW, &tty) < 0)
        printf("Error tcsetattr: %s\n", strerror(errno));
}



UartController::UartController()
{

}

UartController::~UartController()
{

}

int UartController::run(int argc, char **argv) {
    UartNormalizer norm;
    ai::QLearningModel model(norm);
    const char *portname = "/dev/ttyACM0";
    int fd;
    int wlen;

    fd = open(portname, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0) {
        printf("Error opening %s: %s\n", portname, strerror(errno));
        return -1;
    }
    /*baudrate 115200, 8 bits, no parity, 1 stop bit */
    set_interface_attribs(fd, B115200);
    set_mincount(fd, 0);                /* set to pure timed read */

    int rdlen;
    unsigned char buf;

    constexpr auto distCount = 9;
    while (1) {
        distvec dist;
        dist.reserve(distCount);
        for (int i = 0; i < distCount; ++i) {
          while(true) {
            rdlen = read(fd, &buf, sizeof(buf));
            if (rdlen > 0) {
              std::cout << "rdlen: " << rdlen << std::endl;
              dist.push_back(buf);
              wlen = write(fd, &buf, sizeof(buf));
              if (wlen != 1) {
                  std::cout << "cannot send ACK" << std::endl;
              }
              break;
            } else {
              std::cout << "wrong rdlen: " << rdlen << std::endl;
             // dist.push_back(1);
            }
          }
        }
        for(int i = 0; i < dist.size(); ++i) {
          std::cout << "dist[" << i << "]: " << (int)dist[i] << std::endl;
        }
        sleep(1);
        distvec distAvg;
        distAvg.reserve(3);
        distAvg.push_back((dist[0] + dist[3] + dist[6]) / 3);
        distAvg.push_back((dist[1] + dist[4] + dist[7]) / 3);
        distAvg.push_back((dist[2] + dist[5] + dist[8]) / 3);
        for(auto d : distAvg) {
          std::cout << "AVGD: " << d << std::endl;
        }
        auto action = model.getAction(distAvg); //TODO: Get real action.
        if (ai::IModel::Action::ACTION_TERMINATE == action) {
          int sleepCount = 10;
          while(sleepCount --> 0) {
            sleep(1);
            std::cout << "Seconds remaining: " << sleepCount << std::endl;
          }
        }
        wlen = write(fd, &action, sizeof(action));
        if (wlen != sizeof(action)) {
          std::cout << "wrong wlen: " << wlen << std::endl;
          abort();
        }
        tcdrain(fd);
    }
 
    return 0;
};
