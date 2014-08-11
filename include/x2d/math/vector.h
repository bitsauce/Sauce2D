#ifndef MATH_VECTOR_H
#define MATH_VECTOR_H
 
#include <x2d/config.h>
#include <x2d/base.h>

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
	AS_DECL_VALUE

	// Constructor
    Vector2(const float xy);
    Vector2(const float d[2]);
    Vector2(const float x, const float y);
    Vector2(const Vector2i &v);
    Vector2(const Vector3 &v);
    Vector2(const Vector4 &v);
 
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

	// Serialization
	void serialize(stringstream &ss) const;
	void deserialize(stringstream &ss);

public:

	// AngelScript
	static void DefaultConstructor(Vector2 *self);
	static void CopyConstructor1(const Vector2 &other, Vector2 *self);
	static void CopyConstructor2(const Vector2i &other, Vector2 *self);
	static void InitConstructor1(const float xy, Vector2 *self);
	static void InitConstructor2(const float x, const float y, Vector2 *self);
 
public:
    float x, y;
};

#define TUPLE_CMP(a, b) \
	if(a < b) return true; \
	if(a > b) return false;

#define TUPLE_CMP2(a, b) \
	if(a < b) return false; \
	if(a > b) return true;

class XDAPI Vector2i
{
public:
	AS_DECL_VALUE

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


class XDAPI Vector3
{
public:
	AS_DECL_VALUE

	// Constructor
    Vector3(float xyz);
    Vector3(float x, float y, float z);
    Vector3(const Vector4 &v);
    Vector3(const Vector2 &v);

	// Getting
	float getR() const { return x; }
	float getG() const { return y; }
	float getB() const { return z; }
 
	// Setting
	void set(const float x, const float y, const float z);
	void setR(const float r) { x = r; }
	void setG(const float g) { y = g; }
	void setB(const float b) { z = b; }

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
    Vector3& operator= (const Vector3& v2);
    Vector3& operator+= (const Vector3& v2);
    Vector3& operator-= (const Vector3& v2);
    Vector3& operator*= (const float scalar);
    Vector3& operator/= (const float scalar);
    const Vector3 operator+(const Vector3 &v2) const;
    const Vector3 operator-(const Vector3 &v2) const;
    const Vector3 operator*(const float scalar) const;
    const Vector3 operator/(const float scalar) const;
    bool operator== (const Vector3& v2) const;

public:

	// AngelScript
	static void DefaultConstructor(Vector3 *self);
	static void CopyConstructor1(const Vector3 &other, Vector3 *self);
	static void CopyConstructor2(const Vector2 &other, Vector3 *self);
	static void InitConstructor1(float xyz, Vector3 *self);
	static void InitConstructor2(float x, float y, float z, Vector3 *self);
 
public:
    float x, y, z;
	
	// Type id
	static int TypeId;
};

class XDAPI Vector4
{
public:
	AS_DECL_VALUE

	// Constructor
    Vector4(float xyzw = 0.0f);
    Vector4(float x, float y, float z, float w);
    Vector4(const float d[4]);

	// Getters
	float getR() const { return x; }
	float getG() const { return y; }
	float getB() const { return z; }
	float getA() const { return w; }
	Vector3 getRGB() const { return Vector3(x, y, z); }
 
	// Setting
	void set(const float x, const float y, const float z, const float w);
	void setR(const float r) { x = r; }
	void setG(const float g) { y = g; }
	void setB(const float b) { z = b; }
	void setA(const float a) { w = a; }
	void setRGB(const Vector3 &rgb) { x = rgb.x; y = rgb.y; z = rgb.z; }

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
    Vector4& operator= (const Vector4& v2);
    Vector4& operator+= (const Vector4& v2);
    Vector4& operator-= (const Vector4& v2);
    Vector4& operator*= (const float scalar);
    Vector4& operator/= (const float scalar);
    const Vector4 operator+(const Vector4 &v2) const;
    const Vector4 operator-(const Vector4 &v2) const;
    const Vector4 operator*(const float scalar) const;
    const Vector4 operator/(const float scalar) const;
    bool operator== (const Vector4& v2) const;

public:

	// AngelScript
	static void DefaultConstructor(Vector4 *self);
	static void CopyConstructor(const Vector4 &other, Vector4 *self);
	static void InitConstructor1(float xyz, Vector4 *self);
	static void InitConstructor2(float x, float y, float z, float w, Vector4 *self);
 
public:
    float x, y, z, w;
};

#endif // MATH_VECTOR_H