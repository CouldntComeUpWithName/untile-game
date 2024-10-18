#include <UntiledEngine.h>
#include "Editor-Layer.h"

class untiled_app : public utd::application
{
public:
    untiled_app(const utd::cmdline_args& args)
        : utd::application(args)
    {
        UTD_INFO("Creating an app");
        push_overlay(new utd::editor_layer());
    }
};

int main(int argc, char** argv)
{
    utd::logging::init("Untile-App");
    
    untiled_app app({argc, argv});
    app.run();
}