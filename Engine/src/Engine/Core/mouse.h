#pragma once
#include "Base.h"

namespace utd
{
    struct mouse : utility
    {
        using code = u16;
        enum : code
        {
            BUTTON0 = 0,
            BUTTON1 = 1,
            BUTTON2 = 2,
            BUTTON3 = 3,
            BUTTON4 = 4,
            BUTTON5 = 5,
            BUTTON6 = 6,
            BUTTON7 = 7,

            LAST_BUTTON   = BUTTON7,
            LEFT_BUTTON   = BUTTON0,
            RIGHT_BUTTON  = BUTTON1,
            MIDDLE_BUTTON = BUTTON2
        };
    };

}