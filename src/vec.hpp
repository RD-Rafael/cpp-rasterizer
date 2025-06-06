#ifndef H_UTILS
#define H_UTILS

#include <string>
#include <sstream>
#include <cmath>

struct Vec2;
struct Vec3;

float dot(const Vec2 a, const Vec2 b);
float dot(const Vec3 a, const Vec3 b);

Vec2 normalize(Vec2 v);
Vec3 normalize(Vec3 v);

float distance(const Vec2& a, const Vec2& b);
float distance(const Vec3& a, const Vec3& b);

float cosTheta(const Vec2& a, const Vec2& b);
float cosTheta(const Vec3& a, const Vec3& b);

struct Vec2 {
    float x, y;

    Vec2& operator+=(const Vec2& other) {
        x += other.x;
        y += other.y;
        return *this;
    }
    
    friend Vec2 operator+(const Vec2 a, const Vec2& b) {
        return {a.x + b.x, a.y + b.y};
    }

    Vec2& operator-=(Vec2 & other){
        x -= other.x;
        y -= other.y;
        return *this;
    }

    friend Vec2 operator-(const Vec2 a, const Vec2& b) {
        return {a.x - b.x, a.y - b.y};
    }

    Vec2& operator*=(float k){
        x *= k;
        y *= k;
        return *this;
    }

    friend Vec2 operator*(const Vec2& a, float k){
        return {a.x*k, a.y*k};
    }
    
    Vec2& operator/=(float k){
        x /= k;
        y /= k;
        return *this;
    }

    friend Vec2 operator/(const Vec2& a, float k){
        return {a.x/k, a.y/k};
    }
    
    std::string str() {
        std::stringstream ss;
        ss << "Vec2(" << x << ", " << y << ")";
        return ss.str();
    }
    
    float length() const {
        return std::sqrt(x*x + y*y); 
    }
    
    Vec2 right_vec() const {
        return { x, -y };
    }

    Vec2 left_vec() const {
        return { -x, y };
    }
};

struct Vec3 {
    float x, y, z;
    
    Vec3& operator+=(Vec3 &other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }
    
    friend Vec3 operator+(const Vec3& a, const Vec3 &b) {
        return { a.x + b.x, a.y + b.y, a.z + b.z };
    }
    
    Vec3& operator-=(Vec3 &other) {
        x -= other.x;   
        y -= other.y;
        z -= other.z;
        return *this;
    }
    
    friend Vec3 operator-(const Vec3& a, const Vec3 &b) {
        return { a.x - b.x, a.y - b.y, a.z - b.z };
    }
    
    Vec3& operator*=(float k){
        x *= k;
        y *= k;
        z *= k;
        return *this;
    }
    
    friend Vec3 operator*(const Vec3& a, float k){
        return {a.x*k, a.y*k, a.z*k};
    }
    
    Vec3& operator/=(float k){
        x /= k;
        y /= k;
        z /= k;
        return *this;
    }
    
    friend Vec3 operator/(const Vec3& a, float k){
        return {a.x/k, a.y/k, a.z/k};
    }

    operator Vec2() const {
        return { x, y };
    }

    std::string str() {
        std::stringstream ss;
        ss << "Vec3(" << x << ", " << y << ", " << z << ")";
        return ss.str();
    }
    
    float length() const {
        return std::sqrt(x*x + y*y + z*z);
    }
};

float dot(const Vec2 a, const Vec2 b) { return a.x * b.x + a.y * b.y; }
float dot(const Vec3 a, const Vec3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }

Vec2 normalize(Vec2 v) { return v / v.length(); }
Vec3 normalize(Vec3 v) { return v / v.length(); }

float distance(const Vec2& a, const Vec2& b) { return (b-a).length(); }
float distance(const Vec3& a, const Vec3& b) { return (b-a).length(); }

float cosTheta(const Vec2& a, const Vec2& b) { return dot(a,b)/(a.length()*b.length()); }
float cosTheta(const Vec3& a, const Vec3& b) { return dot(a,b)/(a.length()*b.length()); }

#endif
