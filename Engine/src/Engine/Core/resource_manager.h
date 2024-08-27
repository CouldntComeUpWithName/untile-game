#pragma once

#include <string>
#include <unordered_map>
#include <Engine/Utils/memory.h>


namespace utd
{


    template<typename t_resource, typename t_uid = std::string>
    class resource_manager
    {
    public:
        using resource_map = std::unordered_map<t_uid, std::uptr<t_resource>>;
    public:
        template<typename... t_args>
        bool fetch(const t_uid& identifier, t_args&&... args);

        template<typename... t_load_paths>
        bool load(const t_uid& identifier, t_load_paths&&... args);
        
        template<typename... t_args>
        bool create(const t_uid& identifier, t_args&&... args);
        
        t_resource& get(const t_uid& identifier);
       
        ref_ptr<t_resource> find(const t_uid& identifier);
        
        resource_map::iterator begin();
        resource_map::iterator end();

        resource_map::const_iterator begin() const;
        resource_map::const_iterator end() const;
        
    private:
    private:
    };

    template<typename t_uid = std::string>
    using texture_pack = resource_manager<class texture, t_uid>;
    
    template<typename t_uid = std::string>
    using shader_library = resource_manager<class shader, t_uid>;
}