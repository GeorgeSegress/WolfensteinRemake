#pragma once

#include <ge211.hxx>

using Position = ge211::Posn<float>;

class Grunt
{
public:
    Grunt(Position pos);

    void on_frame(double dt);
    bool look(Position pos);
    void reset() { interactable_ = true; }
    void shoot_reset() { shootable_ = true; }
    int get_state() { return cur_state; }
    float get_distance(Position pos);
    void shots_fired(Position pos);
    bool get_damage_state() { return damaging; }
    bool get_shootable() { return shootable_; }
    void received_damage() { damaging = false; }

private:
    int cur_state = 0;
    Position cur_pos;
    int left_;
    int right_;
    int top_;
    int bottom_;
    bool inside(Position pos);

    bool interactable_ = true;
    bool shootable_ = true;
    bool alive_ = false;
    bool damaging = false;
    float cur_frame = 0;

    int shots_left = 2;
};