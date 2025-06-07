#include "vec.hpp"
#include <math.h>

bool is_point_inside_triangle(Vec2 a, Vec2 b, Vec2 c, Vec2 point){
    bool s1, s2, s3;
    s1 = (a - b).is_on_right_side(point - b);
    s2 = (b - c).is_on_right_side(point - c);
    s3 = (c - a).is_on_right_side(point - a);

    return (s1 == s2) && (s2 == s3);
}

struct BoundingBox {
    Vec2 top_left;
    Vec2 bottom_right;

    static BoundingBox triangle_bounds(Vec2 a, Vec2 b, Vec2 c){
        return {
            { std::min(std::min(a.x, b.x), c.x), std::min(std::min(a.y, b.y), c.y) },
            { std::max(std::max(a.x, b.x), c.x), std::max(std::max(a.y, b.y), c.y) }
        };
    }
};
