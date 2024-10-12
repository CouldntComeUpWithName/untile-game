#pragma once

#include <Engine/Core/Base.h>
#include <Engine/Scene/UECS.h>
#include <Engine/Core/resource_manager.h>

#include <unordered_map>

namespace utd
{
    class scene
    {
    public:
        scene() = default;
        
        void active(bool);
        void pause();
        void resume();
        
        bool is_active();
        bool is_paused();
        bool is_running();
        
        void build_scene();
        ecs::entity create_entity(const std::string& name);
        
        virtual void on_event(event&);
        virtual void on_update(float);
        virtual void on_render();
    private:
        void _start(); // not sure...
    private:
        bool m_active;
        bool m_paused;
        bool m_running;
        
        ecs::entity m_player;
        ecs::registry m_registry;
        std::unordered_map<ecs::entity, std::string> m_names;

        texture_pack m_texture_manager;

    };

}