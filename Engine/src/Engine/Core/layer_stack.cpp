#include <upch.h>
#include "layer_stack.h"

void utd::layer_stack::push(layer* layer)
{
    m_stack.emplace(m_stack.begin() + m_layer_insert_idx, layer);
    m_layer_insert_idx++;
}

void utd::layer_stack::pop(layer* layer)
{
    auto it = std::find(m_stack.begin(), m_stack.begin() + m_layer_insert_idx, layer);
    
    if(it != m_stack.begin() + m_layer_insert_idx)
    {
        layer->on_detach();
        m_stack.erase(it);
        m_layer_insert_idx--;
    }
}

void utd::layer_stack::push_overlay(layer* overlay)
{
    m_stack.emplace_back(overlay);
}

void utd::layer_stack::pop_overlay(layer* overlay)
{
    auto it = std::find(m_stack.begin() + m_layer_insert_idx, m_stack.end(), overlay);

    if(it != m_stack.end())
    {
        overlay->on_detach();
        m_stack.erase(it);
    }
}
