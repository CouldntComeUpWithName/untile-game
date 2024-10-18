#pragma once

#include <Engine/Core/Base.h>
#include <Engine/Core/layer.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <Engine/Graphics/shader.h>
#include <Engine/Graphics/vertex_array.h>

constexpr inline auto UTD_IMGUI_ENABLE = 1;

#define UTD_IMGUI_SCOPE() if constexpr(UTD_IMGUI_ENABLE)
    
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

        void begin()const;
        void end()const;
    private:
    };

} /* namespace utd */