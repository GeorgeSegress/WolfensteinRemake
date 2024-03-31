#pragma once

#include <ge211_sprites.hxx>
#include "ge211_window.hxx"
#include "render_bit.hxx"

using Position = ge211::Posn<float>;
using text = ge211::Text_sprite;

class View
{
public:
    explicit View();

    // 3 functions for rendering 3D gamestate (works in this order):
    void draw_wall(ge211::Sprite_set& set, double screen_percent, bool hor,
                   double dist) const;
    void draw_objects(std::vector<render_bit> objs, ge211::Sprite_set& set)
            const;
    void draw_hud(ge211::Sprite_set& set, int score, int health, int ammo,
          text& score_txt, text& health_txt, text& ammo_txt, int hud) const;

    // Misc rendering thingies (it's 2 am and im blasting vangelis)
    void game_over_render(ge211::Sprite_set& set, bool won) const;
    void on_frame(double dt);
    void shoot() { shooting = true; }
    bool get_shoot() const { return shooting; }

    // 2D (debugging) draw functions
    void two_d_draw(ge211::Sprite_set& set, std::vector<char> rooms,
                    Position player_pos) const;
    void draw_hor_crosshair(ge211::Sprite_set& set, Position h_pos) const;
    void draw_ver_crosshair(ge211::Sprite_set& set, Position v_pos) const;
    void draw_ver_spot(ge211::Sprite_set& set, Position player_pos) const;
    void draw_hor_spot(ge211::Sprite_set& set, Position player_pos) const;

private:
    int center_screen_;
    int screen_max_;

    std::vector<ge211::Image_sprite> objects_ =
            std::vector<ge211::Image_sprite>();
    std::vector<ge211::Image_sprite> gun_ =
            std::vector<ge211::Image_sprite>();
    std::vector<ge211::Image_sprite> grunt_ =
            std::vector<ge211::Image_sprite>();
    int cur_gun = 0;
    bool shooting = false;
    double frame_time = 0;
    ge211::Rectangle_sprite game_over_screen;
    ge211::Rectangle_sprite victory_screen;
    ge211::Transform hud_trans_;
    int hud_top_;

    ge211::Text_sprite menu_text;
    ge211::Text_sprite score_text;
    ge211::Font wolfenstein_font;

    ge211::Circle_sprite const player_sprite;
    ge211::Circle_sprite const grunt_sprite;
    ge211::Rectangle_sprite const wall_sprite;
    ge211::Circle_sprite const crosshairs;
    ge211::Circle_sprite const hor_crosshair;
    ge211::Rectangle_sprite const horizontal_wall;
    ge211::Rectangle_sprite const vertical_wall;
};
