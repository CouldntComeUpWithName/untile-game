#include <iostream>
#include <algorithm>
#include <UntiledEngine.h>
#include "cubes.h"
#include <map>
// struct A
// {
//     A() 
//     { 
//         UTD_ASSERT(!utd::singleton<A>::instance(), "Instance already exists")
//         std::cout << "A is created" << std::endl; 
//     }
//     ~A() 
//     { 
//         utd::singleton<A>::instance() = nullptr;
//         std::cout << "A is deleted" << std::endl; 
//     }
// };
class Untile : public utd::application
{
public:
    Untile(const utd::cmdline_args& args)
        : application(args)
    {
        push_layer(new cubes());
    }
}; 

int main(int argc, char** argv)
{
    utd::logging::init("Sandbox");
    
    Untile app(utd::cmdline_args{argc, argv});
    
    Untile::instance().run();
 
    return 0;
}



