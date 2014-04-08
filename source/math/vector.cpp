//     _                                     ____  ____  
//    / \  _   _ _ __ ___  _ __ __ _  __  __|___ \|  _ \ 
//   / _ \| | | |  __/ _ \|  __/ _  | \ \/ /  __) | | | |
//  / ___ \ |_| | | | (_) | | | (_| |  >  <  / __/| |_| |
// /_/   \_\__ _|_|  \___/|_|  \__ _| /_/\_\|_____|____/ 
//		MixedGraphics (C)
//		Inspired by: http://www.danielsoltyka.com/index.php/2010/05/30/c-Vector2-rectangle-classes/


#include "x2d/math/vector.h"
#include "x2d/scripts.h"

void ConstructVector2(Vector2 *self)
{
	new(self) Vector2(0.0f);
}
 
//-----------------------------------------------------
// Vector2
// - A 2-dimentional vector class
//-----------------------------------------------------

void RegisterVectors()
{
	// Vector2
	int r;

	// Register the value type
	AS_REGISTER_VALUE(Vector2)
	Vector2::TypeId = r;

	// Register the object properties
	AS_VALUE_PROPERTY(Vector2, "float x", x)
	AS_VALUE_PROPERTY(Vector2, "float y", y)

	// Register the constructors
	r = scriptEngine->RegisterObjectBehaviour("Vector2", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(ConstructVector2), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	//AS_VALUE_BEHAVIOURO(Vector2, "void f()", asBEHAVE_CONSTRUCT, DefaultConstructor)
	AS_VALUE_BEHAVIOURO(Vector2, "void f(const Vector2 &in)", asBEHAVE_CONSTRUCT, CopyConstructor1)
	AS_VALUE_BEHAVIOURO(Vector2, "void f(float)", asBEHAVE_CONSTRUCT, InitConstructor1)
	AS_VALUE_BEHAVIOURO(Vector2, "void f(float, float)", asBEHAVE_CONSTRUCT, InitConstructor2)

	// Register the operator overloads
	AS_VALUE_METHOD(Vector2, "Vector2 &opAddAssign(const Vector2 &in)", operator+=)
	AS_VALUE_METHOD(Vector2, "Vector2 &opSubAssign(const Vector2 &in)", operator-=)
	AS_VALUE_METHOD(Vector2, "Vector2 &opMulAssign(const float)", operator*=)
	AS_VALUE_METHOD(Vector2, "Vector2 &opDivAssign(const float)", operator/=)
	AS_VALUE_METHOD(Vector2, "bool opEquals(const Vector2 &in) const", operator==)
	AS_VALUE_METHOD(Vector2, "Vector2 opAdd(const Vector2 &in) const", operator+)
	AS_VALUE_METHOD(Vector2, "Vector2 opSub(const Vector2 &in) const", operator-)
	AS_VALUE_METHOD(Vector2, "Vector2 opMul(const float) const", operator*)
	AS_VALUE_METHOD(Vector2, "Vector2 opDiv(const float) const", operator/)

	// Register the object methods
	AS_VALUE_METHOD(Vector2, "void set(const float x, const float y)", set)
	AS_VALUE_METHOD(Vector2, "void rotate(const float)", rotate)
	AS_VALUE_METHOD(Vector2, "Vector2 normalized() const", normalized)
	AS_VALUE_METHOD(Vector2, "void normalize()", normalize)
	AS_VALUE_METHOD(Vector2, "float dot(const Vector2 &in) const", dot)
	AS_VALUE_METHOD(Vector2, "float cross(const Vector2 &in) const", cross)
	AS_VALUE_METHOD(Vector2, "float distance(const Vector2 &in) const", distance)
	AS_VALUE_METHOD(Vector2, "float magnitude() const", magnitude)
	AS_VALUE_METHOD(Vector2, "float length() const", magnitude)
	AS_VALUE_METHODPR(Vector2, "float angle() const", angle, () const, float)
	AS_VALUE_METHODPR(Vector2, "float angle(const Vector2 &in) const", angle, (const Vector2&) const, float)

	// Vector2i

	// Register the value type
	AS_REGISTER_VALUE(Vector2i)
	Vector2i::TypeId = r;

	// Register the object properties
	AS_VALUE_PROPERTY(Vector2i, "int x", x)
	AS_VALUE_PROPERTY(Vector2i, "int y", y)

	// Register the constructors
	AS_VALUE_BEHAVIOURO(Vector2i, "void f()", asBEHAVE_CONSTRUCT, DefaultConstructor)
	AS_VALUE_BEHAVIOURO(Vector2i, "void f(const Vector2i &in)", asBEHAVE_CONSTRUCT, CopyConstructor1)
	AS_VALUE_BEHAVIOURO(Vector2i, "void f(const Vector2 &in)", asBEHAVE_CONSTRUCT, CopyConstructor2)
	AS_VALUE_BEHAVIOURO(Vector2, "void f(const Vector2i &in)", asBEHAVE_CONSTRUCT, CopyConstructor2) // Vector2 to Vector2i
	AS_VALUE_BEHAVIOURO(Vector2i, "void f(int)", asBEHAVE_CONSTRUCT, InitConstructor1)
	AS_VALUE_BEHAVIOURO(Vector2i, "void f(int, int)", asBEHAVE_CONSTRUCT, InitConstructor2)

	// Register the operator overloads
	AS_VALUE_METHOD(Vector2i, "Vector2i &opAddAssign(const Vector2i &in)", operator+=)
	AS_VALUE_METHOD(Vector2i, "Vector2i &opSubAssign(const Vector2i &in)", operator-=)
	AS_VALUE_METHOD(Vector2i, "Vector2i &opMulAssign(const float)", operator*=)
	AS_VALUE_METHOD(Vector2i, "Vector2i &opDivAssign(const float)", operator/=)
	AS_VALUE_METHOD(Vector2i, "bool opEquals(const Vector2i &in) const", operator==)
	AS_VALUE_METHOD(Vector2i, "Vector2i opAdd(const Vector2i &in) const", operator+)
	AS_VALUE_METHOD(Vector2i, "Vector2i opSub(const Vector2i &in) const", operator-)
	AS_VALUE_METHOD(Vector2i, "Vector2i opMul(const float) const", operator*)
	AS_VALUE_METHOD(Vector2i, "Vector2i opDiv(const float) const", operator/)

	// Register the object methods
	AS_VALUE_METHOD(Vector2i, "void set(const int x, const int y)", set)
	AS_VALUE_METHOD(Vector2i, "void rotate(const float)", rotate)
	AS_VALUE_METHOD(Vector2i, "Vector2i normalized() const", normalized)
	AS_VALUE_METHOD(Vector2i, "void normalize()", normalize)
	AS_VALUE_METHOD(Vector2i, "float dot(const Vector2i &in) const", dot)
	AS_VALUE_METHOD(Vector2i, "float cross(const Vector2i &in) const", cross)
	AS_VALUE_METHOD(Vector2i, "float distance(const Vector2i &in) const", distance)
	AS_VALUE_METHOD(Vector2i, "float magnitude() const", magnitude)
	AS_VALUE_METHOD(Vector2i, "float length() const", magnitude)
	AS_VALUE_METHODPR(Vector2i, "float angle() const", angle, () const, float)
	AS_VALUE_METHODPR(Vector2i, "float angle(const Vector2i &in) const", angle, (const Vector2i&) const, float)
	
	// vec3

	// Register the value type
	AS_REGISTER_VALUE(vec3)
	vec3::TypeId = r;

	// Register the object properties
	AS_VALUE_PROPERTY(vec3, "float x", x)
	AS_VALUE_PROPERTY(vec3, "float y", y)
	AS_VALUE_PROPERTY(vec3, "float z", z)

	// Register the constructors
	AS_VALUE_BEHAVIOURO(vec3, "void f()", asBEHAVE_CONSTRUCT, DefaultConstructor)
	AS_VALUE_BEHAVIOURO(vec3, "void f(const vec3 &in)", asBEHAVE_CONSTRUCT, CopyConstructor)
	AS_VALUE_BEHAVIOURO(vec3, "void f(float)", asBEHAVE_CONSTRUCT, InitConstructor1)
	AS_VALUE_BEHAVIOURO(vec3, "void f(float, float, float)", asBEHAVE_CONSTRUCT, InitConstructor2)

	// Register the operator overloads
	AS_VALUE_METHOD(vec3, "vec3 &opAddAssign(const vec3 &in)", operator+=)
	AS_VALUE_METHOD(vec3, "vec3 &opSubAssign(const vec3 &in)", operator-=)
	AS_VALUE_METHOD(vec3, "vec3 &opMulAssign(const float)", operator*=)
	AS_VALUE_METHOD(vec3, "vec3 &opDivAssign(const float)", operator/=)
	AS_VALUE_METHOD(vec3, "bool opEquals(const vec3 &in) const", operator==)
	AS_VALUE_METHOD(vec3, "vec3 opAdd(const vec3 &in) const", operator+)
	AS_VALUE_METHOD(vec3, "vec3 opSub(const vec3 &in) const", operator-)
	AS_VALUE_METHOD(vec3, "vec3 opMul(const float) const", operator*)
	AS_VALUE_METHOD(vec3, "vec3 opDiv(const float) const", operator/)

	// Register the object methods
	AS_VALUE_METHOD(vec3, "void set(const float x, const float y, const float z)", set)
	AS_VALUE_METHOD(vec3, "void nullify()", nullify)
	AS_VALUE_METHOD(vec3, "bool isNull()", isNull)
	AS_VALUE_METHOD(vec3, "void rotate(const float)", rotate)
	AS_VALUE_METHOD(vec3, "vec3 normalized() const", normalized)
	AS_VALUE_METHOD(vec3, "void normalize()", normalize)
	AS_VALUE_METHOD(vec3, "float dot(const vec3 &in) const", dot)
	AS_VALUE_METHOD(vec3, "float cross(const vec3 &in) const", cross)
	AS_VALUE_METHOD(vec3, "float distance(const vec3 &in) const", distance)
	AS_VALUE_METHOD(vec3, "float magnitude() const", magnitude)
	AS_VALUE_METHOD(vec3, "float length() const", length)
	AS_VALUE_METHODPR(vec3, "float angle() const", angle, () const, float)
	AS_VALUE_METHODPR(vec3, "float angle(const vec3 &in) const", angle, (const vec3&) const, float)
	
	// vec4

	// Register the value type
	AS_REGISTER_VALUE(vec4)
	vec4::TypeId = r;

	// Register the object properties
	AS_VALUE_PROPERTY(vec4, "float x", x)
	AS_VALUE_PROPERTY(vec4, "float y", y)
	AS_VALUE_PROPERTY(vec4, "float z", z)
	AS_VALUE_PROPERTY(vec4, "float w", w)

	AS_VALUE_METHOD(vec4, "float get_r()", r)
	AS_VALUE_METHOD(vec4, "float get_g()", g)
	AS_VALUE_METHOD(vec4, "float get_b()", b)
	AS_VALUE_METHOD(vec4, "float get_a()", a)

	AS_VALUE_METHOD(vec4, "void set_r(const float)", setR)
	AS_VALUE_METHOD(vec4, "void set_g(const float)", setG)
	AS_VALUE_METHOD(vec4, "void set_b(const float)", setB)
	AS_VALUE_METHOD(vec4, "void set_a(const float)", setA)

	// Register the constructors
	AS_VALUE_BEHAVIOURO(vec4, "void f()", asBEHAVE_CONSTRUCT, DefaultConstructor)
	AS_VALUE_BEHAVIOURO(vec4, "void f(const vec4 &in)", asBEHAVE_CONSTRUCT, CopyConstructor)
	AS_VALUE_BEHAVIOURO(vec4, "void f(float)", asBEHAVE_CONSTRUCT, InitConstructor1)
	AS_VALUE_BEHAVIOURO(vec4, "void f(float, float, float, float)", asBEHAVE_CONSTRUCT, InitConstructor2)

	// Register the operator overloads
	AS_VALUE_METHOD(vec4, "vec4 &opAddAssign(const vec4 &in)", operator+=)
	AS_VALUE_METHOD(vec4, "vec4 &opSubAssign(const vec4 &in)", operator-=)
	AS_VALUE_METHOD(vec4, "vec4 &opMulAssign(const float)", operator*=)
	AS_VALUE_METHOD(vec4, "vec4 &opDivAssign(const float)", operator/=)
	AS_VALUE_METHOD(vec4, "bool opEquals(const vec4 &in) const", operator==)
	AS_VALUE_METHOD(vec4, "vec4 opAdd(const vec4 &in) const", operator+)
	AS_VALUE_METHOD(vec4, "vec4 opSub(const vec4 &in) const", operator-)
	AS_VALUE_METHOD(vec4, "vec4 opMul(const float) const", operator*)
	AS_VALUE_METHOD(vec4, "vec4 opDiv(const float) const", operator/)

	// Register the object methods
	AS_VALUE_METHOD(vec4, "void set(const float x, const float y, const float z, const float w)", set)
	AS_VALUE_METHOD(vec4, "void rotate(const float)", rotate)
	AS_VALUE_METHOD(vec4, "vec4 normalized() const", normalized)
	AS_VALUE_METHOD(vec4, "void normalize()", normalize)
	AS_VALUE_METHOD(vec4, "float dot(const vec4 &in) const", dot)
	AS_VALUE_METHOD(vec4, "float cross(const vec4 &in) const", cross)
	AS_VALUE_METHOD(vec4, "float distance(const vec4 &in) const", distance)
	AS_VALUE_METHOD(vec4, "float magnitude() const", magnitude)
	AS_VALUE_METHOD(vec4, "float length() const", length)
	AS_VALUE_METHODPR(vec4, "float angle() const", angle, () const, float)
	AS_VALUE_METHODPR(vec4, "float angle(const vec4 &in) const", angle, (const vec4&) const, float)
}

//----------------------------------------------------------------------------
// Functions
//----------------------------------------------------------------------------

// Vector2

int Vector2::TypeId;

Vector2::Vector2(float xy)
{
	x = xy;
	y = xy;
}

Vector2::Vector2(float x, float y)
{
    this->x = x;
    this->y = y;
}

Vector2::Vector2(const Vector2i &v)
{
    this->x = (float)v.x;
    this->y = (float)v.y;
}

void Vector2::set(const float x, const float y)
{
    this->x = x;
    this->y = y;
}

// Transform
void Vector2::rotate(const float angle)
{
    x = (x * cosf(angle)) - (y * sinf(angle));
    y = (y * cosf(angle)) + (x * sinf(angle));
}

float Vector2::angle() const
{
    return atan2(y, x);
}

float Vector2::angle(const Vector2& v2) const
{
	return acos(dot(v2)/(magnitude()*v2.magnitude()));
}

// Vector functions
void Vector2::normalize()
{
    float mag = magnitude();
    if(mag != 0.0) {
        x /= mag;
        y /= mag;
    }
}

Vector2 Vector2::normalized() const
{
	if(x == 0.0f && y == 0.0f)
		return Vector2(0.0f);
    float mag = magnitude();
	return Vector2(x/mag, y/mag);
}

float Vector2::dot(const Vector2 &v2) const
{
    return (x*v2.x)+(y*v2.y);
}

float Vector2::cross(const Vector2 &v2) const
{
    return (x*v2.y)-(y*v2.x);
}

float Vector2::magnitude() const
{
    return sqrtf(x*x+y*y);
}

float Vector2::distance(const Vector2& v2) const
{
    return sqrtf(pow((v2.x - x), 2 ) + pow((v2.y - y), 2));
}

Vector2& Vector2::operator=(const Vector2& v2)
{
    if(this == &v2) return *this;
    x = v2.x;
    y = v2.y;
    return *this;
}

Vector2& Vector2::operator+=(const Vector2& v2)
{
    x += v2.x;
    y += v2.y;
    return *this;
}
 
Vector2& Vector2::operator-=(const Vector2& v2)
{
    x -= v2.x;
    y -= v2.y;
    return *this;
}
 
Vector2& Vector2::operator*=(const float scalar)
{
    x *= scalar;
    y *= scalar;
    return *this;
}
 
Vector2& Vector2::operator/=(const float scalar)
{
    x /= scalar;
    y /= scalar;
    return *this;
}
 
const Vector2 Vector2::operator+(const Vector2 &v2) const
{
    return Vector2(*this) += v2;
}
 
const Vector2 Vector2::operator-(const Vector2 &v2) const
{
    return Vector2(*this) -= v2;
}
 
const Vector2 Vector2::operator*(const float scalar) const
{
    return Vector2(*this) *= scalar;
}
 
const Vector2 Vector2::operator/(const float scalar) const
{
    return Vector2(*this) /= scalar;
}
 
bool Vector2::operator== (const Vector2& v2) const
{
    return ((x == v2.x) && (y == v2.y));
}

// Vector2i

int Vector2i::TypeId;

Vector2i::Vector2i(int xy)
{
	x = xy;
	y = xy;
}

Vector2i::Vector2i(int x, int y)
{
    this->x = x;
    this->y = y;
}

Vector2i::Vector2i(const Vector2 &v)
{
    this->x = (int)v.x;
    this->y = (int)v.y;
}

// Setting
void Vector2i::set(const int x, const int y)
{
    this->x = x;
    this->y = y;
}

// Transform
void Vector2i::rotate(const float angle)
{
    x = int(x * cosf(angle)) - int(y * sinf(angle));
    y = int(y * cosf(angle)) + int(x * sinf(angle));
}

float Vector2i::angle() const
{
    return atan2((float)y, (float)x);
}

float Vector2i::angle(const Vector2i& v2) const
{
	return acos(dot(v2)/(magnitude()*v2.magnitude()));
}

// Vector functions
void Vector2i::normalize()
{
    int mag = (int)magnitude();
    if(mag != 0.0) {
        x /= mag;
        y /= mag;
    }
}

Vector2i Vector2i::normalized() const
{
	if(x == 0.0f && y == 0.0f)
		return Vector2i(0);
    float mag = magnitude();
	return Vector2i(int(x/mag), int(y/mag));
}

float Vector2i::dot(const Vector2i &v2) const
{
    return float(x*v2.x)+float(y*v2.y);
}

float Vector2i::cross(const Vector2i &v2) const
{
    return float(x*v2.y)-float(y*v2.x);
}

float Vector2i::magnitude() const
{
    return sqrtf((float)x*x+(float)y*y);
}

float Vector2i::distance(const Vector2i& v2) const
{
    return sqrtf(pow(float(v2.x - x), 2.0f) + pow(float(v2.y - y), 2.0f));
}

Vector2i& Vector2i::operator=(const Vector2i& v2)
{
    if(this == &v2) return *this;
    x = v2.x;
    y = v2.y;
    return *this;
}

Vector2i& Vector2i::operator+=(const Vector2i& v2)
{
    x += v2.x;
    y += v2.y;
    return *this;
}
 
Vector2i& Vector2i::operator-=(const Vector2i& v2)
{
    x -= v2.x;
    y -= v2.y;
    return *this;
}
 
Vector2i& Vector2i::operator*=(const float scalar)
{
    x *= (int)scalar;
    y *= (int)scalar;
    return *this;
}
 
Vector2i& Vector2i::operator/=(const float scalar)
{
    x /= (int)scalar;
    y /= (int)scalar;
    return *this;
}
 
const Vector2i Vector2i::operator+(const Vector2i &v2) const
{
    return Vector2i(*this) += v2;
}
 
const Vector2i Vector2i::operator-(const Vector2i &v2) const
{
    return Vector2i(*this) -= v2;
}
 
const Vector2i Vector2i::operator*(const float scalar) const
{
    return Vector2i(*this) *= scalar;
}
 
const Vector2i Vector2i::operator/(const float scalar) const
{
    return Vector2i(*this) /= scalar;
}
 
bool Vector2i::operator== (const Vector2i& v2) const
{
    return ((x == v2.x) && (y == v2.y));
}

//----------------------------------------------------------------------------
// AngelScript
//----------------------------------------------------------------------------

// Vector2

void Vector2::CopyConstructor1(const Vector2 &other, Vector2 *self)
{
	new(self) Vector2(other);
}

void Vector2::CopyConstructor2(const Vector2i &other, Vector2 *self)
{
	new(self) Vector2(other);
}

void Vector2::InitConstructor1(float xy, Vector2 *self)
{
	new(self) Vector2(xy);
}

void Vector2::InitConstructor2(float x, float y, Vector2 *self)
{
	new(self) Vector2(x, y);
}

// Vector2i

void Vector2i::DefaultConstructor(Vector2i *self)
{
	new(self) Vector2i(0);
}

void Vector2i::CopyConstructor1(const Vector2i &other, Vector2i *self)
{
	new(self) Vector2i(other);
}

void Vector2i::CopyConstructor2(const Vector2 &other, Vector2i *self)
{
	new(self) Vector2i(other);
}

void Vector2i::InitConstructor1(int xy, Vector2i *self)
{
	new(self) Vector2i(xy);
}

void Vector2i::InitConstructor2(int x, int y, Vector2i *self)
{
	new(self) Vector2i(x, y);
}
 
//-----------------------------------------------------
// vec3
// - A 3-dimentional vector class
//-----------------------------------------------------

int vec3::TypeId;

vec3::vec3(float xyz)
{
	x = xyz;
	y = xyz;
	z = xyz;
}

vec3::vec3(float x, float y, float z)
{
    this->x = x;
    this->y = y;
	this->z = z;
}

//----------------------------------------------------------------------------
// Functions
//----------------------------------------------------------------------------

// Setting
void vec3::set(const float x, const float y, const float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

// Null
void vec3::nullify()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

bool vec3::isNull() const
{
	return x == 0.0f && y == 0.0f && z == 0.0f;
}

// Transform
void vec3::rotate(const float angle)
{
    // TODO: Implement
}

float vec3::angle() const
{
    // TODO: Implement
	return 0;
}

float vec3::angle(const vec3& v2) const
{
    // TODO: Implement
	return 0;
}

// Vector functions
void vec3::normalize()
{
	if(isNull()) return;
    float mag = magnitude();
    x /= mag;
    y /= mag;
    z /= mag;
}

vec3 vec3::normalized() const
{
	if(isNull()) return vec3(0.0f);
    float mag = magnitude();
	return vec3(x/mag, y/mag, z/mag);
}

float vec3::dot(const vec3 &v2) const
{
    return (x*v2.x)+(y*v2.y)+(z*v2.z);
}

vec3 vec3::cross(const vec3 &v2) const
{
    return vec3((y*v2.z)-(z*v2.y), (z*v2.x)-(x*v2.z), (x*v2.y)-(y*v2.x));
}

float vec3::magnitude() const
{
    return sqrtf(x*x+y*y+z*z);
}

float vec3::length() const
{
    return sqrtf(x*x+y*y+z*z);
}

float vec3::distance(const vec3& v2) const
{
    // TODO: Implement
	return 0;
}

vec3& vec3::operator=(const vec3& v2)
{
    if(this == &v2) return *this;
    x = v2.x;
    y = v2.y;
	z = v2.z;
    return *this;
}

vec3& vec3::operator+=(const vec3& v2)
{
    x += v2.x;
    y += v2.y;
    z += v2.z;
    return *this;
}
 
vec3& vec3::operator-=(const vec3& v2)
{
    x -= v2.x;
    y -= v2.y;
    z -= v2.z;
    return *this;
}
 
vec3& vec3::operator*=(const float scalar)
{
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}
 
vec3& vec3::operator/=(const float scalar)
{
    x /= scalar;
    y /= scalar;
    z /= scalar;
    return *this;
}
 
const vec3 vec3::operator+(const vec3 &v2) const
{
    return vec3(*this) += v2;
}
 
const vec3 vec3::operator-(const vec3 &v2) const
{
    return vec3(*this) -= v2;
}
 
const vec3 vec3::operator*(const float scalar) const
{
    return vec3(*this) *= scalar;
}
 
const vec3 vec3::operator/(const float scalar) const
{
    return vec3(*this) /= scalar;
}
 
bool vec3::operator== (const vec3& v2) const
{
    return ((x == v2.x) && (y == v2.y)) && (z == v2.z);
}

//----------------------------------------------------------------------------
// AngelScript
//----------------------------------------------------------------------------

void vec3::DefaultConstructor(vec3 *self)
{
	new(self) vec3(0.0f);
}

void vec3::CopyConstructor(const vec3 &other, vec3 *self)
{
	new(self) vec3(other);
}

void vec3::InitConstructor1(float xyz, vec3 *self)
{
	new(self) vec3(xyz);
}

void vec3::InitConstructor2(float x, float y, float z, vec3 *self)
{
	new(self) vec3(x, y, z);
}

//-----------------------------------------------------
// vec4
// - A 4-dimentional vector class
//-----------------------------------------------------

int vec4::TypeId;

vec4::vec4(float xyzw)
{
	x = xyzw;
	y = xyzw;
	z = xyzw;
	w = xyzw;
}

vec4::vec4(float x, float y, float z, float w)
{
    this->x = x;
    this->y = y;
	this->z = z;
	this->w = w;
}

//----------------------------------------------------------------------------
// Functions
//----------------------------------------------------------------------------

// Setting
void vec4::set(const float x, const float y, const float z, const float w)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

// Transform
void vec4::rotate(const float angle)
{
    // TODO: Implement
}

float vec4::angle() const
{
    // TODO: Implement
	return 0;
}

float vec4::angle(const vec4& v2) const
{
    // TODO: Implement
	return 0;
}

// Vector functions
void vec4::normalize()
{
	if(x == 0.0f && y == 0.0f && z == 0.0f && w == 0.0f) return;
    float mag = magnitude();
    x /= mag;
    y /= mag;
    z /= mag;
    w /= mag;
}

vec4 vec4::normalized() const
{
	if(x == 0.0f && y == 0.0f && z == 0.0f && w == 0.0f) return vec4(0.0f);
    float mag = magnitude();
	return vec4(x/mag, y/mag, z/mag, w/mag);
}

float vec4::dot(const vec4 &v2) const
{
    return (x*v2.x)+(y*v2.y)+(z*v2.z)+(w*v2.w);
}

vec4 vec4::cross(const vec4 &v2) const
{
    // TODO: Implement?
	return 0;
}

float vec4::magnitude() const
{
    return sqrtf(x*x+y*y+z*z+w*w);
}

float vec4::length() const
{
    return sqrtf(x*x+y*y+z*z+w*w);
}

float vec4::distance(const vec4& v2) const
{
    // TODO: Implement?
	return 0;
}

vec4& vec4::operator=(const vec4& v2)
{
    if(this == &v2) return *this;
    x = v2.x;
    y = v2.y;
	z = v2.z;
	w = v2.w;
    return *this;
}

vec4& vec4::operator+=(const vec4& v2)
{
    x += v2.x;
    y += v2.y;
    z += v2.z;
    return *this;
}
 
vec4& vec4::operator-=(const vec4& v2)
{
    x -= v2.x;
    y -= v2.y;
    z -= v2.z;
    w -= v2.w;
    return *this;
}
 
vec4& vec4::operator*=(const float scalar)
{
    x *= scalar;
    y *= scalar;
    z *= scalar;
    w *= scalar;
    return *this;
}
 
vec4& vec4::operator/=(const float scalar)
{
    x /= scalar;
    y /= scalar;
    z /= scalar;
    w /= scalar;
    return *this;
}
 
const vec4 vec4::operator+(const vec4 &v2) const
{
    return vec4(*this) += v2;
}
 
const vec4 vec4::operator-(const vec4 &v2) const
{
    return vec4(*this) -= v2;
}
 
const vec4 vec4::operator*(const float scalar) const
{
    return vec4(*this) *= scalar;
}
 
const vec4 vec4::operator/(const float scalar) const
{
    return vec4(*this) /= scalar;
}
 
bool vec4::operator== (const vec4& v2) const
{
    return ((x == v2.x) && (y == v2.y)) && (z == v2.z) && (w == v2.w);
}

//----------------------------------------------------------------------------
// AngelScript
//----------------------------------------------------------------------------

void vec4::DefaultConstructor(vec4 *self)
{
	new(self) vec4(0.0f);
}

void vec4::CopyConstructor(const vec4 &other, vec4 *self)
{
	new(self) vec4(other);
}

void vec4::InitConstructor1(float xyz, vec4 *self)
{
	new(self) vec4(xyz);
}

void vec4::InitConstructor2(float x, float y, float z, float w, vec4 *self)
{
	new(self) vec4(x, y, z, w);
}