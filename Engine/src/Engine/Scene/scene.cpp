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
    
    //m_player = create_entity("player");

    for(int i = 0; i< 100; i++)
    {
        for(int j = 0; j < 100; j++)
        {
            auto e = m_registry.create();
            m_registry.emplace<transform>(e, transform{ {j * 1.1f, i * 1.1f, 0.f} });
            m_registry.emplace<sprite>(e, m_texture_manager.get("grass"));
        }
    }

}

uecs::entity utd::scene::create_entity(const std::string& name)
{
    auto entity = m_registry.create();
    
    m_registry.emplace<transform>(entity, transform{});
    m_registry.emplace<sprite>(entity, sprite{});
    
    m_names[entity] = name;

    return entity;
}

void utd::scene::on_event(event &)
{

}

void utd::scene::on_update(float dt)
{
    static float scale_accumulator = 0.1f;
    for (auto entity : m_registry)
    {
        auto& transform = m_registry.get<utd::transform>(entity);

        if (entity % 2 == 0)
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
    for(auto entity : m_registry)
    {
        UTD_PROFILE_BEGIN("Component Query");
        auto [transform, sprite] = m_registry.query<utd::transform, utd::sprite>(entity);
         
        utd::renderer2d::draw(transform, sprite);
        
        UTD_PROFILE_END();
    }

}
