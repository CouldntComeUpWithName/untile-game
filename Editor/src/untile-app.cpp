#include <UntiledEngine.h>
#include "Editor-Layer.h"

class untile_app : public utd::application
{
public:
    untile_app(const utd::cmdline_args& args)
        : utd::application(args)
    {
        push_overlay(new utd::editor_layer());
    }
};

int main(int argc, char** argv)
{
    utd::logging::init("Untile-App");
    
    untile_app app({argc, argv});
    app.run();
}