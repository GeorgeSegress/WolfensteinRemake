#pragma once
#include <ge211.hxx>
#include "hit_data.hxx"

using Position = ge211::Posn<float>;

class Wall
{
public:
    explicit Wall(Position center, float width);

    hit_data within(Position point) const;

private:
    Position center_;

    float right_;
    float left_;
    float top_;
    float bottom_;
};