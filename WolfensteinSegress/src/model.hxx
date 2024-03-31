#pragma once

#define PI 3.14159265359
#define PI2 1.57079632679
#define PI3 4.71238898038
#include <ge211.hxx>
#include <ge211_sprites.hxx>
#include "wall.hxx"
#include "object.hxx"
#include "hit_data.hxx"
#include "render_bit.hxx"
#include "view.hxx"
#include "grunt.hxx"

using Position = ge211::Posn<float>;



class Model
{
public:
    explicit Model(char* room_file, View const& the_view, bool testing);

    std::vector<char> get_rooms() const {return room;}
    int get_game_mode() const { return game_mode; }

    // Player Functions
    void player_move(ge211::Key key);
    void mouse_move(int difference);
    void gain_damage(int damage);
    void ammo_pickup();
    void health_pickup();
    void score_pickup();
    void gain_score(int add) { score_ += add; }
    void victory_check();
    bool shoot();
    void grunt_frames(double dt);

    // Raycasting Functions
    void player_see(ge211::Sprite_set& set);
    struct hit_data have_collision(Position point) const;
    void game_over_render(ge211::Sprite_set& set);
    void victory_render(ge211::Sprite_set& set);

    friend struct Test_Access;

private:
    View const& view_;
    // Initial / overarching variables
    int game_mode = 1; // 1 = level, 2 = gameover, 3 = victory
    Wall victory_square = Wall(Position(0, 0), 32);
    int view_size = 60;
    bool two_dimensional;

    // Active variables
    Position player_pos;
    Position current_target = {0, 0};
    float player_rot = PI3;
    int ammo_ = 8;
    int health_ = 100;
    int score_ = 0;

    // Room functions & Members
    std::vector<char> get_room_file(char* room_file);
    std::string level_name_;
    std::vector<Wall> walls = std::vector<Wall>();
    std::vector<Object> objs_ = std::vector<Object>();
    std::vector<Object> ammos_ = std::vector<Object>();
    std::vector<Object> healths_ = std::vector<Object>();
    std::vector<Object> scores_ = std::vector<Object>();
    std::vector<Grunt> grunts_ = std::vector<Grunt>();
    std::vector<char> room;

    // Rendering Related Functions & Members
    void get_bits_of_line(Position p1, Position p2, int recurs, float
    percentage, bool shot);
    void get_bit_from_point(Position pos, float percentage);
    void shoot_check(Position pos);
    std::vector<render_bit> renders_ = std::vector<render_bit>();
    ge211::Text_sprite score_txt;
    ge211::Text_sprite health_txt;
    ge211::Text_sprite ammo_txt;
};