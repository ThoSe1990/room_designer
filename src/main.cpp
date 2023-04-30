
#include "pch.hpp"

#include "engine/engine.hpp"


int main(int argc, char* argv[])
{
    cwt::engine engine;

    while (engine.is_running())
    {
        engine.read_input();
        engine.update();
        engine.render();
    }
    
    return 0;
}