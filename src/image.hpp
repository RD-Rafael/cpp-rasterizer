#ifndef H_IMAGE
#define H_IMAGE

#include "vec.hpp"
#include "triangle.hpp"

#include <cstdlib>
#include <cassert>
#include <string>
#include <fstream>

typedef Vec3 Color;

class Image {
private:
    Color* data;
public:
    const int width, height;

    Image(int width, int height)
    : width(width), height(height)
    {
        data = (Color*)malloc(sizeof(Color) * width * height);
    }

    Image(int width, int height, Color background_color)
    : Image(width, height)
    {
        for (int i=0; i<width*height; i++)
            data[i] = background_color;
    }

    ~Image() {
        free(data);
    }

    Color get(int x, int y) const {
        assert((x >= 0 && x < width) && "x is out of bounds.");
        assert((y >= 0 && y < height) && "y is out of bounds.");
        return data[x + y * width];
    }

    void set(int x, int y, Color color) {
        assert((x >= 0 && x < width) && "x is out of bounds.");
        assert((y >= 0 && y < height) && "y is out of bounds.");
        data[x + y * width] = color;
    }

    void render_line(Vec2 a, Vec2 b, Color color){
        int x0 = static_cast<int>(a.x);
        int y0 = static_cast<int>(a.y);
        int x1 = static_cast<int>(b.x);
        int y1 = static_cast<int>(b.y);

        int dx = std::abs(x1 - x0);
        int dy = std::abs(y1 - y0);
        int sx = (x0 < x1) ? 1 : -1;
        int sy = (y0 < y1) ? 1 : -1;
        int err = dx - dy;

        while (true) {
            if (x0 >= 0 && x0 < width && y0 >= 0 && y0 < height)
                set(x0, y0, color);
            if (x0 == x1 && y0 == y1) break;
            int e2 = 2 * err;
            if (e2 > -dy) { err -= dy; x0 += sx; }
            if (e2 < dx)  { err += dx; y0 += sy; }
        }
    }

    void render_triangle(Vec2 a, Vec2 b, Vec2 c, Color color) {
        BoundingBox bounding_box = bounding_box.triangle_bounds(a, b, c);
        
        int min_x = std::max((int) bounding_box.top_left.x, 0),
            max_x = std::min((int) bounding_box.bottom_right.x, this->width),
            min_y = std::max((int) bounding_box.top_left.y, 0),
            max_y = std::min((int) bounding_box.bottom_right.y, this->height);
        
        for(int x = min_x; x < max_x; x++){
            set(x, max_y, color);
            set(x, min_y, color);
        }
        for(int y = min_y; y < max_y; y++){
            set(max_x, y, color);
            set(min_x, y, color);
        }

        for(int x = min_x; x < max_x; x++){
            for(int y = min_y; y < max_y; y++) {
                Vec2 point = { (float)x, (float)y };
                if (is_point_inside_triangle(a, b, c, point)) {
                    set(x, y, color);
                }
            }
        }


        for(int x = 0; x < width; x++){
            for(int y = 0; y < height; y++) {
                Vec2 point = { (float)x, (float)y };
                if (is_point_inside_triangle(a, b, c, point)) {
                    set(x, y, color);
                }
            }
        }
    }

    struct Iterator {   
        using iterator_category = std::forward_iterator_tag;
        using difference_type  = std::ptrdiff_t;
        using value_type       = Color;
        using pointer          = Color*;
        using reference        = Color&;

        Iterator(pointer ptr) : m_ptr(ptr) {}

        reference operator*() const { return *m_ptr; }
        pointer operator->() { return m_ptr; }
        Iterator& operator++() { m_ptr++; return *this; }
        Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }

        friend bool operator==(const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; };
        friend bool operator!=(const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; };
    private:
        pointer m_ptr;
    };

    Iterator begin() { return Iterator(&data[0]); }
    Iterator end() { return Iterator(&data[width * height]); }
};


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

class ImageGenerator {
public:
    virtual bool generate(Image&) = 0;
};

class BMPImageGenerator : public ImageGenerator {
private:
    std::fstream file;

    public:
    BMPImageGenerator(std::string file_name) {
        file.open(file_name, std::ios::out | std::ios::binary);
    }
    
    ~BMPImageGenerator() {
        if (file.is_open()) file.close();
    }

    bool generate(Image& image) override {
        if (!file.is_open()) return false;

        unsigned int image_size = image.width * image.height * 4;
        unsigned int file_size = 14 + 40 + image_size;
        unsigned int data_offset = 14 + 40;
        unsigned int dib_header_size = 40;
        unsigned short color_planes = 1;
        unsigned short bits_per_pixel = 32;
        unsigned int compression = 0;
        unsigned int print_resolution = 0;

        file.put('B');
        file.put('M');
        file.write(reinterpret_cast<const char*>(&file_size), 4);
        file.write(reinterpret_cast<const char*>(&print_resolution), 2);
        file.write(reinterpret_cast<const char*>(&print_resolution), 2);
        file.write(reinterpret_cast<const char*>(&data_offset), 4);
        file.write(reinterpret_cast<const char*>(&dib_header_size), 4);
        file.write(reinterpret_cast<const char*>(&image.width), 4);
        file.write(reinterpret_cast<const char*>(&image.height), 4);
        file.write(reinterpret_cast<const char*>(&color_planes), 2);
        file.write(reinterpret_cast<const char*>(&bits_per_pixel), 2);
        file.write(reinterpret_cast<const char*>(&compression), 4);
        file.write(reinterpret_cast<const char*>(&image_size), 4);
        file.write(reinterpret_cast<const char*>(&print_resolution), 4);
        file.write(reinterpret_cast<const char*>(&print_resolution), 4);
        file.write(reinterpret_cast<const char*>(&print_resolution), 4);
        file.write(reinterpret_cast<const char*>(&print_resolution), 4);

        for(int y = image.height-1; y >= 0; y--){
            for(int x = 0; x < image.width; x++){
                Color color = image.get(x, y);
                file.put((unsigned char)std::ceil(color.x * 255));
                file.put((unsigned char)std::ceil(color.y * 255));
                file.put((unsigned char)std::ceil(color.z * 255));
                file.put((unsigned char)0);
            }
        }

        file.close();
        return true;
    }
};

void image_demo() {
    #include <memory>
    using namespace std;

    const int width = 64;
    const int height = 64;

    Image image(width, height);
    for (int x=0; x<width; x++) {
        for (int y=0; y<height; y++) {
            float r = min(x / (width - 1.f), 1.f);
            float g = min(y / (height - 1.f), 1.f);
            image.set(x, y, { r, g, 0 });
        }
    }

    unique_ptr<ImageGenerator> generator = make_unique<BMPImageGenerator>("demo.bmp");
    generator->generate(image);
}

#endif
