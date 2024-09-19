#pragma once

namespace utd::utils
{
    template<typename t_enum, typename t_return>
    t_return enum_cast(t_enum en)
    {
        return static_cast<t_return>(en);
    }

}