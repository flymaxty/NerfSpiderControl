#include <iostream>
#include <unistd.h>
#include <stdlib.h>

#include "NerfSpiderControl.hpp"

int main(int argc, char* argv[])
{
    NerfSpiderControl spiderControl;
    if(argc < 3)
    {
        std::cout << "Error!! Need two inputs!" << std::endl;
        return -1;
    }
    
    float speed = atof(argv[1]);
    uint8_t timer = atoi(argv[2]);
    if(timer < 0)
    {
        std::cout << "Argument is invaild!!" << std::endl;
        return -1;
    }

    spiderControl.footWalk(speed);
    sleep(timer);
    spiderControl.footStop();
    return 0;
}