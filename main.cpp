#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include "vec.hpp"
#include "image.hpp"



int main() {
    using namespace std;

    image_demo();

    Image image(360, 360, { 1, 1, 1 });
    Vec2 a = {180, 180}, b = { 100, 0 }, c = { 0, 100 };
    //image.render_triangle(a, b, c, { 1, 0, 0 });

    image.set(180, 180, { 0, 0, 1 });

    cout << is_point_inside_triangle(a, b, c, {45, 55});


    unique_ptr<ImageGenerator> generator = make_unique<BMPImageGenerator>("triangulos.bmp");
    generator->generate(image);
}
