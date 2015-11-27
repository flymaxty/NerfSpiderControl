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

    uint8_t enable = atoi(argv[1]);

    if(enable > 0)
        spiderControl.laserControl(true);
    else
        spiderControl.laserControl(false);
    return 0;
}