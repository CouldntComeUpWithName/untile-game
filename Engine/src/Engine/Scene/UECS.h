#pragma once
#include <limits>

#include <Engine/Core/Base.h>
#include <Engine/Core/Assert.h>
#include <Engine/Profiling/profile.h>

namespace utd::ecs
{
    using entity = u64;
    
    constexpr entity ENULL = std::numeric_limits<entity>::max();
    
    constexpr auto MAX_COMPONENTS = 64ULL;
    constexpr auto MAX_ENTITIES = 100'000ULL;
    
    template<typename Entity>
    class sparse_set
    {
    public:
        static constexpr auto PAGE_SIZE = 32'768U;
        static constexpr auto ENTITIES_PER_PAGE = PAGE_SIZE / sizeof(Entity);

        static_assert(utd::is_pow_of_two(ENTITIES_PER_PAGE));

    public:
        using entity_type = Entity;
        using page_type   = std::uptr<Entity[]>;
        using size_type   = size_t;
    
    public:
        class iterator
        {
            friend class sparse_set<Entity>;
        public:
            using value_type = Entity;
            using pointer = const value_type*;
            using reference = const value_type&;
            using iterator_category = std::random_access_iterator_tag;
        public:
            iterator& operator ++() noexcept
            {
                return --m_index, *this;
            }
            
            iterator operator ++(int)
            {
                iterator original = *this;
                return operator++(), original;
            }

            iterator& operator --() 
            {
                return ++m_index, *this; 
            }

            iterator operator --(int) 
            {
                iterator original = *this;
                return operator--(), original;
            }
            
            bool operator ==(const iterator& rhs) const
            {
                return m_index == rhs->index;
            }
            
            bool operator <(const iterator& rhs) const
            {
                return m_index < rhs->index;
            }

            bool operator >(const iterator& rhs) const
            {
                return m_index > rhs->index;
            }

            bool operator !=(const iterator& rhs) const
            {
                return m_index != rhs->index;
            }

            bool operator <=(const iterator& rhs) const
            {
                return m_index <= rhs->index;
            }

            bool operator >=(const iterator& rhs) const
            {
                return m_index >= rhs->index;
            }

            pointer operator ->()
            {
                const auto pos = m_index - 1;
                return &(*m_packed)[pos];
            }

            reference operator *()
            {
                return *operator->();
            }

        private:
            using packed_type = std::vector<entity_type>;
            using index_type = i64;

            iterator(const packed_type& packed, const index_type index)
                : m_packed{&packed}, m_index{index}
            { }
        
        private:
            packed_type* m_packed;
            index_type m_index;
        };

    public:
        sparse_set()             = default;
        sparse_set(sparse_set&&) = default;
        virtual ~sparse_set()    = default;

        bool contains(const entity_type entity) const
        {
            const auto current_page = page(entity);

            return (current_page < m_sparse.size() && m_sparse[current_page] && m_sparse[current_page][offset(entity)] != ENULL);
        }

        size_type page(const entity_type entity) const
        {
            return entity / ENTITIES_PER_PAGE;
        }

        size_type offset(const entity_type entity) const
        {
            return entity & (ENTITIES_PER_PAGE - 1); // 3.5-7% faster than '%' operator
        }
        size_type index(entity_type entity)
        {
            UTD_ASSERT(contains(entity));
            return m_sparse[page(entity)][offset(entity)];
        }
        size_type capacity() const
        {
            return m_pool.capacity();
        }
        
        size_type extended() const 
        {
            return m_sparse.size() * ENTITIES_PER_PAGE; 
        }

        bool empty() const
        {
            return m_pool.empty();
        }

        iterator begin() { return iterator{m_pool, {}}; }
        iterator end() { return iterator{m_pool, m_pool->size() }; }

        void emplace(const entity_type entity)
        {
            UTD_ASSERT(!contains(entity));

            auto& valid_page = secure(page(entity));
            valid_page[offset(entity)] = static_cast<entity_type>(m_pool.size());
            
            m_pool.push_back(entity);
        }

        void clear()
        {
            m_sparse.clear();
            m_pool.clear();
        }

    private:
        page_type& secure(const size_type page_position)
        {
            if(page_position >= m_sparse.size())
            {
                m_sparse.resize(page_position + 1);
            }

            if(!m_sparse[page_position])
            {
                m_sparse[page_position] = std::make_unique<entity_type[]>(ENTITIES_PER_PAGE);

                for(auto* begin = m_sparse[page_position].get(), *end = begin + ENTITIES_PER_PAGE; begin != end; begin++)
                    *begin = ENULL;
            }

            return m_sparse[page_position];
        }

    private:
        std::vector<page_type> m_sparse;
        std::vector<entity_type> m_pool;
    };

    template<typename Entity, typename Component>
    class component_storage : public sparse_set<Entity>
    {
    public:
        using component_type  = Component;
        using entity_type     = Entity;
        using underlying_type = sparse_set<Entity>;

    public:
        component_type& get(const entity_type entity)
        {
            return m_instances[underlying_type::index(entity)];
        }
        
        template<typename... Args>
        void emplace(const entity_type entity, Args&&... args)
        {
            if constexpr(std::is_aggregate_v<component_type>)
            {
                m_instances.push_back(component_type{std::forward<Args>(args)...});
            }
            else
            {
                m_instances.emplace_back(std::forward<Args>(args)...);
            }

            underlying_type::emplace(entity);
        }

    private:
        std::vector<component_type> m_instances;
    };

#if 0 // TODO: implement
    template <typename Entity, typename... Components>
    class group
    {
    public:
        using entity_type = Entity;
        using size_type = size_t;
        using iterator = typename sparse_set<Entity>::iterator;

    public:
        
        std::tuple<> get(entity_type entity)
        {
            return {};
        };
        
        size_type size() { return m_handler->size(); }
        
        template<typename Component>
        size_type size() { return std::get<pool_type<Component>*>(m_group)->size(); }

        size_type capacity() { return m_handler->capacity(); }
        
        iterator begin(){ return m_handler->begin(); }
        iterator end(){ return m_handler->end(); }

    private:
        template<typename Component>
        using pool_type = component_storage<Entity, Component>;
        
        group(sparse_set<Entity>& handler, component_storage<Entity, Components>&... pools)
            : m_handler{&handler}, m_group{&pools}
        {}

    private:
        sparse_set<entity_type>* m_handler;
        std::tuple<pool_type<Components>*...> m_group;
    };
#endif

    struct incrementor
    {   
        template<typename Component>
        friend struct storage_index;

    private:
        static size_t next()
        {
            static size_t counter = 0;
            return counter++;
        }
    };

    template<typename Component>
    struct storage_index
    {
        static auto get()
        {
            static const auto value = incrementor::next();
            return value;
        }
    };

    template<typename Entity>
    class basic_registry
    {
    public:
        using pool_data = std::uptr<sparse_set<Entity>>;
        using entity_type = Entity;
        
    public:
        basic_registry() = default;
    public:
        entity_type create()
        {
            return m_entities.emplace_back(m_entities.size());
        }
        // temp
        auto begin() {return m_entities.begin(); }
        auto end() {return m_entities.end(); } 

        size_t size() { return m_entities.size(); }

        template<typename Component, typename... Args>
        void emplace(entity_type entity, Args&&... args)
        {
            secure<Component>().emplace(entity, std::forward<Args>(args)...);
        }
        
        template<typename Component>
        Component& get(entity_type entity)
        {
            return secure<Component>().get(entity);
        }

        template<typename... Component>
        decltype(auto) query(entity_type entity)
        {
            static_assert(sizeof...(Component) > 0);
            
            //UTD_PROFILE_FUNC();
            UTD_ASSERT(has<Component...>(entity));
            
            return std::tuple<Component&...>{ secure<Component>().get(entity)...};
        }

        bool valid(entity_type entity) const
        {
            return entity < m_entities.size() && entity == m_entities[entity];
        }

        template<typename... Component>
        bool has(const entity_type entity) const
        {
            UTD_ASSERT(valid(entity), "An Invalid entity");

            return (secure<Component>().contains(entity) && ...);
        }

        void clear()
        {
            m_entities.clear();
            
            for(auto& component_pool : m_data)
            {
                component_pool->clear();
            }
        }

    private:
        template<typename Component>
        component_storage<Entity, Component>& secure() const
        {
            //UTD_PROFILE_FUNC();

            const auto index = storage_index<Component>::get();

            if(index >= m_data.size())
                m_data.resize(index + 1);

            if(!m_data[index])
                m_data[index] = std::make_unique<component_storage<Entity, Component>>();

            return static_cast<component_storage<Entity, Component>&>(*m_data[index]);
        }

    private:
        mutable std::vector<pool_data> m_data;
        std::vector<entity_type> m_entities;
        entity_type m_destroyed = ENULL;
    };

    using registry = basic_registry<entity>;

} /* namespace utd::ecs */

namespace uecs = utd::ecs;