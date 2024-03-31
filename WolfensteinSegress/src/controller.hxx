#pragma once

#include "model.hxx"
#include "view.hxx"

#include <ge211.hxx>

class Controller : public ge211::Abstract_game
{
public:
    Controller(char* room_file, bool testing);

    void on_key(ge211::Key key) override;

    void on_mouse_move (ge211::Posn<int> mouse) override;

    ge211::Dims<int> initial_window_dimensions() const override;

protected:
    void draw(ge211::Sprite_set& set) override;

    void on_frame(double dt) override;

    void on_mouse_down(ge211::events::Mouse_button butt,
                       ge211::Posn<int> pos) override;

private:
    View view_;
    Model model_;

    int prev_mouse = 0;
};
