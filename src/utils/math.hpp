#ifndef MATH_HPP_
#define MATH_HPP_

#include "assert.h"

struct vec2;
struct vec3;
struct vec4;
struct mat4;
struct quat;

void print(vec2 v);
void print(vec3 v);
void print(vec4 v);
void print(mat4 m);
void print(quat q);

// Define the
struct vec2 {
    float x, y;
    vec2(float _x, float _y) {
        x = _x;
        y = _y;
    }
    vec2(const vec2& v) {
        x = v.x;
        y = v.y;
    }
    vec2(vec3 v);
    vec2(vec4 v);
};

struct vec3 {
    float x, y, z;
    vec3() { }
    vec3(float _x, float _y, float _z) {
        x = _x;
        y = _y;
        z = _z;
    }
    vec3(const vec2& v, float _z) {
        x = v.x;
        y = v.y;
        z = _z;
    }
    vec3(const vec3& v) {
        x = v.x;
        y = v.y;
        z = v.z;
    }
    vec3(vec4 v);

    vec3 operator+(vec3 rhs) {
        vec3 vc;
        vc.x = x + rhs.x;
        vc.y = y + rhs.y;
        vc.z = z + rhs.z;
        return vc;
    }
    vec3& operator+=(vec3 rhs) {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }
    vec3 operator-(vec3 rhs) {
        vec3 vc;
        vc.x = x - rhs.x;
        vc.y = y - rhs.y;
        vc.z = z - rhs.z;
        return vc;
    }
    vec3& operator-=(vec3 rhs) {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }
    vec3 operator*(vec3 rhs) {
        vec3 vc;
        vc.x = x * rhs.x;
        vc.y = y * rhs.y;
        vc.z = z * rhs.z;
        return vc;
    }
    vec3& operator*=(vec3 rhs) {
        x *= rhs.x;
        y *= rhs.y;
        z *= rhs.z;
        return *this;
    }
    vec3 operator/(vec3 rhs) {
        vec3 vc;
        vc.x = x / rhs.x;
        vc.y = y / rhs.y;
        vc.z = z / rhs.z;
        return vc;
    }
    vec3& operator/=(vec3 rhs) {
        x /= rhs.x;
        y /= rhs.y;
        z /= rhs.z;
        return *this;
    }

    vec3 operator+(float rhs) {
        vec3 vc;
        vc.x = x + rhs;
        vc.y = y + rhs;
        vc.z = z + rhs;
        return vc;
    }
    vec3& operator+=(float rhs) {
        x += rhs;
        y += rhs;
        z += rhs;
        return *this;
    }
    vec3 operator-(float rhs) {
        vec3 vc;
        vc.x = x - rhs;
        vc.y = y - rhs;
        vc.z = z - rhs;
        return vc;
    }
    vec3& operator-=(float rhs) {
        x -= rhs;
        y -= rhs;
        z -= rhs;
        return *this;
    }
    vec3 operator*(float rhs) {
        vec3 vc;
        vc.x = x * rhs;
        vc.y = y * rhs;
        vc.z = z * rhs;
        return vc;
    }
    vec3& operator*=(float rhs) {
        x *= rhs;
        y *= rhs;
        z *= rhs;
        return *this;
    }
    vec3 operator/(float rhs) {
        vec3 vc;
        vc.x = x / rhs;
        vc.y = y / rhs;
        vc.z = z / rhs;
        return vc;
    }
    vec3& operator/=(float rhs) {
        x /= rhs;
        y /= rhs;
        z /= rhs;
        return *this;
    }
};

struct vec4 {
    float x, y, z, w;
    vec4(float _x, float _y, float _z, float _w) {
        x = _x;
        y = _y;
        z = _z;
        w = _w;
    }
    vec4(vec2 v, float _z, float _w) {
        x = v.x;
        y = v.y;
        z = _z;
        w = _w;
    }
    vec4(vec3 v, float _w) {
        x = v.x;
        y = v.y;
        z = v.z;
        w = _w;
    }
    vec4(vec4& v) {
        x = v.x;
        y = v.y;
        z = v.z;
        w = v.w;
    }
};

// This is stored like this
// 00 03 06
// 01 04 07
// 02 05 08
struct mat3 {
    float m[9];
    float getElement(int row, int col) {
        assert(row >= 0 && row <= 2 && col >= 0 && col <= 2);
        return m[col * 3 + row];
    }
    void setElement(int row, int col, float value) {
        assert(row >= 0 && row <= 2 && col >= 0 && col <= 2);
        m[col * 3 + row] = value;
    }
};

// This is stored like this
// 00 04 08 12
// 01 05 09 13
// 02 06 10 14
// 03 07 11 15
struct mat4 {
    float m[16];
    float getElement(int row, int col) {
        assert(row >= 0 && row <= 3 && col >= 0 && col <= 3);
        return m[col * 4 + row];
    }
    void setElement(int row, int col, float value) {
        assert(row >= 0 && row <= 3 && col >= 0 && col <= 3);
        m[col * 4 + row] = value;
    }
};

#endif