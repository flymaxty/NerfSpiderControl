#include <iostream>
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
    uint16_t angle = atoi(argv[2]);
    if(angle < 0)
    {
        std::cout << "Argument 2 is invaild!!" << std::endl;
        return -1;      
    }

    spiderControl.barbetteRotate(speed, angle);
    return 0;
}