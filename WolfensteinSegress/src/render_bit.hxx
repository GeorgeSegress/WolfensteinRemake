#pragma once

#include "ge211.hxx"

using Position = ge211::Posn<float>;

struct render_bit
{
    int image_id;
    float screen_percent;
    float distance;
    float height_offset;
    bool grunt = false;
};