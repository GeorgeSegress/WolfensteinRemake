#include "object.hxx"
#include <iostream>

Object::Object(int num, Position pos, float offset)
        :my_image(num),
         pos_(pos),
         height_offset(offset),
         left_(pos.x - 10),
         right_(pos.x + 10),
         top_(pos.y - 10),
         bottom_(pos.y + 10)
{

}

bool
Object::is_colliding(Position pos)
{
    if(interactable_ && (pos.x > left_ && pos.x < right_) &&
       (pos.y > top_ && pos.y < bottom_) && !dead) {
        interactable_ = 0;
        return true;
    }
    return false;
}

float
Object::get_distance(Position player_pos) const
{
    return sqrt((pos_.x - player_pos.x) * (pos_.x - player_pos.x) +
                (pos_.y - player_pos.y) * (pos_.y - player_pos.y));
}

bool
Object::ammo_pickup(Position pos)
{
    if((pos.x > left_ && pos.x < right_) &&
       (pos.y > top_ && pos.y < bottom_) && !dead){
        dead = true;
        return true;
    }
    return false;
}

void
Object::reset()
{
    interactable_ = true;
}