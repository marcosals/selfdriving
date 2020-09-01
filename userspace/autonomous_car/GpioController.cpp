#include "GpioContoller.h"

namespace autonomous
{

    int setup()
    {
        // validate gpio sysfs folder exists


    }

    GpioController::GpioController()
    {

    }

    int GpioController::GPIOExport(int pin)
    {
    #define BUFFER_MAX 3
        char buffer[BUFFER_MAX];
        ssize_t bytes_written;
        int fd;

        fd = open("/sys/class/gpio/export", O_WRONLY);
        if (-1 == fd) {
            fprintf(stderr, "Failed to open export for writing!\n");
            return(-1);
        }

        bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
        write(fd, buffer, bytes_written);
        close(fd);
        return(0);

    }
    
    int GpioController::GPIOUnexport(int pin)
    {
        char buffer[BUFFER_MAX];
        ssize_t bytes_written;
        int fd;

        fd = open("/sys/class/gpio/unexport", O_WRONLY);
        if (-1 == fd) {
            fprintf(stderr, "Failed to open unexport for writing!\n");
            return(-1);
        }

        bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
        write(fd, buffer, bytes_written);
        close(fd);
        return(0);
    }
            
    int GpioController::GPIODirection(int pin, int dir)
    {
        static const char s_directions_str[]  = "in\0out";

    #define DIRECTION_MAX 35
        char path[DIRECTION_MAX];
        int fd;

        snprintf(path, DIRECTION_MAX, "/sys/class/gpio/gpio%d/direction", pin);
        fd = open(path, O_WRONLY);
        if (-1 == fd) {
            fprintf(stderr, "Failed to open gpio direction for writing!\n");
            return(-1);
        }

        if (-1 == write(fd, &s_directions_str[IN == dir ? 0 : 3], IN == dir ? 2 : 3)) {
            fprintf(stderr, "Failed to set direction!\n");
            return(-1);
        }

        close(fd);
        return(0);
    }
    
    int GpioController::GPIORead(int pin)
    {
    #define VALUE_MAX 30
        char path[VALUE_MAX];
        char value_str[3];
        int fd;

        snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
        fd = open(path, O_RDONLY);
        if (-1 == fd) {
            fprintf(stderr, "Failed to open gpio value for reading!\n");
            return(-1);
        }

        if (-1 == read(fd, value_str, 3)) {
            fprintf(stderr, "Failed to read value!\n");
            return(-1);
        }

        close(fd);

        return(atoi(value_str));
    }
    
    int GpioController::GPIOWrite(int pin, const char* value, int length)
    {
        char path[VALUE_MAX];
        int fd;

        snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
        fd = open(path, O_WRONLY);
        if (-1 == fd) {
            fprintf(stderr, "Failed to open gpio value for writing!\n");
            return(-1);
        }

        if (1 != write(fd, value, length)) {
            fprintf(stderr, "Failed to write pin: %d, value: %s\n", pin, value);
            return(-1);
        }

        close(fd);
        return(0);
    }

}