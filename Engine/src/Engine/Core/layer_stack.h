#pragma once

#include <Engine/Core/Base.h>
#include <Engine/Core/Layer.h>

#include <vector>

namespace utd
{
    class layer_stack
    {
        using iterator               = std::vector<layer*>::iterator;
        using const_iterator         = std::vector<layer*>::const_iterator; 
        using reverse_iterator       = std::vector<layer*>::reverse_iterator;
        using const_reverse_iterator = std::vector<layer*>::const_reverse_iterator;
    public:
        layer_stack() = default;
        
        ~layer_stack()
        {
            for (auto* layer : m_stack)
                delete layer;
        }

        void push(layer*);
        void pop(layer*);
        
        void push_overlay(layer*);
        void pop_overlay(layer*);

        iterator begin() { return m_stack.begin(); }
        iterator end()   { return m_stack.end(); }

        reverse_iterator rbegin(){ return m_stack.rbegin(); }
        reverse_iterator rend()  { return m_stack.rend(); }

        const_iterator begin() const { return m_stack.cbegin(); }
        const_iterator end()   const { return m_stack.cend(); }
        
        const_reverse_iterator rbegin() const { return m_stack.crbegin(); }
        const_reverse_iterator rend()   const { return m_stack.crend(); }

    private:
        std::vector<layer*> m_stack;
        u32 m_layer_insert_idx = 0;
    };

} /* namespace utd */