#include "DjGlove.h"

DjGlove::DjGlove()
:   punched(false),
    button_push_0(false),
    button_push_1(false),
    button_push_2(false),
    button_push_3(false),
    button_push_4(false),
    button_touch_0(false),
    button_touch_1(false),
    button_touch_2(false),
    button_touch_3(false),
    button_flip(false),
    poti_0(0),
    poti_1(0),
    poti_2(0),
    distance(0),
    orientation_x(0),
    orientation_y(0),
    orientation_z(0),
    flex(0),
    channel(0),
    program(0)
{
}

DjGlove::~DjGlove()
{
}

