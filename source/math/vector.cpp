//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)
//		Inspired by: http://www.danielsoltyka.com/index.php/2010/05/30/c-Vector2-rectangle-classes/


#include <x2d/math/vector.h>
#include <x2d/scriptengine.h>
 
//-----------------------------------------------------
// Vector2
// - A 2-dimentional vector class
//-----------------------------------------------------

AS_REG_VALUE(Vector2)

int Vector2::Register(asIScriptEngine *scriptEngine)
{
	// Vector2
	int r;

	// Register the object properties
	r = scriptEngine->RegisterObjectProperty("Vector2", "float x", offsetof(Vector2, x)); AS_ASSERT
	r = scriptEngine->RegisterObjectProperty("Vector2", "float y", offsetof(Vector2, y)); AS_ASSERT

	// Register the constructors
	r = scriptEngine->RegisterObjectBehaviour("Vector2", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(DefaultConstructor), asCALL_CDECL_OBJLAST); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("Vector2", asBEHAVE_CONSTRUCT, "void f(float)", asFUNCTION(InitConstructor1), asCALL_CDECL_OBJLAST); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("Vector2", asBEHAVE_CONSTRUCT, "void f(float, float)", asFUNCTION(InitConstructor2), asCALL_CDECL_OBJLAST); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("Vector2", asBEHAVE_CONSTRUCT, "void f(const Vector2 &in)", asFUNCTION(CopyConstructor1), asCALL_CDECL_OBJLAST); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("Vector2", asBEHAVE_CONSTRUCT, "void f(const Vector2i &in)", asFUNCTION(CopyConstructor2), asCALL_CDECL_OBJLAST); AS_ASSERT

	// Register the operator overloads
	r = scriptEngine->RegisterObjectMethod("Vector2", "Vector2 &opAddAssign(const Vector2 &in)", asMETHOD(Vector2, operator+=), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector2", "Vector2 &opSubAssign(const Vector2 &in)", asMETHOD(Vector2, operator-=), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector2", "Vector2 &opMulAssign(const float)", asMETHOD(Vector2, operator*=), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector2", "Vector2 &opDivAssign(const float)", asMETHOD(Vector2, operator/=), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector2", "bool opEquals(const Vector2 &in) const", asMETHOD(Vector2, operator==), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector2", "Vector2 opAdd(const Vector2 &in) const", asMETHOD(Vector2, operator+), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector2", "Vector2 opSub(const Vector2 &in) const", asMETHOD(Vector2, operator-), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector2", "Vector2 opMul(const float) const", asMETHODPR(Vector2, operator*, (const float) const, const Vector2), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector2", "Vector2 opMul(const Vector2 &in) const", asMETHODPR(Vector2, operator*, (const Vector2&) const, const Vector2), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector2", "Vector2 opDiv(const float) const", asMETHOD(Vector2, operator/), asCALL_THISCALL); AS_ASSERT

	// Register the object methods
	r = scriptEngine->RegisterObjectMethod("Vector2", "void set(const float x, const float y)", asMETHOD(Vector2, set), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector2", "void rotate(const float)", asMETHOD(Vector2, rotate), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector2", "Vector2 normalized() const", asMETHOD(Vector2, normalized), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector2", "void normalize()", asMETHOD(Vector2, normalize), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector2", "float dot(const Vector2 &in) const", asMETHOD(Vector2, dot), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector2", "float cross(const Vector2 &in) const", asMETHOD(Vector2, cross), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector2", "float distance(const Vector2 &in) const", asMETHOD(Vector2, distance), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector2", "float magnitude() const", asMETHOD(Vector2, magnitude), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector2", "float length() const", asMETHOD(Vector2, magnitude), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector2", "float angle() const", asMETHODPR(Vector2, angle, () const, float), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector2", "float angle(const Vector2 &in) const", asMETHODPR(Vector2, angle, (const Vector2&) const, float), asCALL_THISCALL); AS_ASSERT
	
	return r;
}

AS_REG_VALUE(Vector2i)

int Vector2i::Register(asIScriptEngine *scriptEngine)
{
	// Vector2i
	int r = 0;

	// Register the object properties
	r = scriptEngine->RegisterObjectProperty("Vector2i", "int x", offsetof(Vector2i, x)); AS_ASSERT
	r = scriptEngine->RegisterObjectProperty("Vector2i", "int y", offsetof(Vector2i, y)); AS_ASSERT

	// Register the constructors
	r = scriptEngine->RegisterObjectBehaviour("Vector2i", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(DefaultConstructor), asCALL_CDECL_OBJLAST); 
	r = scriptEngine->RegisterObjectBehaviour("Vector2i", asBEHAVE_CONSTRUCT, "void f(const Vector2i &in)", asFUNCTION(CopyConstructor1), asCALL_CDECL_OBJLAST); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("Vector2i", asBEHAVE_CONSTRUCT, "void f(const Vector2 &in)", asFUNCTION(CopyConstructor2), asCALL_CDECL_OBJLAST); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("Vector2i", asBEHAVE_CONSTRUCT, "void f(int)", asFUNCTION(InitConstructor1), asCALL_CDECL_OBJLAST); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("Vector2i", asBEHAVE_CONSTRUCT, "void f(int, int)", asFUNCTION(InitConstructor2), asCALL_CDECL_OBJLAST); AS_ASSERT

	// Register the operator overloads
	r = scriptEngine->RegisterObjectMethod("Vector2i", "Vector2i &opAddAssign(const Vector2i &in)", asMETHOD(Vector2i, operator+=), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector2i", "Vector2i &opSubAssign(const Vector2i &in)", asMETHOD(Vector2i, operator-=), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector2i", "Vector2i &opMulAssign(const float)", asMETHOD(Vector2i, operator*=), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector2i", "Vector2i &opDivAssign(const float)", asMETHOD(Vector2i, operator/=), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector2i", "bool opEquals(const Vector2i &in) const", asMETHOD(Vector2i, operator==), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector2i", "Vector2i opAdd(const Vector2i &in) const", asMETHOD(Vector2i, operator+), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector2i", "Vector2i opSub(const Vector2i &in) const", asMETHOD(Vector2i, operator-), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector2i", "Vector2i opMul(const float) const", asMETHOD(Vector2i, operator*), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector2i", "Vector2i opDiv(const float) const", asMETHOD(Vector2i, operator/), asCALL_THISCALL); AS_ASSERT

	// Register the object methods
	r = scriptEngine->RegisterObjectMethod("Vector2i", "void set(const int x, const int y)", asMETHOD(Vector2i, set), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector2i", "void rotate(const float)", asMETHOD(Vector2i, rotate), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector2i", "Vector2i normalized() const", asMETHOD(Vector2i, normalized), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector2i", "void normalize()", asMETHOD(Vector2i, normalize), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector2i", "float dot(const Vector2i &in) const", asMETHOD(Vector2i, dot), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector2i", "float cross(const Vector2i &in) const", asMETHOD(Vector2i, cross), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector2i", "float distance(const Vector2i &in) const", asMETHOD(Vector2i, distance), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector2i", "float magnitude() const", asMETHOD(Vector2i, magnitude), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector2i", "float length() const", asMETHOD(Vector2i, magnitude), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector2i", "float angle() const", asMETHODPR(Vector2i, angle, () const, float), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector2i", "float angle(const Vector2i &in) const", asMETHODPR(Vector2i, angle, (const Vector2i&) const, float), asCALL_THISCALL); AS_ASSERT

	return r;
}

AS_REG_VALUE(Vector3)
	
int Vector3::Register(asIScriptEngine *scriptEngine)
{
	// Vector3
	int r = 0;

	// Register the object properties
	r = scriptEngine->RegisterObjectProperty("Vector3", "int x", offsetof(Vector3, x)); AS_ASSERT
	r = scriptEngine->RegisterObjectProperty("Vector3", "int y", offsetof(Vector3, y)); AS_ASSERT
	r = scriptEngine->RegisterObjectProperty("Vector3", "int z", offsetof(Vector3, z)); AS_ASSERT
	
	r = scriptEngine->RegisterObjectMethod("Vector3", "float get_r() const", asMETHOD(Vector3, getR), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector3", "float get_g() const", asMETHOD(Vector3, getG), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector3", "float get_b() const", asMETHOD(Vector3, getB), asCALL_THISCALL); AS_ASSERT

	r = scriptEngine->RegisterObjectMethod("Vector3", "void set_r(const float)", asMETHOD(Vector3, setR), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector3", "void set_g(const float)", asMETHOD(Vector3, setG), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector3", "void set_b(const float)", asMETHOD(Vector3, setB), asCALL_THISCALL); AS_ASSERT
	
	// Register the constructors
	r = scriptEngine->RegisterObjectBehaviour("Vector3", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(DefaultConstructor), asCALL_CDECL_OBJLAST); 
	r = scriptEngine->RegisterObjectBehaviour("Vector3", asBEHAVE_CONSTRUCT, "void f(const Vector3 &in)", asFUNCTION(CopyConstructor1), asCALL_CDECL_OBJLAST); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("Vector3", asBEHAVE_CONSTRUCT, "void f(const Vector2 &in)", asFUNCTION(CopyConstructor2), asCALL_CDECL_OBJLAST); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("Vector3", asBEHAVE_CONSTRUCT, "void f(float)", asFUNCTION(InitConstructor1), asCALL_CDECL_OBJLAST); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("Vector3", asBEHAVE_CONSTRUCT, "void f(float, float, float)", asFUNCTION(InitConstructor2), asCALL_CDECL_OBJLAST); AS_ASSERT

	// Register the operator overloads
	r = scriptEngine->RegisterObjectMethod("Vector3", "Vector3 &opAddAssign(const Vector3 &in)", asMETHOD(Vector3, operator+=), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector3", "Vector3 &opSubAssign(const Vector3 &in)", asMETHOD(Vector3, operator-=), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector3", "Vector3 &opMulAssign(const float)", asMETHOD(Vector3, operator*=), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector3", "Vector3 &opDivAssign(const float)", asMETHOD(Vector3, operator/=), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector3", "bool opEquals(const Vector3 &in) const", asMETHOD(Vector3, operator==), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector3", "Vector3 opAdd(const Vector3 &in) const", asMETHOD(Vector3, operator+), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector3", "Vector3 opSub(const Vector3 &in) const", asMETHOD(Vector3, operator-), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector3", "Vector3 opMul(const float) const", asMETHOD(Vector3, operator*), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector3", "Vector3 opDiv(const float) const", asMETHOD(Vector3, operator/), asCALL_THISCALL); AS_ASSERT

	// Register the object methods
	r = scriptEngine->RegisterObjectMethod("Vector3", "void set(const float x, const float y, const float z)", asMETHOD(Vector3, set), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector3", "void nullify()", asMETHOD(Vector3, nullify), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector3", "bool isNull()", asMETHOD(Vector3, isNull), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector3", "void rotate(const float)", asMETHOD(Vector3, rotate), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector3", "Vector3 normalized() const", asMETHOD(Vector3, normalized), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector3", "void normalize()", asMETHOD(Vector3, normalize), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector3", "float dot(const Vector3 &in) const", asMETHOD(Vector3, dot), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector3", "float cross(const Vector3 &in) const", asMETHOD(Vector3, cross), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector3", "float distance(const Vector3 &in) const", asMETHOD(Vector3, distance), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector3", "float magnitude() const", asMETHOD(Vector3, magnitude), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector3", "float length() const", asMETHOD(Vector3, length), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector3", "float angle() const", asMETHODPR(Vector3, angle, () const, float), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector3", "float angle(const Vector3 &in) const", asMETHODPR(Vector3, angle, (const Vector3&) const, float), asCALL_THISCALL); AS_ASSERT

	return r;
}

AS_REG_VALUE(Vector4)

int Vector4::Register(asIScriptEngine *scriptEngine)
{
	// Vector4
	int r = 0;

	// Register the object properties
	r = scriptEngine->RegisterObjectProperty("Vector4", "int x", offsetof(Vector4, x)); AS_ASSERT
	r = scriptEngine->RegisterObjectProperty("Vector4", "int y", offsetof(Vector4, y)); AS_ASSERT
	r = scriptEngine->RegisterObjectProperty("Vector4", "int z", offsetof(Vector4, z)); AS_ASSERT
	r = scriptEngine->RegisterObjectProperty("Vector4", "int w", offsetof(Vector4, w)); AS_ASSERT

	r = scriptEngine->RegisterObjectMethod("Vector4", "float get_r() const", asMETHOD(Vector4, getR), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector4", "float get_g() const", asMETHOD(Vector4, getG), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector4", "float get_b() const", asMETHOD(Vector4, getB), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector4", "float get_a() const", asMETHOD(Vector4, getA), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector4", "Vector3 get_rgb() const", asMETHOD(Vector4, getRGB), asCALL_THISCALL); AS_ASSERT

	r = scriptEngine->RegisterObjectMethod("Vector4", "void set_r(const float)", asMETHOD(Vector4, setR), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector4", "void set_g(const float)", asMETHOD(Vector4, setG), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector4", "void set_b(const float)", asMETHOD(Vector4, setB), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector4", "void set_a(const float)", asMETHOD(Vector4, setA), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector4", "void set_rgb(const Vector3 &in)", asMETHOD(Vector4, setRGB), asCALL_THISCALL); AS_ASSERT

	// Register the constructors
	r = scriptEngine->RegisterObjectBehaviour("Vector4", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(DefaultConstructor), asCALL_CDECL_OBJLAST); 
	r = scriptEngine->RegisterObjectBehaviour("Vector4", asBEHAVE_CONSTRUCT, "void f(const Vector4 &in)", asFUNCTION(CopyConstructor), asCALL_CDECL_OBJLAST); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("Vector4", asBEHAVE_CONSTRUCT, "void f(float)", asFUNCTION(InitConstructor1), asCALL_CDECL_OBJLAST); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("Vector4", asBEHAVE_CONSTRUCT, "void f(float, float, float, float)", asFUNCTION(InitConstructor2), asCALL_CDECL_OBJLAST); AS_ASSERT

	// Register the operator overloads
	r = scriptEngine->RegisterObjectMethod("Vector4", "Vector4 &opAddAssign(const Vector4 &in)", asMETHOD(Vector4, operator+=), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector4", "Vector4 &opSubAssign(const Vector4 &in)", asMETHOD(Vector4, operator-=), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector4", "Vector4 &opMulAssign(const float)", asMETHOD(Vector4, operator*=), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector4", "Vector4 &opDivAssign(const float)", asMETHOD(Vector4, operator/=), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector4", "bool opEquals(const Vector4 &in) const", asMETHOD(Vector4, operator==), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector4", "Vector4 opAdd(const Vector4 &in) const", asMETHOD(Vector4, operator+), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector4", "Vector4 opSub(const Vector4 &in) const", asMETHOD(Vector4, operator-), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector4", "Vector4 opMul(const float) const", asMETHOD(Vector4, operator*), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector4", "Vector4 opDiv(const float) const", asMETHOD(Vector4, operator/), asCALL_THISCALL); AS_ASSERT

	// Register the object methods
	r = scriptEngine->RegisterObjectMethod("Vector4", "void set(const float x, const float y, const float z, const float w)", asMETHOD(Vector4, set), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector4", "void rotate(const float)", asMETHOD(Vector4, rotate), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector4", "Vector4 normalized() const", asMETHOD(Vector4, normalized), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector4", "void normalize()", asMETHOD(Vector4, normalize), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector4", "float dot(const Vector4 &in) const", asMETHOD(Vector4, dot), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector4", "float cross(const Vector4 &in) const", asMETHOD(Vector4, cross), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector4", "float distance(const Vector4 &in) const", asMETHOD(Vector4, distance), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector4", "float magnitude() const", asMETHOD(Vector4, magnitude), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector4", "float length() const", asMETHOD(Vector4, length), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector4", "float angle() const", asMETHODPR(Vector4, angle, () const, float), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Vector4", "float angle(const Vector4 &in) const", asMETHODPR(Vector4, angle, (const Vector4&) const, float), asCALL_THISCALL); AS_ASSERT

	return r;
}

//----------------------------------------------------------------------------
// Functions
//----------------------------------------------------------------------------

// Vector2

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

Vector2::Vector2(const Vector2i &v)
{
    this->x = (float)v.x;
    this->y = (float)v.y;
}

Vector2::Vector2(const Vector3 &v)
{
    this->x = v.x;
    this->y = v.y;
}

Vector2::Vector2(const Vector4 &v)
{
    this->x = v.x;
    this->y = v.y;
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

const Vector2 Vector2::operator*(const Vector2 &v2) const
{
    Vector2 v1(*this);
	v1.x *= v2.x;
	v1.y *= v2.y;
	return v1;
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

void Vector2::DefaultConstructor(Vector2 *self)
{
	new(self) Vector2(0.0f);
}

void Vector2::CopyConstructor1(const Vector2 &other, Vector2 *self)
{
	new(self) Vector2(other);
}

void Vector2::CopyConstructor2(const Vector2i &other, Vector2 *self)
{
	new(self) Vector2(other);
}

void Vector2::InitConstructor1(const float xy, Vector2 *self)
{
	new(self) Vector2(xy);
}

void Vector2::InitConstructor2(const float x, const float y, Vector2 *self)
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
// Vector3
// - A 3-dimentional vector class
//-----------------------------------------------------

int Vector3::TypeId;

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

Vector3::Vector3(const Vector4 &v)
{
    this->x = v.x;
    this->y = v.y;
	this->z = v.z;
}

Vector3::Vector3(const Vector2 &v)
{
    this->x = v.x;
    this->y = v.y;
	this->z = 0.0f;
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
 
bool Vector3::operator== (const Vector3& v2) const
{
    return ((x == v2.x) && (y == v2.y)) && (z == v2.z);
}

//----------------------------------------------------------------------------
// AngelScript
//----------------------------------------------------------------------------

void Vector3::DefaultConstructor(Vector3 *self)
{
	new(self) Vector3(0.0f);
}

void Vector3::CopyConstructor1(const Vector3 &other, Vector3 *self)
{
	new(self) Vector3(other);
}

void Vector3::CopyConstructor2(const Vector2 &other, Vector3 *self)
{
	new(self) Vector3(other);
}

void Vector3::InitConstructor1(float xyz, Vector3 *self)
{
	new(self) Vector3(xyz);
}

void Vector3::InitConstructor2(float x, float y, float z, Vector3 *self)
{
	new(self) Vector3(x, y, z);
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

//----------------------------------------------------------------------------
// AngelScript
//----------------------------------------------------------------------------

void Vector4::DefaultConstructor(Vector4 *self)
{
	new(self) Vector4(0.0f);
}

void Vector4::CopyConstructor(const Vector4 &other, Vector4 *self)
{
	new(self) Vector4(other);
}

void Vector4::InitConstructor1(float xyz, Vector4 *self)
{
	new(self) Vector4(xyz);
}

void Vector4::InitConstructor2(float x, float y, float z, float w, Vector4 *self)
{
	new(self) Vector4(x, y, z, w);
}