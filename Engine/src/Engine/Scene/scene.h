#pragma once


namespace utd
{
    class scene
    {
    public:
        scene() = default;

        bool active();
        bool active(bool);
        
        void _start();

        void pause();
        void resume();
        
        bool is_paused();
        bool is_running();

        virtual void on_update();
        virtual void on_render();
        virtual void on_event();
    private:

    private:
        bool m_active;
        bool m_paused;
        bool m_running;

        std::unordered_map<int, ref_ptr<entity>> m_entity_map;
    };

}