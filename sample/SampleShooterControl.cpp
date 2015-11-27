#include <iostream>
#include <unistd.h>
#include <stdlib.h>


#include "NerfSpiderControl.hpp"

int main(int argc, char* argv[])
{
    NerfSpiderControl spiderControl;
    if(argc < 2)
    {
        std::cout << "Error!! Need two inputs!" << std::endl;
        return -1;
    }

    uint8_t timer = atoi(argv[1]);
    if(timer < 0)
    {
        std::cout << "Argument is invaild!!" << std::endl;
        return -1;
    }

    spiderControl.shooterControl();
    sleep(timer);
    spiderControl.shootAllStop();
    return 0;
}