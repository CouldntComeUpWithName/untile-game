#pragma once

#include <Engine/Core/base.h>
#include <Engine/Core/layer.h>

#include<glm/glm.hpp>

namespace utd
{   


    class imgui_layer : public layer
    {
    public:
        imgui_layer() = default;
        imgui_layer(const std::string& name);

        virtual void on_attach() override;
        virtual void on_detach() override;
        virtual void on_update(float) override {};
        virtual void on_event(event&) override;
        ~imgui_layer() override = default;

        void begin();
        void end();
    };


    class demo : public layer
    {
    public:
        void on_render() override;
        glm::vec4& get_color()
        {
            static glm::vec4 color;

            return color;
        }
        
    };

} /* namespace utd */