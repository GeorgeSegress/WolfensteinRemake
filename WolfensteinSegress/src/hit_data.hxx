#pragma once

#include <ge211.hxx>

// returns the hit position for gun firing
struct hit_data
{
    bool hit;
    ge211::Posn<float> hit_pos;
};