#ifndef X2D_VECTOR_H
#define X2D_VECTOR_H

#include "../config.h"

/** \brief A class which holds a 2 dimentional vector
 *
 * The Vector2 class is a 2 dimentional (hence the 2) vector.
 * It stores the x and y value as floating-point values.
 *
 */

class Vector2i;
class Vector3;
class Vector4;

class XDAPI Vector2
{
public:
	// Constructor
    Vector2(const float xy = 0.0f);
    Vector2(const float d[2]);
    Vector2(const float x, const float y);
	Vector2(const Vector2 &v);
 
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
	const Vector2 operator*(const Vector2 &v2) const;
    const Vector2 operator/(const float scalar) const;
    bool operator==(const Vector2& v2) const;
	operator Vector2i() const;

public:

	// AngelScript
	static void DefaultConstructor(Vector2 *self);
	static void CopyConstructor(const Vector2 &other, Vector2 *self);
	static void InitConstructor1(const float xy, Vector2 *self);
	static void InitConstructor2(const float x, const float y, Vector2 *self);
 
public:
    float x, y;
};

class XDAPI Vector2i
{
public:
	// Constructor
    Vector2i(int xy = 0.0f);
    Vector2i(int x, int y);
 
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
	operator Vector2() const;

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
	static void CopyConstructor(const Vector2i &other, Vector2i *self);
	static void InitConstructor1(int xy, Vector2i *self);
	static void InitConstructor2(int x, int y, Vector2i *self);
 
public:
    int x, y;
};


class XDAPI Vector3
{
public:
	// Constructor
    Vector3(float xyz = 0.0f);
    Vector3(float x, float y, float z);
    Vector3(const Vector3 &other);

	// Swizzling
	Vector3(const float x, const Vector2 &v) { this->x = x; this->y = v.x; this->z = v.y; }
    Vector3(const Vector2 &v, const float z)  { this->x = v.x; this->y = v.y; this->z = z; }

	Vector2 getXX() const { return Vector2(x, x); }
	Vector2 getXY() const { return Vector2(x, y); }
	Vector2 getXZ() const { return Vector2(x, z); }
	Vector2 getYX() const { return Vector2(y, x); }
	Vector2 getYY() const { return Vector2(y, y); }
	Vector2 getYZ() const { return Vector2(y, z); }
	Vector2 getZX() const { return Vector2(z, x); }
	Vector2 getZY() const { return Vector2(z, y); }
	Vector2 getZZ() const { return Vector2(z, z); }
	
	void setXX(const Vector2 &v) { x = v.y; }
	void setXY(const Vector2 &v) { x = v.x; y = v.y; }
	void setXZ(const Vector2 &v) { x = v.x; z = v.y; }
	void setYX(const Vector2 &v) { y = v.x; x = v.y; }
	void setYY(const Vector2 &v) { y = v.y; }
	void setYZ(const Vector2 &v) { y = v.x; z = v.y; }
	void setZX(const Vector2 &v) { z = v.x; x = v.y; }
	void setZY(const Vector2 &v) { z = v.x; y = v.y; }
	void setZZ(const Vector2 &v) { z = v.y; }
 
	// Setting
	void set(const float x, const float y, const float z);

	// Null
    void nullify();
    bool isNull() const;

	// Transform
    void rotate(const float angle);
    float angle() const;
	float angle(const Vector3& v2) const;

	// Vector functions
    void normalize();
    Vector3 normalized() const;
    float dot(const Vector3& v2) const;
    Vector3 cross(const Vector3& v2) const;
    float distance(const Vector3& v2) const;
	float magnitude() const;
	float length() const;
 
	// Operators
    Vector3& operator=(const Vector3& v2);
    Vector3& operator+=(const Vector3& v2);
    Vector3& operator-=(const Vector3& v2);
    Vector3& operator*=(const float scalar);
    Vector3& operator/=(const float scalar);
    const Vector3 operator+(const Vector3 &v2) const;
    const Vector3 operator-(const Vector3 &v2) const;
    const Vector3 operator*(const float scalar) const;
    const Vector3 operator/(const float scalar) const;
    bool operator==(const Vector3& v2) const;

public:

	// AngelScript
	static void DefaultConstructor(Vector3 *self);
	static void CopyConstructor(const Vector3 &other, Vector3 *self);
	static void InitConstructor1(float xyz, Vector3 *self);
	static void InitConstructor2(float x, float y, float z, Vector3 *self);
 
public:
    float x, y, z;
};

class XDAPI Vector4
{
public:
	// Constructor
    Vector4(float xyzw = 0.0f);
    Vector4(float x, float y, float z, float w);
    Vector4(const float d[4]);
    Vector4(const Vector2 &v0, const Vector2 &v1);
    Vector4(const float x, const float y, const Vector2 &v);
    Vector4(const float x, const Vector2 &v, const float w);
    Vector4(const Vector2 &v, const float z, const float w);
    Vector4(const float x, const Vector3 &v);
    Vector4(const Vector3 &v, const float w);

	// Swizzling operators
	Vector2 getXX() const { return Vector2(x, x); }
	Vector2 getXY() const { return Vector2(x, y); }
	Vector2 getXZ() const { return Vector2(x, z); }
	Vector2 getYX() const { return Vector2(y, x); }
	Vector2 getYY() const { return Vector2(y, y); }
	Vector2 getYZ() const { return Vector2(y, z); }
	Vector2 getZX() const { return Vector2(z, x); }
	Vector2 getZY() const { return Vector2(z, y); }
	Vector2 getZZ() const { return Vector2(z, z); }
	
	Vector3 getXXX() const { return Vector3(x, x, x); }
	Vector3 getXXY() const { return Vector3(x, x, y); }
	Vector3 getXXZ() const { return Vector3(x, x, z); }
	Vector3 getXXW() const { return Vector3(x, x, w); }
	Vector3 getXYX() const { return Vector3(x, y, x); }
	Vector3 getXYY() const { return Vector3(x, y, y); }
	Vector3 getXYZ() const { return Vector3(x, y, z); }
	Vector3 getXYW() const { return Vector3(x, y, w); }
	Vector3 getXZX() const { return Vector3(x, z, x); }
	Vector3 getXZY() const { return Vector3(x, z, y); }
	Vector3 getXZZ() const { return Vector3(x, z, z); }
	Vector3 getXZW() const { return Vector3(x, z, w); }
	Vector3 getXWX() const { return Vector3(x, w, x); }
	Vector3 getXWY() const { return Vector3(x, w, y); }
	Vector3 getXWZ() const { return Vector3(x, w, z); }
	Vector3 getXWW() const { return Vector3(x, w, w); }
	
	Vector3 getYXX() const { return Vector3(y, x, x); }
	Vector3 getYXY() const { return Vector3(y, x, y); }
	Vector3 getYXZ() const { return Vector3(y, x, z); }
	Vector3 getYXW() const { return Vector3(y, x, w); }
	Vector3 getYYX() const { return Vector3(y, y, x); }
	Vector3 getYYY() const { return Vector3(y, y, y); }
	Vector3 getYYZ() const { return Vector3(y, y, z); }
	Vector3 getYYW() const { return Vector3(y, y, w); }
	Vector3 getYZX() const { return Vector3(y, z, x); }
	Vector3 getYZY() const { return Vector3(y, z, y); }
	Vector3 getYZZ() const { return Vector3(y, z, z); }
	Vector3 getYZW() const { return Vector3(y, z, w); }
	Vector3 getYWX() const { return Vector3(y, w, x); }
	Vector3 getYWY() const { return Vector3(y, w, y); }
	Vector3 getYWZ() const { return Vector3(y, w, z); }
	Vector3 getYWW() const { return Vector3(y, w, w); }
	
	Vector3 getZXX() const { return Vector3(z, x, x); }
	Vector3 getZXY() const { return Vector3(z, x, y); }
	Vector3 getZXZ() const { return Vector3(z, x, z); }
	Vector3 getZXW() const { return Vector3(z, x, w); }
	Vector3 getZYX() const { return Vector3(z, y, x); }
	Vector3 getZYY() const { return Vector3(z, y, y); }
	Vector3 getZYZ() const { return Vector3(z, y, z); }
	Vector3 getZYW() const { return Vector3(z, y, w); }
	Vector3 getZZX() const { return Vector3(z, z, x); }
	Vector3 getZZY() const { return Vector3(z, z, y); }
	Vector3 getZZZ() const { return Vector3(z, z, z); }
	Vector3 getZZW() const { return Vector3(z, z, w); }
	Vector3 getZWX() const { return Vector3(z, w, x); }
	Vector3 getZWY() const { return Vector3(z, w, y); }
	Vector3 getZWZ() const { return Vector3(z, w, z); }
	Vector3 getZWW() const { return Vector3(z, w, w); }
	
	Vector3 getWXX() const { return Vector3(w, x, x); }
	Vector3 getWXY() const { return Vector3(w, x, y); }
	Vector3 getWXZ() const { return Vector3(w, x, z); }
	Vector3 getWXW() const { return Vector3(w, x, w); }
	Vector3 getWYX() const { return Vector3(w, y, x); }
	Vector3 getWYY() const { return Vector3(w, y, y); }
	Vector3 getWYZ() const { return Vector3(w, y, z); }
	Vector3 getWYW() const { return Vector3(w, y, w); }
	Vector3 getWZX() const { return Vector3(w, z, x); }
	Vector3 getWZY() const { return Vector3(w, z, y); }
	Vector3 getWZZ() const { return Vector3(w, z, z); }
	Vector3 getWZW() const { return Vector3(w, z, w); }
	Vector3 getWWX() const { return Vector3(w, w, x); }
	Vector3 getWWY() const { return Vector3(w, w, y); }
	Vector3 getWWZ() const { return Vector3(w, w, z); }
	Vector3 getWWW() const { return Vector3(w, w, w); }
	
	void setXX(const Vector2 &v) { x = v.y; }
	void setXY(const Vector2 &v) { x = v.x; y = v.y; }
	void setXZ(const Vector2 &v) { x = v.x; z = v.y; }
	void setYX(const Vector2 &v) { y = v.x; x = v.y; }
	void setYY(const Vector2 &v) { y = v.y; }
	void setYZ(const Vector2 &v) { y = v.x; z = v.y; }
	void setZX(const Vector2 &v) { z = v.x; x = v.y; }
	void setZY(const Vector2 &v) { z = v.x; y = v.y; }
	void setZZ(const Vector2 &v) { z = v.y; }
	
	void setXYZ(const Vector3 &v) { x = v.x; y = v.y; z = v.z; }
 
	// Setting
	void set(const float x, const float y, const float z, const float w);

	// Transform
    void rotate(const float angle);
    float angle() const;
	float angle(const Vector4& v2) const;

	// Vector functions
    void normalize();
    Vector4 normalized() const;
    float dot(const Vector4& v2) const;
    Vector4 cross(const Vector4& v2) const;
    float distance(const Vector4& v2) const;
	float magnitude() const;
	float length() const;
 
	// Operators
    Vector4& operator=(const Vector4& v2);
    Vector4& operator+=(const Vector4& v2);
    Vector4& operator-=(const Vector4& v2);
    Vector4& operator*=(const float scalar);
    Vector4& operator/=(const float scalar);
    const Vector4 operator+(const Vector4 &v2) const;
    const Vector4 operator-(const Vector4 &v2) const;
    const Vector4 operator*(const float scalar) const;
    const Vector4 operator/(const float scalar) const;
    bool operator==(const Vector4& v2) const;

public:
    float x, y, z, w;

	// AngelScript
	static void DefaultConstructor(Vector4 *self);
	static void CopyConstructor(const Vector4 &other, Vector4 *self);
	static void InitConstructor1(float xyz, Vector4 *self);
	static void InitConstructor2(float x, float y, float z, float w, Vector4 *self);
};

#endif // X2D_VECTOR_H