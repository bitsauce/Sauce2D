//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)
// Inspired by: http://www.danielsoltyka.com/index.php/2010/05/30/c-Vector2-rectangle-classes/

#include <x2d/math.h>
 
//-----------------------------------------------------
// Vector2
// - A 2-dimentional vector class
//-----------------------------------------------------

Vector2::Vector2(const float xy)
{
	x = xy;
	y = xy;
}

Vector2::Vector2(const float d[2])
{
	x = d[0];
	y = d[1];
}

Vector2::Vector2(const float x, const float y)
{
    this->x = x;
    this->y = y;
}

Vector2::Vector2(const Vector2 &v)
{
    this->x = v.x;
    this->y = v.y;
}

Vector2::Vector2(const Vector2i &v)
{
    this->x = (float) v.x;
    this->y = (float) v.y;
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

Vector2 &Vector2::operator*=(const Vector2 &v2)
{
    x *= v2.x;
    y *= v2.y;
    return *this;
}

Vector2& Vector2::operator/=(const float scalar)
{
    x /= scalar;
    y /= scalar;
    return *this;
}

Vector2& Vector2::operator/=(const Vector2 &v2)
{
    x /= v2.x;
    y /= v2.y;
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

const Vector2 Vector2::operator*(const Vector2 &v2) const
{
	return  Vector2(*this) *= v2;
}

const Vector2 Vector2::operator/(const float scalar) const
{
    return Vector2(*this) /= scalar;
}

const Vector2 Vector2::operator/(const Vector2 &v2) const
{
    return Vector2(*this) /= v2;
}

bool Vector2::operator==(const Vector2& v2) const
{
    return (x == v2.x) && (y == v2.y);
}

bool Vector2::operator!=(const Vector2& v2) const
{
    return (x != v2.x) || (y != v2.y);
}

// Vector2i

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
    this->x = (int) v.x;
    this->y = (int) v.y;
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
    x = (int) (x * scalar);
	y = (int) (y * scalar);
    return *this;
}
 
Vector2i& Vector2i::operator/=(const float scalar)
{
    x = (int) (x / scalar);
	y = (int) (y / scalar);
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

bool Vector2i::operator==(const Vector2i& v2) const
{
    return x == v2.x && y == v2.y;
}

bool Vector2i::operator!=(const Vector2i& v2) const
{
    return x != v2.x || y != v2.y;
}
 
//-----------------------------------------------------
// Vector3
// - A 3-dimentional vector class
//-----------------------------------------------------

Vector3::Vector3(float xyz)
{
	x = xyz;
	y = xyz;
	z = xyz;
}

Vector3::Vector3(float x, float y, float z)
{
    this->x = x;
    this->y = y;
	this->z = z;
}

Vector3::Vector3(const Vector3 &other)
{
    this->x = other.x;
    this->y = other.y;
	this->z = other.z;
}

//----------------------------------------------------------------------------
// Functions
//----------------------------------------------------------------------------

// Setting
void Vector3::set(const float x, const float y, const float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

// Null
void Vector3::nullify()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

bool Vector3::isNull() const
{
	return x == 0.0f && y == 0.0f && z == 0.0f;
}

// Transform
void Vector3::rotate(const float angle)
{
    // TODO: Implement
}

float Vector3::angle() const
{
    // TODO: Implement
	return 0;
}

float Vector3::angle(const Vector3& v2) const
{
    // TODO: Implement
	return 0;
}

// Vector functions
void Vector3::normalize()
{
	if(isNull()) return;
    float mag = magnitude();
    x /= mag;
    y /= mag;
    z /= mag;
}

Vector3 Vector3::normalized() const
{
	if(isNull()) return Vector3(0.0f);
    float mag = magnitude();
	return Vector3(x/mag, y/mag, z/mag);
}

float Vector3::dot(const Vector3 &v2) const
{
    return (x*v2.x)+(y*v2.y)+(z*v2.z);
}

Vector3 Vector3::cross(const Vector3 &v2) const
{
    return Vector3((y*v2.z)-(z*v2.y), (z*v2.x)-(x*v2.z), (x*v2.y)-(y*v2.x));
}

float Vector3::magnitude() const
{
    return sqrtf(x*x+y*y+z*z);
}

float Vector3::length() const
{
    return sqrtf(x*x+y*y+z*z);
}

float Vector3::distance(const Vector3& v2) const
{
    // TODO: Implement
	return 0;
}

Vector3& Vector3::operator=(const Vector3& v2)
{
    if(this == &v2) return *this;
    x = v2.x;
    y = v2.y;
	z = v2.z;
    return *this;
}

Vector3& Vector3::operator+=(const Vector3& v2)
{
    x += v2.x;
    y += v2.y;
    z += v2.z;
    return *this;
}
 
Vector3& Vector3::operator-=(const Vector3& v2)
{
    x -= v2.x;
    y -= v2.y;
    z -= v2.z;
    return *this;
}
 
Vector3& Vector3::operator*=(const float scalar)
{
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}
 
Vector3& Vector3::operator/=(const float scalar)
{
    x /= scalar;
    y /= scalar;
    z /= scalar;
    return *this;
}
 
const Vector3 Vector3::operator+(const Vector3 &v2) const
{
    return Vector3(*this) += v2;
}
 
const Vector3 Vector3::operator-(const Vector3 &v2) const
{
    return Vector3(*this) -= v2;
}
 
const Vector3 Vector3::operator*(const float scalar) const
{
    return Vector3(*this) *= scalar;
}
 
const Vector3 Vector3::operator/(const float scalar) const
{
    return Vector3(*this) /= scalar;
}
 
bool Vector3::operator==(const Vector3& v2) const
{
    return x == v2.x && y == v2.y && z == v2.z;
}

//-----------------------------------------------------
// Vector4
// - A 4-dimentional vector class
//-----------------------------------------------------

Vector4::Vector4(float xyzw)
{
	x = y = z = w = xyzw;
}

Vector4::Vector4(float x, float y, float z, float w)
{
    this->x = x;
    this->y = y;
	this->z = z;
	this->w = w;
}

Vector4::Vector4(const float d[4])
{
    this->x = d[0];
	this->y = d[1];
	this->z = d[2];
	this->w = d[3];
}

//----------------------------------------------------------------------------
// Functions
//----------------------------------------------------------------------------

// Setting
void Vector4::set(const float x, const float y, const float z, const float w)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

// Transform
void Vector4::rotate(const float angle)
{
    // TODO: Implement
}

float Vector4::angle() const
{
    // TODO: Implement
	return 0;
}

float Vector4::angle(const Vector4& v2) const
{
    // TODO: Implement
	return 0;
}

// Vector functions
void Vector4::normalize()
{
	if(x == 0.0f && y == 0.0f && z == 0.0f && w == 0.0f) return;
    float mag = magnitude();
    x /= mag;
    y /= mag;
    z /= mag;
    w /= mag;
}

Vector4 Vector4::normalized() const
{
	if(x == 0.0f && y == 0.0f && z == 0.0f && w == 0.0f) return Vector4(0.0f);
    float mag = magnitude();
	return Vector4(x/mag, y/mag, z/mag, w/mag);
}

float Vector4::dot(const Vector4 &v2) const
{
    return (x*v2.x)+(y*v2.y)+(z*v2.z)+(w*v2.w);
}

Vector4 Vector4::cross(const Vector4 &v2) const
{
    // TODO: Implement?
	return Vector4(0.0f);
}

float Vector4::magnitude() const
{
    return sqrtf(x*x+y*y+z*z+w*w);
}

float Vector4::length() const
{
    return sqrtf(x*x+y*y+z*z+w*w);
}

float Vector4::distance(const Vector4& v2) const
{
    // TODO: Implement?
	return 0;
}

Vector4& Vector4::operator=(const Vector4& v2)
{
    if(this == &v2) return *this;
    x = v2.x;
    y = v2.y;
	z = v2.z;
	w = v2.w;
    return *this;
}

Vector4& Vector4::operator+=(const Vector4& v2)
{
    x += v2.x;
    y += v2.y;
    z += v2.z;
    return *this;
}
 
Vector4& Vector4::operator-=(const Vector4& v2)
{
    x -= v2.x;
    y -= v2.y;
    z -= v2.z;
    w -= v2.w;
    return *this;
}
 
Vector4& Vector4::operator*=(const float scalar)
{
    x *= scalar;
    y *= scalar;
    z *= scalar;
    w *= scalar;
    return *this;
}
 
Vector4& Vector4::operator/=(const float scalar)
{
    x /= scalar;
    y /= scalar;
    z /= scalar;
    w /= scalar;
    return *this;
}
 
const Vector4 Vector4::operator+(const Vector4 &v2) const
{
    return Vector4(*this) += v2;
}
 
const Vector4 Vector4::operator-(const Vector4 &v2) const
{
    return Vector4(*this) -= v2;
}
 
const Vector4 Vector4::operator*(const float scalar) const
{
    return Vector4(*this) *= scalar;
}
 
const Vector4 Vector4::operator/(const float scalar) const
{
    return Vector4(*this) /= scalar;
}
 
bool Vector4::operator== (const Vector4& v2) const
{
    return ((x == v2.x) && (y == v2.y)) && (z == v2.z) && (w == v2.w);
}