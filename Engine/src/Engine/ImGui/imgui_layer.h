#pragma once

#include <Engine/Core/Base.h>
#include <Engine/Core/layer.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <Engine/Graphics/shader.h>
#include <Engine/Graphics/vertex_array.h>

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

    class triangle_layer : public layer
    {
    public:
        triangle_layer() = default;
        triangle_layer(const std::string& name)
            : layer(name)
        { }

        virtual void on_attach();
        virtual void on_detach() {}
        virtual void on_render();

        virtual void on_update(float);

        virtual void on_event(event&) {}
        virtual ~triangle_layer() = default;
    private:
        glm::vec4 color;
        std::uptr<shader> m_shader;
        bool visible = true;

        vertex_array m_vertex_array;
    };

} /* namespace utd */