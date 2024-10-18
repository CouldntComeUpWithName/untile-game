#include <iostream>
#include <algorithm>
#include <UntiledEngine.h>
#include "cubes.h"
#include <map>

class Sandbox : public utd::application
{
public:
    Sandbox(const utd::cmdline_args& args)
        : application(args)
    {

        push_layer(new cubes());
    }
}; 

int main(int argc, char** argv)
{
    utd::logging::init("Sandbox");
    
    Sandbox app(utd::cmdline_args{argc, argv});
    Sandbox::instance().run();
    
    return 0;
}



