#pragma once

#include <ge211.hxx>

using Position = ge211::Posn<float>;

class Object
{
public:
    Object(int num, Position pos, float offset);

    int get_id() const { return my_image; }
    Position get_pos() const { return pos_; }
    float get_offset() const { return height_offset; }
    float get_distance(Position player_pos) const;
    bool ammo_pickup(Position pos);

    bool is_colliding(Position pos);
    void reset();

    bool dead = false; // refers to whether an ammo pickup is "live"

private:
    int my_image;
    Position pos_;
    bool interactable_ = true; // Refers to whether an object can be seen
    float height_offset;

    int left_;
    int right_;
    int top_;
    int bottom_;
};