#include <iostream>
#include <stdlib.h>

#include "NerfSpiderControl.hpp"

int main(int argc, char* argv[])
{
    NerfSpiderControl spiderControl;
    if(argc < 2)
    {
        std::cout << "Error!! Need one inputs!" << std::endl;
        return -1;
    }

    uint8_t bulletNumber = atoi(argv[1]);
    if(bulletNumber < 0)
    {
        std::cout << "Argument 1 is invaild!!" << std::endl;
        return -1;
    }

    spiderControl.shootControl(bulletNumber);
    return 0;
}