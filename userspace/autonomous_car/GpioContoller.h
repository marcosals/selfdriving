#ifndef GPIO_CONTROLLER_CPP
#define GPIO_CONTROLLER_CPP

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define IN  0
#define OUT 1

#define LOW  "0"
#define HIGH "1"



namespace autonomous
{
    class GpioController
    {
        public:
            //int pinMode(int pin, int direction);
            //int digitalWrite(int pin, int value);

        //private:
            GpioController();
            static int GPIOExport(int pin);
            static int GPIOUnexport(int pin);
            static int GPIODirection(int pin, int dir);
            static int GPIORead(int pin);
            static int GPIOWrite(int pin, const char* value, int length);
    };
};

#endif