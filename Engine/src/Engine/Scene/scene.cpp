#include <upch.h>
#include "scene.h"

#include <Engine/Scene/UECS.h>
#include <Engine/Scene/components.h>
#include <Engine/Graphics/renderer2d.h>

void utd::scene::build_scene()
{
    m_texture_manager.fetch("sand", "E:/Programming/untile/Sandbox/assets/textures/sand_3.png");
    m_texture_manager.fetch("grass", "E:/Programming/untile/Sandbox/assets/textures/grass_2.png");
    m_texture_manager.fetch("cobblestone", "E:/Programming/untile/Sandbox/assets/textures/cobblestone_1.png");
    m_texture_manager.fetch("red sand", "C:/Users/Star Maker/Downloads/red-sand-ground-256x256.png");
    
    //m_player = create_entity("player");

    for(int i = 0; i< 317; i++)
    {
        for(int j = 0; j < 317; j++)
        {
            auto e = m_registry.create();
            m_registry.emplace<transform>(e, transform{ {j * 1.1f, i * 1.1f, 0.f} });
            m_registry.emplace<sprite>(e, m_texture_manager.get("red sand"));
        }
    }

}

uecs::entity utd::scene::create_entity(const std::string& name)
{
    auto entity = m_registry.create();
    
    m_registry.emplace<transform>(entity, transform{});
    m_registry.emplace<sprite>(entity, sprite{});
    
    //m_names[entity] = name;

    return (uecs::entity)entity;
}

void utd::scene::on_event(event &)
{

}

void utd::scene::on_update(float dt)
{
    static float scale_accumulator = 0.1f;
   
    auto view = m_registry.view<utd::transform, utd::sprite>();
    
    //auto* data = m_registry.get<utd::transform>(entity);

    for (entt::entity entity : view)
    {
        auto& transform = view.get<utd::transform>(entity);

        if ((size_t)entity % 2 == 0)
        {
            transform.rotation.x += 0.2f * dt;
            transform.rotation.y -= 0.2f * dt;
        }
        else 
        {
            transform.rotation.x -= 0.2f * dt;
            transform.rotation.y += 0.2f * dt;
            transform.rotation.z += 0.2f * dt;
        }
    }
}

void utd::scene::on_render()
{
    auto view = m_registry.view<utd::transform, utd::sprite>();
    
    for(auto entity : view)
    {
        //UTD_PROFILE_BEGIN("Component Query");
        auto [transform, sprite] = view.get<utd::transform, utd::sprite>(entity);
         
        utd::renderer2d::draw(transform, sprite);
        
        //UTD_PROFILE_END();
    }
}
