#include <iostream>
#include <algorithm>
#include <UntiledEngine.h>

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
        : utd::application(args)
    {}
}; 

int main(int argc, char** argv)
{
    utd::logging::init("Sandbox");
    
    // std::cout << &utd::singleton<utd::application>::instance() << std::endl;
    // std::cout <<  &utd::application::instance() << std::endl;
    // std::cout << &utd::application::singleton<utd::application>::instance() << std::endl;
    
    // A* a0 = new A();
    // A* a1 = new A();
    // A* a2 = new A();
    // utd::triangle();
    //Triangle();

    Untile app(utd::cmdline_args{argc, argv});
    // // utd::application app1(utd::cmdline_args{argc, argv});
    
    Untile::instance().run();
    
    // // utd::application app1(utd::cmdline_args{argc, argv});
    // app.run();

    // std::cout << "after instantioation\n";
    // std::cout << &utd::singleton<utd::application>::instance() << std::endl;
    // std::cout <<  &utd::application::instance() << std::endl;
    // std::cout << &utd::application::singleton<utd::application>::instance() << std::endl;

    // //UTD_ENGINE_INFO("Hello", "World");
    // std::cin.get();
    // utd::application app;
    // app.run();

    return 0;
}



