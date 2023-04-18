

#include <iostream>
#include "LabEngine.h"


int main(int argc, char** argv)
{
    std::cout << "ICT397 Lab 4!\n";

    // entry point that creates and starts the game engine
    ///pixelsEnvironment
    auto &app = Lab::LabEngine::getInstance();
    app.run();
    
    return 0;
}

