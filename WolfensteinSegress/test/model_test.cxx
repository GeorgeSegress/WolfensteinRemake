#include "model.hxx"
#include <catch.hxx>
#include "view.hxx"
#include <iostream>

/*struct Test_Access
{
    Model& model;

    explicit Test_Access(Model&);
    std::vector<char> calculate_from_file(char* room_file);
};*/

TEST_CASE("example test (TODO: replace this)")
{
    Position player_pos = Position(193, 54);
    Position obj = Position(368, 144);
    float rot = atan((player_pos.y - obj.y) /
                       (player_pos.x - obj.x));
    std::cout << rot << '\n';
    if(player_pos.x > obj.x && player_pos.y < obj.y)
    {
        rot = PI - rot;
    } else if(player_pos.x > obj.x &&
              player_pos.y > obj.y){
        rot = PI + rot;
    } else if(player_pos.x < obj.x &&
              player_pos.y > obj.y){
        rot = 2*PI - rot;
    }
    std::cout << rot;
}

/*TEST_CASE("Victory game state change")
{

}

TEST_CASE("Correct file reading")
{
    char c[] = "testing_room";
    Model model(c, View(), false);
    Test_Access access(model);
    char t[] = "testing_room";
    std::vector<char> chars = access.calculate_from_file(t);
    int walls = 0;
    int objects = 0;
    for(char cs : chars){
        if(cs == 'X'){
            walls++;
        } else if(cs != '_'){
            objects++;
        }
    }
    CHECK(walls == 64);
    CHECK(objects == 13);
}

TEST_CASE("Player's correct spawn")
{

}

Test_Access::Test_Access(Model& model_)
        :model(model_)
{
}

std::vector<char>
Test_Access::calculate_from_file(char *room_file)
{
    std::vector<char> chars = model.get_room_file(room_file);
    return chars;
}*/