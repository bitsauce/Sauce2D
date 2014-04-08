#ifndef MATH_VECTOR_H
#define MATH_VECTOR_H
 
#include "x2d/platform.h"

/** \brief A class which holds a 2 dimentional vector
 *
 * The Vector2 class is a 2 dimentional (hence the 2) vector.
 * It stores the x and y value as floating-point values.
 *
 */

void RegisterVectors();

class Vector2i;

class X2DAPI Vector2
{
public:
	// Constructor
    Vector2(float xy);
    Vector2(float x, float y);
    Vector2(const Vector2i &v);
 
	// Setting
	void set(const float x, const float y);

	// Transform
    void rotate(const float angle);
    float angle() const;
	float angle(const Vector2& v2) const;

	// Vector functions
    void normalize();
    Vector2 normalized() const;
    float dot(const Vector2& v2) const;
    float cross(const Vector2& v2) const;
    float distance(const Vector2& v2) const;
	float magnitude() const;
 
	// Operators
    Vector2& operator=(const Vector2& v2);
    Vector2& operator+=(const Vector2& v2);
    Vector2& operator-=(const Vector2& v2);
    Vector2& operator*=(const float scalar);
    Vector2& operator/=(const float scalar);
    const Vector2 operator+(const Vector2 &v2) const;
    const Vector2 operator-(const Vector2 &v2) const;
    const Vector2 operator*(const float scalar) const;
    const Vector2 operator/(const float scalar) const;
    bool operator==(const Vector2& v2) const;

public:

	// AngelScript
	static void DefaultConstructor(Vector2 *self);
	static void CopyConstructor1(const Vector2 &other, Vector2 *self);
	static void CopyConstructor2(const Vector2i &other, Vector2 *self);
	static void InitConstructor1(float xy, Vector2 *self);
	static void InitConstructor2(float x, float y, Vector2 *self);
 
public:
    float x, y;
	
	// Type id
	static int TypeId;
};

#define TUPLE_CMP(a, b) \
	if(a < b) return true; \
	if(a > b) return false;

#define TUPLE_CMP2(a, b) \
	if(a < b) return false; \
	if(a > b) return true;

class X2DAPI Vector2i
{
public:
	// Constructor
    Vector2i(int xy);
    Vector2i(int x, int y);
    Vector2i(const Vector2 &v);
 
	// Setting
	void set(const int x, const int y);

	// Transform
    void rotate(const float angle);
    float angle() const;
	float angle(const Vector2i& v2) const;

	// Vector functions
    void normalize();
    Vector2i normalized() const;
    float dot(const Vector2i& v2) const;
    float cross(const Vector2i& v2) const;
    float distance(const Vector2i& v2) const;
	float magnitude() const;
 
	// Operators
    Vector2i& operator=(const Vector2i& v2);
    Vector2i& operator+=(const Vector2i& v2);
    Vector2i& operator-=(const Vector2i& v2);
    Vector2i& operator*=(const float scalar);
    Vector2i& operator/=(const float scalar);
    const Vector2i operator+(const Vector2i &v2) const;
    const Vector2i operator-(const Vector2i &v2) const;
    const Vector2i operator*(const float scalar) const;
    const Vector2i operator/(const float scalar) const;
    bool operator==(const Vector2i& v2) const;

	bool operator<(const Vector2i& v2) const
	{
		TUPLE_CMP(this->x, v2.x)
		TUPLE_CMP(this->y, v2.y)
		return false;
	}

	bool operator>(const Vector2i& v2) const
	{
		TUPLE_CMP2(this->x, v2.x)
		TUPLE_CMP2(this->y, v2.y)
		return false;
	}

public:

	// AngelScript
	static void DefaultConstructor(Vector2i *self);
	static void CopyConstructor1(const Vector2i &other, Vector2i *self);
	static void CopyConstructor2(const Vector2 &other, Vector2i *self);
	static void InitConstructor1(int xy, Vector2i *self);
	static void InitConstructor2(int x, int y, Vector2i *self);
 
public:
    int x, y;
	
	// Type id
	static int TypeId;
};


class X2DAPI vec3
{
public:
	// Constructor
    vec3(float xyz);
    vec3(float x, float y, float z);
 
	// Setting
	void set(const float x, const float y, const float z);

	// Null
    void nullify();
    bool isNull() const;

	// Transform
    void rotate(const float angle);
    float angle() const;
	float angle(const vec3& v2) const;

	// Vector functions
    void normalize();
    vec3 normalized() const;
    float dot(const vec3& v2) const;
    vec3 cross(const vec3& v2) const;
    float distance(const vec3& v2) const;
	float magnitude() const;
	float length() const;
 
	// Operators
    vec3& operator= (const vec3& v2);
    vec3& operator+= (const vec3& v2);
    vec3& operator-= (const vec3& v2);
    vec3& operator*= (const float scalar);
    vec3& operator/= (const float scalar);
    const vec3 operator+(const vec3 &v2) const;
    const vec3 operator-(const vec3 &v2) const;
    const vec3 operator*(const float scalar) const;
    const vec3 operator/(const float scalar) const;
    bool operator== (const vec3& v2) const;

public:

	// AngelScript
	static void DefaultConstructor(vec3 *self);
	static void CopyConstructor(const vec3 &other, vec3 *self);
	static void InitConstructor1(float xyz, vec3 *self);
	static void InitConstructor2(float x, float y, float z, vec3 *self);
 
public:
    float x, y, z;
	
	// Type id
	static int TypeId;
};

class X2DAPI vec4
{
public:
	// Constructor
    vec4(float xyzw);
    vec4(float x, float y, float z, float w);

	// Color
	float r() { return x; }
	float g() { return y; }
	float b() { return z; }
	float a() { return w; }
	void setR(const float r) { x = r; }
	void setG(const float g) { y = g; }
	void setB(const float b) { z = b; }
	void setA(const float a) { w = a; }
 
	// Setting
	void set(const float x, const float y, const float z, const float w);

	// Transform
    void rotate(const float angle);
    float angle() const;
	float angle(const vec4& v2) const;

	// Vector functions
    void normalize();
    vec4 normalized() const;
    float dot(const vec4& v2) const;
    vec4 cross(const vec4& v2) const;
    float distance(const vec4& v2) const;
	float magnitude() const;
	float length() const;
 
	// Operators
    vec4& operator= (const vec4& v2);
    vec4& operator+= (const vec4& v2);
    vec4& operator-= (const vec4& v2);
    vec4& operator*= (const float scalar);
    vec4& operator/= (const float scalar);
    const vec4 operator+(const vec4 &v2) const;
    const vec4 operator-(const vec4 &v2) const;
    const vec4 operator*(const float scalar) const;
    const vec4 operator/(const float scalar) const;
    bool operator== (const vec4& v2) const;

public:

	// AngelScript
	static void DefaultConstructor(vec4 *self);
	static void CopyConstructor(const vec4 &other, vec4 *self);
	static void InitConstructor1(float xyz, vec4 *self);
	static void InitConstructor2(float x, float y, float z, float w, vec4 *self);
 
public:
    float x, y, z, w;
	
	// Type id
	static int TypeId;
};

#endif // MATH_VECTOR_H