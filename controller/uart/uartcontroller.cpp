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
    char *portname = "/dev/ttyACM0";
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

    /* simple output */
   // wlen = write(fd, "Hello!\n", 7);
   // if (wlen != 7) {
    //    printf("Error from write: %d, %d\n", wlen, errno);
    //}
    //tcdrain(fd);    /* delay for output */

    bool startSequenceEnd = false;
    int rdlen;
    unsigned char buf;
    do {
        rdlen = read(fd, &buf, sizeof(buf));
        if (rdlen > 0) {
            if (buf != 1) {
              continue;
            }
            rdlen = read(fd, &buf, sizeof(buf));
            if (rdlen > 0) {
              if (buf == 2) {
                startSequenceEnd = true;
              }
            }
        } else if (rdlen < 0) {
            printf("Error from read: %d: %s\n", rdlen, strerror(errno));
        }
    } while (!startSequenceEnd);
    
    while (1) {
        unsigned char dist[2];
        for (int i = 0; i < 2; ++i) {
          rdlen = read(fd, &buf, sizeof(buf));
          if (rdlen > 0) {
            dist[i] = buf;
          } else {
            abort();
          }
        }
        for(int i = 0; i < 2; ++i) {
          std::cout << "dist[" << i << "]: " << (int)dist[i] << std::endl;
        }
        uint8_t action = 2; //TODO: Get real action.
        wlen = write(fd, &action, sizeof(action));
        if (wlen != sizeof(action)) {
          abort();
        }
        tcdrain(fd);
        read(fd, &buf, sizeof(buf));
        read(fd, &buf, sizeof(buf));
    }
 
    return 0;
};
