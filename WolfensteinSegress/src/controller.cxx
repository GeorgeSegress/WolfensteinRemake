#include "controller.hxx"

Controller::Controller(char* room_file, bool testing)
        : view_(),
        model_(room_file, view_, testing)
{

}

ge211::Dims<int>
Controller::initial_window_dimensions() const
{
    return ge211::Window::max_fullscreen_dimensions();
}

void
Controller::draw(ge211::Sprite_set& set)
{
    if(model_.get_game_mode() == 1) {
        get_window().set_fullscreen(true);
        model_.player_see(set);
    } else if(model_.get_game_mode() == 3){
        model_.victory_render(set);
    } else {
        model_.game_over_render(set);
    }
}

void
Controller::on_key(ge211::Key key)
{
    if(model_.get_game_mode() == 1) {
        model_.player_move(key);
    }
}

void
Controller::on_mouse_move(ge211::Posn<int> mouse)
{
    if(model_.get_game_mode() == 1) {
        model_.mouse_move(mouse.x - prev_mouse);
        prev_mouse = mouse.x;
    }
}

void
Controller::on_frame(double dt)
{
    view_.on_frame(dt);
    model_.grunt_frames(dt);
}

void
Controller::on_mouse_down(ge211::events::Mouse_button butt,
        ge211::Posn<int> pos) {
    if(butt == ge211::events::Mouse_button::left) {
        if(model_.shoot()) {
            view_.shoot();
        }
    }
}