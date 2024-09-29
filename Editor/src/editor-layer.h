#pragma once
#include <Engine/Core/Base.h>
#include <Engine/Core/layer.h>
#include <Engine/Core/resource_manager.h>
#include <Engine/Graphics/camera.h>
#include <Engine/Graphics/Texture.h>
#include <Engine/Graphics/Framebuffer.h>
#include<Engine/Graphics/atlas.h>

namespace utd
{
    class editor_layer : public layer
    {
    public:
        void on_attach() override;
        void on_event(event& event) override;
        void on_update(float dt) override;
        void on_render() override;
        void on_detach() override;
    private:
        void _on_imgui_render();
    private:
        utd::ref_ptr<utd::texture> m_cobblestone;
        utd::ref_ptr<utd::texture> m_grass;
        utd::ref_ptr<utd::texture> m_sand;
        
        utd::editor_camera m_editor_camera;

        std::uptr<utd::framebuffer> m_framebuffer;
        
        texture_pack m_texture_manager;

        bool m_viewport_hovered = false;
        bool m_viewport_focused = false;
        
        //temp 
        utd::atlas m_character_atlas;
    };
}
