#include "view.hxx"
#include "hit_data.hxx"
#include <iostream>
#include "ge211_window.hxx"

#define RADDEGREE 0.0174533

// Convenient type aliases:
using Color = ge211::Color;
using Sprite_set = ge211::Sprite_set;
using Position = ge211::Posn<float>;

static Color const wall_col {171, 171, 171};
static Color const vert_wall_col {110, 110, 110};
static Color const player_col {213, 114, 117};
static Color const grunt_col {114, 144, 230};
static Color const stark_white {255, 255, 255};
static Color const blood_red {255, 50, 50};
static Color const cool_green {50, 255, 50};
static int view_size = 60;



View::View()
        : center_screen_(ge211::Window::max_fullscreen_dimensions().height / 2),
          screen_max_(ge211::Window::max_fullscreen_dimensions().width),
          game_over_screen(ge211::Window::max_fullscreen_dimensions(),
                           blood_red),
          victory_screen(ge211::Window::max_fullscreen_dimensions(),
                           cool_green),
          hud_trans_(ge211::Transform{}.scale((double)screen_max_ / (double)300)),
          hud_top_(center_screen_ * 2 - (screen_max_ / 300) * 35),
          menu_text(),
          score_text(),
          wolfenstein_font("wolfenstein.ttf", 23.0 * (screen_max_ / 300)),
          player_sprite(5, player_col),
          grunt_sprite(5, grunt_col),
          wall_sprite({32, 32}, wall_col),
          crosshairs(2, grunt_col),
          hor_crosshair(2, player_col),
          horizontal_wall({screen_max_ / view_size + 1, 1}, wall_col),
          vertical_wall({screen_max_ / view_size + 1, 1}, vert_wall_col)
{
    // Loads all image files into data memory, including player and grunt animations from spritesheets
    objects_.push_back(ge211::Image_sprite("hud_image.png"));
    objects_.push_back(ge211::Image_sprite("ammo.png"));
    objects_.push_back(ge211::Image_sprite("chem_barrel.png"));
    objects_.push_back(ge211::Image_sprite("column.png"));
    objects_.push_back(ge211::Image_sprite("dead_hud_image.png"));
    objects_.push_back(ge211::Image_sprite("win_hud_image.png"));
    objects_.push_back(ge211::Image_sprite("victory.png"));
    objects_.push_back(ge211::Image_sprite("health_pack.png"));
    objects_.push_back(ge211::Image_sprite("score_crown.png"));

    gun_.push_back(ge211::Image_sprite("gun/gun_1.png"));
    gun_.push_back(ge211::Image_sprite("gun/gun_2.png"));
    gun_.push_back(ge211::Image_sprite("gun/gun_3.png"));
    gun_.push_back(ge211::Image_sprite("gun/gun_4.png"));
    gun_.push_back(ge211::Image_sprite("gun/gun_5.png"));

    grunt_.push_back(ge211::Image_sprite("grunt/grunt_0.png"));
    grunt_.push_back(ge211::Image_sprite("grunt/grunt_1.png"));
    grunt_.push_back(ge211::Image_sprite("grunt/grunt_2.png"));
    grunt_.push_back(ge211::Image_sprite("grunt/grunt_3.png"));
    grunt_.push_back(ge211::Image_sprite("grunt/grunt_4.png"));
    grunt_.push_back(ge211::Image_sprite("grunt/grunt_5.png"));
    grunt_.push_back(ge211::Image_sprite("grunt/grunt_6.png"));
    grunt_.push_back(ge211::Image_sprite("grunt/grunt_7.png"));
    grunt_.push_back(ge211::Image_sprite("grunt/grunt_8.png"));
    grunt_.push_back(ge211::Image_sprite("grunt/grunt_9.png"));
}

void
View::draw_ver_crosshair(ge211::Sprite_set& set, Position v_pos) const
{
    set.add_sprite(crosshairs, ge211::Posn<int>
            (v_pos.x - 2, v_pos.y - 2), 5);
}

void
View::draw_hor_crosshair(ge211::Sprite_set& set, Position h_pos) const
{
    set.add_sprite(hor_crosshair, ge211::Posn<int>
            (h_pos.x - 2, h_pos.y - 2), 5);
}

void
View::draw_ver_spot(ge211::Sprite_set& set, Position player_pos) const
{
    set.add_sprite(player_sprite, ge211::Posn<int>
            (player_pos.x - 5, player_pos.y - 5), 5);
}

void
View::draw_hor_spot(ge211::Sprite_set& set, Position player_pos) const
{
    set.add_sprite(grunt_sprite, ge211::Posn<int>
            (player_pos.x - 5, player_pos.y - 5), 5);
}

void
View::draw_objects(std::vector<render_bit> objs, ge211::Sprite_set& set)
const
{
    for(render_bit obj : objs)
    {
        float scale = 400.0 / obj.distance;
        int x = (int)((double)obj.screen_percent * screen_max_);
        int y = center_screen_ - (int)(obj.height_offset * scale);
        ge211::Transform t = ge211::Transform{}.set_scale(scale);
        if(!obj.grunt) {
            set.add_sprite(objects_[obj.image_id], {x, y},
                           50, t);
        } else {
            set.add_sprite(grunt_[obj.image_id], {x, y},
                           50, t);
        }
    }
}

void
View::draw_wall(ge211::Sprite_set& set, double screen_percent, bool hor,
                double dist) const
{
    float height = 16384 / dist;
    if(height > center_screen_ * 2){
        height = center_screen_ * 2;
    }
    int y_pos = center_screen_ - height / 2;
    ge211::Transform new_trans = ge211::Transform{}.scale_y(height);
    set.add_sprite(hor ? horizontal_wall : vertical_wall,ge211::Posn<int>
    {(int)(screen_max_ * screen_percent),y_pos}, 0, new_trans);
}

void
View::draw_hud(ge211::Sprite_set& set, int score, int health, int ammo,
       text& score_txt, text& health_txt, text& ammo_txt, int hud) const
{
    // Draws gun if gamemode is still active.
    if(hud == 0)
    {
        ge211::geometry::Transform t = ge211::geometry::Transform{}.scale(11);
        set.add_sprite(gun_[cur_gun], {screen_max_ / 2 - 64 * 6,
                                       0},75, t);
    }
    // Draws hud and calculates the top value of the text
    int text_top = hud_top_ + (int)((double)7 * hud_trans_.get_scale_x());
    set.add_sprite(objects_[hud], {0, hud_top_},
                   98, hud_trans_);
    // Gets score to have correct # of 0's and then prints
    text::Builder score_builder(wolfenstein_font);
    for(int i = 0; i < 6; i++)
    {
        if((score / pow(10,i)) <= 1){
            score_builder << "0";
        }
    }
    score_builder.color(stark_white) << score;
    score_txt.reconfigure(score_builder);
    set.add_sprite(score_txt, {(int)((double)42 * hud_trans_.get_scale_x()),
                                text_top}, 99);
    // Same for health
    text::Builder health_builder(wolfenstein_font);
    health_builder << (health < 100 ? "0" : "") << (health < 10 ? "0" : "");
    health_builder << (health > 0 ? health : 0);
    health_txt.reconfigure(health_builder);
    set.add_sprite(health_txt, {(int)((double)161 * hud_trans_.get_scale_x()),
                                text_top}, 99);
    // then ammo
    text::Builder ammo_builder(wolfenstein_font);
    ammo_builder << (ammo < 100 ? "0" : "") << (ammo < 10 ? "0" : "");
    ammo_builder << ammo;
    ammo_txt.reconfigure(ammo_builder);
    set.add_sprite(ammo_txt, {(int)((double)203 * hud_trans_.get_scale_x()),
                                text_top}, 99);
}

// additional debugging render mode that renders the 2D view of level and game functions
void
View::two_d_draw(Sprite_set& set, std::vector<char> rooms,
                 Position player_pos) const
{
    int x = 0;
    int y = 0;
    // Runs a switch case statement on each character to render field
    for(char c : rooms)
    {
        switch(c)
        {
        case 'X': // Wall
            set.add_sprite(wall_sprite,
                           ge211::Posn<int>(x, y), 0);
            x += 32;
            break;
        case '_': // Empty Space
            x += 32;
            break;
        case 'G': // Enemy grunt
            set.add_sprite(grunt_sprite,ge211::Posn<int>
                    (x + 32 / 2 - 5, y + 32 / 2 - 5), 1);
            x += 32;
            break;
        case '\n': // End of line
            y += 32;
            x = 0;
            break;
        default:
            x += 32;
            break;
        }
    }

    set.add_sprite(player_sprite,ge211::Posn<int>
            (player_pos.x - 5, player_pos.y - 5), 1);

}

void
View::game_over_render(ge211::Sprite_set& set, bool won) const
{
    set.add_sprite(won ? victory_screen : game_over_screen,
                   {0, 0}, 0);
}

void
View::on_frame(double dt)
{
    if(shooting)
    {
        frame_time += dt;
        if(frame_time >= 0.1){
            frame_time = 0;
            if(cur_gun == 4){
                cur_gun = 0;
                shooting = false;
            } else {
                cur_gun++;
            }
        }
    }
}