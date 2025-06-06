#include "vec.hpp"
#include <math.h>

bool is_point_inside_triangle(Vec2 a, Vec2 b, Vec2 c, Vec2 point){
    bool s1, s2, s3;


    std::cout << dot((a-b).right_vec(), point-b) << '\n';
    std::cout << dot((b-c).right_vec(), point-c) << '\n';
    std::cout << dot((c-a).right_vec(), point-a) << '\n';
    s1 = dot((a-b).right_vec(), point-b) > 0;
    s2 = dot((b-c).right_vec(), point-c) > 0;
    s3 = dot((c-a).right_vec(), point-a) > 0;
    return s1 == s2 && s2 == s3;

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
