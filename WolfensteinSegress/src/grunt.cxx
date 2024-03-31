#include "grunt.hxx"
#include <iostream>
#include <controller.hxx>

// Grunt Initialization
Grunt::Grunt(Position pos)
        :cur_pos(pos),
        left_(pos.x - 16),
        right_(pos.x + 16),
        top_(pos.y - 16),
        bottom_(pos.y + 16)
{
}

// Displays itself if seen and turns on animation system
bool
Grunt::look(Position pos)
{
    if(interactable_ && inside(pos)) {
        alive_ = true;
        interactable_ = false;
        return true;
    }
    return false;
}

// returns distance for quick rendering
float
Grunt::get_distance(Position pos)
{
    return sqrt((cur_pos.x - pos.x) * (cur_pos.x - pos.x) +
                (cur_pos.y - pos.y) * (cur_pos.y - pos.y));
}

// Simultaneous animation and AI system for enemies
void
Grunt::on_frame(double dt)
{
    switch(cur_state)
    {
    case 0:
        if(alive_){
            cur_frame += dt;
            if(cur_frame >= 2){
                cur_frame = 0;
                cur_state = 1;
            }
        }
        break;
    case 1:
    case 2:
        cur_frame += dt;
        if(cur_frame >= 0.15){
            cur_frame = 0;
            cur_state++;
        }
        break;
    case 3:
        cur_frame += dt;
        if(cur_frame >= 0.15){
            damaging = true;
            cur_state = 0;
            cur_frame = 0;
        }
        break;
    case 4:
        cur_frame += dt;
        if(cur_frame >= 0.2){
            cur_frame = 0;
            cur_state = 0;
        }
        break;
    case 5:
    case 6:
    case 7:
    case 8:
        cur_frame += dt;
        if(cur_frame >= 0.1){
            cur_frame = 0;
            cur_state++;
        }
    }
}

// shoots gun if available and updates animator.
void
Grunt::shots_fired(Position pos)
{
    if(inside(pos) && cur_state < 5 && shootable_){
        shootable_ = false;
        shots_left -= 1;
        cur_frame = 0;
        if(shots_left == 0){
            cur_state = 5;
        } else{
            cur_state = 4;
        }
    }
}

bool
Grunt::inside(Position pos){
    return (pos.x > left_ && pos.x < right_) &&
           (pos.y > top_ && pos.y < bottom_);
}