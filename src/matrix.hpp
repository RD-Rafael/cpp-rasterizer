#include "vec.hpp"
#include <cassert>

class Matrix3 {
    float data[3][3]; 

    float get(int i, int j) const {
        return data[i][j];
    }

    void set(int i, int j, float x) {
        data[i][j] = x;
    }

    Vec3 get_row(int i) const {
        assert(i >= 0 && i < 3);
    }

    Vec3 get_column(int i) const {
        assert(i >= 0 && i < 3);
    }

    Matrix3& operator+=(const Matrix3& other) {
        for (int i=0; i<3; i++)
            for (int j=0; j<3; j++)
                data[i][j] += other.data[i][j];
    }

    // friend Matrix3 operator+(const Matrix3& other) {
    //     for (int i=0; i<3; i++)
    //         for (int j=0; j<3; j++)
    //             data[i][j] += other.data[i][j];
    // }

    Matrix3& operator-=(const Matrix3& other) {
        for (int i=0; i<3; i++)
            for (int j=0; j<3; j++)
                data[i][j] -= other.data[i][j];
    }
};
