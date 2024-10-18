#pragma once

#include <string>
#include <unordered_map>
#include <Engine/Utils/memory.h>
#include <Engine/Core/Assert.h>
#include <Engine/Profiling/Profile.h>

namespace utd
{
    using handle_id = size_t;

    template<typename t_resource, typename t_uid = std::string>
    class resource_manager
    {
    public:
        using resource_type = t_resource;
        using id_type = t_uid;
        using resource_map = std::unordered_map<t_uid, std::uptr<t_resource>>;
    public:
        template<typename... t_args>
        ref_ptr<t_resource> fetch(const t_uid& identifier, t_args&&... args);
        
        template<typename... t_params>
        bool load(const t_uid& identifier, t_params&&... args);
        
        template<typename... t_args>
        bool create(const t_uid& identifier, t_args&&... args);
        
        t_resource& get(const t_uid& identifier);
       
        ref_ptr<t_resource> find(const t_uid& identifier);
        
        void remove(const t_uid& identifier);
        void clear() { m_map.clear(); }

        ref_ptr<t_resource> reload(const t_uid& identifier)
        {
            UTD_PROFILE_FUNC();
            auto& resource = m_map[identifier];
            UTD_ENGINE_INFO("resource id {0}", resource->get_id());
            resource = t_resource::load(resource->path());
            UTD_ENGINE_INFO("resource id {0}", resource->get_id());

            return resource;
        }

        bytes_t memory_usage() { return m_memory_usage.load(); }

        resource_map::iterator begin() { return m_map.begin(); }
        resource_map::iterator end() { return m_map.end(); }

        resource_map::const_iterator begin() const { return m_map.cbegin(); }
        resource_map::const_iterator end() const { return m_map.cend(); }
        
    private:
    private:
        std::atomic<bytes_t> m_memory_usage;
        resource_map m_map;
    };

    template<typename t_resource, typename t_uid>
    template<typename ...t_args>
    inline ref_ptr<t_resource> resource_manager<t_resource, t_uid>::fetch(const t_uid& identifier, t_args && ...args)
    {
        auto it = m_map.find(identifier);
        if (it == m_map.end())
        {
            auto resource = m_map.emplace(std::make_pair(identifier, t_resource::load(std::forward<t_args>(args)...)));
            return *(resource.first->second.get()); // WTF?
        }

        return *(it->second.get());
    }

    template<typename t_resource, typename t_uid>
    template<typename... t_params>
    inline bool resource_manager<t_resource, t_uid>::load(const t_uid &identifier, t_params &&...args)
    {
        auto it = m_map.find(identifier);
        if(it == m_map.end())
        {
            m_map.emplace(std::make_pair(identifier, t_resource::load(std::forward<t_params>(args)...)));
            return true;
        }
        
        return false;
    }
    
    template<typename t_resource, typename t_uid>
    inline t_resource& resource_manager<t_resource, t_uid>::get(const t_uid &identifier)
    {
        auto it = m_map.find(identifier);
        
        UTD_ENGINE_ASSERT(it != m_map.end(), "can\'t aquire a resource");
        
        return *(it->second.get());
    }

    template <typename t_resource, typename t_uid>
    inline void resource_manager<t_resource, t_uid>::remove(const t_uid &identifier)
    {
        m_map.erase(identifier);
    }
    
    // default resource managers alias for convenience
    using texture_pack   = resource_manager<class texture, std::string>;
    using shader_library = resource_manager<class shader, std::string>;

}