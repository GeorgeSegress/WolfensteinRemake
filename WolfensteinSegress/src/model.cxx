#include "model.hxx"
#include <iostream>

#define RADDEGREE 0.0174533

enum ray_mode
{
    vertical,
    horizontal,
    only_vertical,
    only_horizontal
};

Model::Model(char* room_file, View const& the_view, bool testing)
        :view_(the_view),
        two_dimensional(testing),
        player_pos(0, 0),
        level_name_(room_file),
        room(get_room_file(room_file)),
        score_txt(),
        health_txt(),
        ammo_txt()
{
}

// The function that takes in and reads the room file given on initialization, allowing for accurate level editing
std::vector<char>
Model::get_room_file(char* room_file)
{
    std::vector<char> temp_room;
    std::ifstream room_f;
    try {room_f = ge211::open_resource_file(room_file); }
    catch(ge211::exceptions::File_error)
    {
        return temp_room;
    }
    char room_char;

    int x = 0;
    int y = 0;
    // while nloop that runs through each character in the room file and adds data to a 2D array of positional objects.
    while(room_f.get(room_char))
    {
        if(room_char == 'X')
        {
            walls.push_back(Wall(Position(x, y),32));
        }
        else if(room_char == 'V')
        {
            victory_square = Wall(Position(x, y), 32);
            objs_.push_back(Object(6, Position(x + 16,
                                               y + 16), 0));
        }
        else if(room_char == '2')
        { // Chemical Barrels
            objs_.push_back(Object(2,Position(x + 16,
                                              y + 16), 0));
        }
        else if(room_char == '3')
        { // Columns
            objs_.push_back(Object(3, Position(x + 16,
                                               y + 16), 64));
        }
        else if(room_char == 'A')
        { // ammo pickups
            ammos_.push_back({1, Position(x + 16, y + 16), 28});
        }
        else if(room_char == 'H'){
            healths_.push_back({7, Position(x + 16, y + 16), 28});
        }
        else if(room_char == 'G'){
            grunts_.push_back({Position(x + 16, y + 16)});
        }
        else if(room_char == 'S'){
            scores_.push_back({8, Position(x + 16, y + 16), 28});
        }
        x += 32;
        if(room_char == '\n')
        {
            x = 0;
            y += 32;
        }
        if(room_char == 'P')
        {
            player_pos = Position(x + 32 / 2 - 5, y + 32 / 2 - 5);
            temp_room.push_back('_');
            x += 32;
        } else {
            temp_room.push_back(room_char);
        }
    }
    return temp_room;
}

// moves character based on player inputs and checks for pickups
void
Model::player_move(ge211::Key key)
{
    if (key == ge211::Key::code('w'))
    {
        Position new_pos = Position(player_pos.x + std::cos(player_rot) * 15,
                                    player_pos.y + std::sin(player_rot) * 15);
        if(!have_collision(new_pos).hit)
        {
            player_pos = Position(player_pos.x + std::cos(player_rot) * 9,
                                  player_pos.y + std::sin(player_rot) * 9);
        }
        victory_check();
        ammo_pickup();
        health_pickup();
        score_pickup();
    }
    if(key == ge211::Key::code('s'))
    {
        Position new_pos = Position(player_pos.x - std::cos(player_rot) * 15,
                                    player_pos.y -std::sin(player_rot) * 15);
        if(!have_collision(new_pos).hit)
        {
            player_pos = Position(player_pos.x - std::cos(player_rot) * 9,
                                  player_pos.y - std::sin(player_rot) * 9);
        }
        victory_check();
        ammo_pickup();
        health_pickup();
        score_pickup();
    }
    if(key == ge211::Key::code('a'))
    {
        player_rot -= 0.2;
        if(player_rot < 0)
            player_rot += 2*PI;
    }
    if(key == ge211::Key::code('d'))
    {
        player_rot += 0.2;
        if(player_rot > 2 * PI)
            player_rot -= 2*PI;
    }
    if(key == ge211::Key::code('p'))
    { // for rapid testing damaging system
        gain_damage(13);
    }
}

// moves mouse based on player dragging and converts it to radians
void
Model::mouse_move(int difference)
{
    player_rot += 0.012 * difference;
    if(player_rot > 2 * PI) { player_rot -= 2*PI; }
    else if(player_rot < 0) { player_rot += 2*PI; }
}

// rapidly checks collision between player and each wall object.
struct hit_data
Model::have_collision(Position point) const
{
    hit_data hd = {false, Position(0, 0)};
    for(Wall w : walls)
    {
        hd = w.within(point);
        if(hd.hit) { break; }
    }
    return hd;
}

bool
Model::shoot()
{
    if(ammo_ > 0 && !view_.get_shoot())
    {
        ammo_--;
        get_bits_of_line(player_pos, current_target, 0,
                         0, true);
        for(Grunt& g : grunts_){
            if(!g.get_shootable()) {
                score_ += 100;
                g.shoot_reset();
            }
        }
        return true;
    }else {
        return false;
    }
}

void
Model::grunt_frames(double dt){
    for(Grunt& g : grunts_){
        if(g.get_damage_state()){
            g.received_damage();
            gain_damage(13);
        }
        g.on_frame(dt);
    }
}
// all _pickup functions check collision with each object type to test if player should pickup
void
Model::victory_check()
{
    if(victory_square.within(player_pos).hit)
    {
        game_mode = 3;
    }
}

void
Model::ammo_pickup()
{
    for(Object& ammo : ammos_)
    {
        if(ammo.ammo_pickup(player_pos)){
            ammo_ += 8;
        }
    }
}

void
Model::health_pickup()
{
    for(Object& health : healths_)
    {
        if(health_ < 100 && health.ammo_pickup(player_pos)){
            health_ += 25;
            if(health_ > 100){
                health_ = 100;
            }
        }
    }
}

void
Model::score_pickup(){
    for(Object& score : scores_){
        if(score.ammo_pickup(player_pos)){
            score_ += 1000;
        }
    }
}

void
Model::gain_damage(int damage)
{
    health_ -= damage;
    if(health_ <= 0)
    {
        game_mode = 2;
    }
}

void
Model::game_over_render(ge211::Sprite_set& set)
{
    view_.game_over_render(set, false);
    view_.draw_hud(set, score_, health_, ammo_, score_txt,
                   health_txt, ammo_txt, 4);
}

void
Model::victory_render(ge211::Sprite_set& set)
{
    view_.game_over_render(set, true);
    view_.draw_hud(set, score_, health_, ammo_, score_txt,
                   health_txt, ammo_txt, 5);
}

// repeatedly iterates through the midpoints of a raycasted line, checking for objects at each point to add to the renders_
void
Model::get_bits_of_line(Position p1, Position p2, int recurs,
                        float percentage, bool shot)
{
    Position p3 = {(p1.x + p2.x) / 2, (p1.y + p2.y) / 2};
    if(recurs < 7){
        get_bits_of_line(p1, p3, recurs + 1, percentage, shot);
        get_bits_of_line(p3, p2, recurs + 1, percentage, shot);
    }
    else {
        if(!shot) {
            get_bit_from_point(p3, percentage);
        } else {
            shoot_check(p3);
        }
    }
}

// checking function for each bit of prior function, adding objects to render list if hit
void
Model::get_bit_from_point(Position pos, float percentage)
{
    for(Object& obj : objs_){
        if(obj.is_colliding(pos)){
            renders_.push_back({ obj.get_id(), percentage,
                 obj.get_distance(player_pos), obj.get_offset()});
            break;
        }
    }
    for(Object& obj : ammos_){
        if(obj.is_colliding(pos)){
            renders_.push_back({ obj.get_id(), percentage,
                                 obj.get_distance(player_pos), obj.get_offset()});
            break;
        }
    }
    for(Object& obj : healths_){
        if(obj.is_colliding(pos)){
            renders_.push_back({ obj.get_id(), percentage,
                                 obj.get_distance(player_pos), obj.get_offset()});
            break;
        }
    }
    for(Grunt& grunt : grunts_){
        if(grunt.look(pos)){
            renders_.push_back({ grunt.get_state(), percentage,
                     grunt.get_distance(player_pos), 32, true});
        }
    }
    for(Object& obj : scores_){
        if(obj.is_colliding(pos)){
            renders_.push_back({ obj.get_id(), percentage,
                                 obj.get_distance(player_pos), obj.get_offset()});
            break;
        }
    }
}

void
Model::shoot_check(Position pos)
{
    for(Grunt& grunt : grunts_){
        grunt.shots_fired(pos);
    }
}

// The actual draw function called each frame. Begins by raycasting, then drawing objects and HUD info on top.
void
Model::player_see(ge211::Sprite_set& set)
{
    // sets rotation to be leftmost side of view window
    float rot = player_rot - view_size / 2 * RADDEGREE;
    if(rot < 0) { rot += 2*PI; }
    Position pos = player_pos;

    // Raycast system
    for(int i = 0; i < view_size; i++)
    {
        Position v_pos = Position(0,0); // position of the ray in vertical
        Position h_pos = Position(0,0); // position of the ray in horizontal
        Position v_delta = Position(0, 0); // how much vertical is moved
        Position h_delta = Position(0, 0); // how much horizontal is moved
        float v_dist = 9999; // distance of the vertical ray's hit
        float h_dist = 9999; // distance of the horizontal ray's hit
        float a_tan = -1/tan(rot); // arctangents of rotation (vert)
        float n_tan = -tan(rot); //  negative tangent of rotation (hor)
        ray_mode my_ray = ray_mode::horizontal;

        // creating conditions for vertical and horizontal rays
        // VERTICAL:
        if(rot > PI) // looking up
        {
            v_pos.y = ((int)pos.y >> 5) << 5;
            if(v_pos.y > pos.y) {v_pos.y -= 32;}
            v_pos.x = (pos.y - v_pos.y)*a_tan+pos.x;
            v_delta.y = -32;
            v_delta.x = a_tan * 32;
        }
        else if(rot < PI) // looking down
        {
            v_pos.y = ((int)pos.y >> 5) << 5;
            if(v_pos.y < pos.y) {v_pos.y += 32;}
            v_pos.x = (pos.y - v_pos.y)*a_tan+pos.x;
            v_delta.y = 32;
            v_delta.x = a_tan * -32;
        }

        // HORIZONTAL:
        if (rot > PI2 && rot < PI3) // looking left
        {
            h_pos.x = ((int) pos.x >> 5) << 5;
            if (h_pos.x > pos.x) { h_pos.x -= 32; }
            h_pos.y = (pos.x - h_pos.x) * n_tan + pos.y;
            h_delta.x = -32;
            h_delta.y = n_tan * 32;
        } else if (rot < PI2 || rot > PI3) // looking right
        {
            h_pos.x = ((int) pos.x >> 5) << 5;
            if (h_pos.x < pos.x) { h_pos.x += 32; }
            h_pos.y = (pos.x - h_pos.x) * n_tan + pos.y;
            h_delta.x = 32;
            h_delta.y = -32 * n_tan;
        }

        // alternates horizontal and vertical ray spots until one intersects
        for (int j = 0; j < 32; j++)
        {
            if(my_ray == ray_mode::horizontal ||
               my_ray == ray_mode::only_horizontal)
            {
                if(two_dimensional) {
                    view_.draw_hor_crosshair(set, h_pos);
                }
                hit_data hd = have_collision(h_pos);
                if (my_ray == ray_mode::horizontal)
                {
                    if (hd.hit)
                    {
                        my_ray = ray_mode::only_vertical;
                        h_dist = (h_pos.x - pos.x) * (h_pos.x - pos.x) +
                                 (h_pos.y - pos.y) * (h_pos.y - pos.y);
                    } else
                    {
                        my_ray = ray_mode::vertical;
                        h_pos = Position(h_pos.x + h_delta.x, h_pos.y +
                                                              h_delta.y);
                    }
                } else
                {
                    h_dist = (h_pos.x - pos.x) * (h_pos.x - pos.x) +
                             (h_pos.y - pos.y) * (h_pos.y - pos.y);
                    if (hd.hit || h_dist >= v_dist)
                    {
                        break;
                    }
                    h_pos = Position(h_pos.x + h_delta.x, h_pos.y + h_delta.y);
                }
            }
            else
            {
                if(two_dimensional) {
                    view_.draw_ver_crosshair(set, v_pos);
                }
                hit_data hd = have_collision(v_pos);
                if (my_ray == ray_mode::vertical)
                {
                    if (hd.hit)
                    {
                        my_ray = ray_mode::only_horizontal;
                        v_dist = (v_pos.x - pos.x) * (v_pos.x - pos.x) +
                                 (v_pos.y - pos.y) * (v_pos.y - pos.y);
                    } else
                    {
                        my_ray = ray_mode::horizontal;
                        v_pos = Position(v_pos.x + v_delta.x, v_pos.y +
                                                              v_delta.y);
                    }
                } else
                {
                    v_dist = (v_pos.x - pos.x) * (v_pos.x - pos.x) +
                             (v_pos.y - pos.y) * (v_pos.y - pos.y);
                    if (hd.hit || v_dist >= h_dist)
                    {
                        break;
                    }
                    v_pos = Position(v_pos.x + v_delta.x, v_pos.y + v_delta.y);
                }
            }
        }

        // determines whether vertical is the correct ray or not
        if(h_dist > v_dist)
        {
            if(two_dimensional) {
                view_.draw_ver_spot(set, v_pos);
            }
            else
            {
                get_bits_of_line(player_pos, v_pos, 0,
                                 (double)i / (double)view_size, false);

                float beta = rot - player_rot;
                if(beta > 2*PI) { beta -= 2*PI; }
                else if(beta < 0) { beta += 2*PI; }
                v_dist = sqrt(v_dist) * cos(beta);
                view_.draw_wall(set, (double)i / (double)view_size,
                          false,v_dist);
            }
            if(i == view_size / 2){
                current_target = v_pos;
            }
        }
        else
        {
            if(two_dimensional) {
                view_.draw_hor_spot(set, h_pos);

            }
            else
            {
                get_bits_of_line(player_pos, h_pos, 0,
                                 (double)i / (double)view_size, false);

                float beta = rot - player_rot;
                if(beta > 2*PI) { beta -= 2*PI; }
                else if(beta < 0) { beta += 2*PI; }
                h_dist = sqrt(h_dist) * cos(beta);
                view_.draw_wall(set, (double)i / (double)view_size,
                          true,h_dist);
            }
            if(i == view_size / 2){
                current_target = h_pos;
            }
        }

        // updates rotation to the next degree
        rot += RADDEGREE;
        if(rot < 0) { rot += 2*PI; }
        else if(rot > 2*PI) { rot -= 2*PI; }
    }
    // Draws the additional information on top of that.
    if(two_dimensional){
        view_.two_d_draw(set, room, player_pos);
    }
    else {
        // renders objects given from the prior calls to getbitsfromlines function
        view_.draw_objects(renders_, set);
        // relays player data to the views function to draw HUG with updated data
        view_.draw_hud(set, score_, health_, ammo_, score_txt,
                       health_txt, ammo_txt, 0);
    }

    // Resets objects at the end of frame for proper rendering next frame
    for(Object& obj : objs_){
        obj.reset();
    }
    for(Object& obj : ammos_){
        obj.reset();
    }
    for(Object& obj : healths_){
        obj.reset();
    }
    for(Grunt& g : grunts_){
        g.reset();
    }
    for(Object& obj : scores_){
        obj.reset();
    }
    renders_.clear();
}