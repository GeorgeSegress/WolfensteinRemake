#include "wall.hxx"
#include "hit_data.hxx"

Wall::Wall(Position center, float width)
        :center_(center),
         right_(center.x + width),
         left_(center.x),
         top_(center.y),
         bottom_(center.y + width)
{}

hit_data
Wall::within(Position point) const
{
    hit_data hd = {(point.x <= right_ &&
                    point.x >= left_) && (point.y <= bottom_ && point.y >= top_),
                   point };
    return hd;
}