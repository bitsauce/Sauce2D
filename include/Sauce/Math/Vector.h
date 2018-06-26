#ifndef SAUCE_VECTOR_H
#define SAUCE_VECTOR_H

#include <Sauce/Config.h>

/** \brief A class which holds a 2 dimensional vector
 *
 * The Vector2F class is a 2 dimensional (hence the 2) vector.
 * It stores the x and y value as Ting-point values.
 *
 */

template<typename T>
class Vector2
{
public:
	inline explicit Vector2(const T xy = T(0))
	{
		x = xy;
		y = xy;
	}

	inline explicit Vector2(const T d[2])
	{
		x = d[0];
		y = d[1];
	}

	inline Vector2(const T x, const T y)
	{
		this->x = x;
		this->y = y;
	}

	template<typename U>
	inline Vector2(const Vector2<U> &v)
	{
		this->x = v.x;
		this->y = v.y;
	}
 
	inline void set(const T x, const T y)
	{
		this->x = x;
		this->y = y;
	}

	inline void rotate(const T angle)
	{
		T tmpX = (x * cosf(angle)) - (y * sinf(angle));
		y      = (y * cosf(angle)) + (x * sinf(angle));
		x = tmpX;
	}

	inline T angle() const
	{
		return atan2(y, x);
	}

	template<typename U>
	inline T angle(const Vector2<U> &v2) const
	{
		return acos(dot(v2) / (magnitude() * v2.magnitude()));
	}

	inline void normalize()
	{
		T len = length();
		if(len > T(0))
		{
			x /= len;
			y /= len;
		}
	}

	inline Vector2<T> normalized() const
	{
		T len = length();
		if(len > T(0))
		{
			return Vector2<T>(x / len, y / len);
		}
		return Vector2<T>();
	}

	template<typename U>
	inline T dot(const Vector2<U> &v2) const
	{
		return (x * v2.x) + (y * v2.y);
	}

	template<typename U>
	inline T cross(const Vector2<U> &v2) const
	{
		return (x * v2.y) - (y * v2.x);
	}

	template<typename U>
	inline T distance(const Vector2<U> &v2) const
	{
		return sqrtf(pow((v2.x - x), 2) + pow((v2.y - y), 2));
	}

	inline T length() const
	{
		return sqrtf(lengthSquared());
	}

	inline T lengthSquared() const
	{
		return x * x + y * y;
	}
 
	template<typename U>
	inline Vector2<T> &operator=(const Vector2<U> &v2)
	{
		x = v2.x;
		y = v2.y;
		return *this;
	}

	template<typename U>
	inline Vector2<T> &operator+=(const Vector2<U> &v2)
	{
		x += v2.x;
		y += v2.y;
		return *this;
	}

	template<typename U>
	inline Vector2<T> &operator-=(const Vector2<U> &v2)
	{
		x -= v2.x;
		y -= v2.y;
		return *this;
	}

	template<typename U>
	inline Vector2<T> &operator*=(const U scalar)
	{
		x *= scalar;
		y *= scalar;
		return *this;
	}

	template<typename U>
	inline Vector2<T> &operator*=(const Vector2<U> &v2)
	{
		x *= v2.x;
		y *= v2.y;
		return *this;
	}

	template<typename U>
	inline Vector2<T> &operator/=(const U scalar)
	{
		x /= scalar;
		y /= scalar;
		return *this;
	}

	template<typename U>
	inline Vector2<T> &operator/=(const Vector2<U> &v2)
	{
		x /= v2.x;
		y /= v2.y;
		return *this;
	}

	template<typename U>
	inline const Vector2<T> operator+(const Vector2<U> &v2) const
	{
		return Vector2<T>(*this) += v2;
	}

	template<typename U>
	inline const Vector2<T> operator-(const Vector2<U> &v2) const
	{
		return Vector2<T>(*this) -= v2;
	}

	template<typename U>
	inline const Vector2<T> operator*(const U scalar) const
	{
		return Vector2<T>(*this) *= scalar;
	}

	template<typename U>
	inline const Vector2<T> operator*(const Vector2<U> &v2) const
	{
		return  Vector2<T>(*this) *= v2;
	}

	template<typename U>
	inline const Vector2<T> operator/(const U scalar) const
	{
		return Vector2<T>(*this) /= scalar;
	}

	template<typename U>
	inline const Vector2<T> operator/(const Vector2<U> &v2) const
	{
		return Vector2<T>(*this) /= v2;
	}

	inline const Vector2<T> operator-()
	{
		return *this *= -1;
	}

	template<typename U>
	inline bool operator==(const Vector2<U> &v2) const
	{
		return (x == v2.x) && (y == v2.y);
	}

	template<typename U>
	inline bool operator!=(const Vector2<U> &v2) const
	{
		return (x != v2.x) || (y != v2.y);
	}

	template<typename U>
	inline bool operator<(const Vector2<U> &v2) const
	{
		TUPLE_CMP(this->x, v2.x);
		TUPLE_CMP(this->y, v2.y);
		return false;
	}

	template<typename U>
	inline bool operator>(const Vector2<U> &v2) const
	{
		TUPLE_CMP2(this->x, v2.x);
		TUPLE_CMP2(this->y, v2.y);
		return false;
	}

	string toString() const
	{
		stringstream ss;
		ss << *this;
		return ss.str();
	}

	friend ostream& operator<<(ostream &os, const Vector2<T> &rhs)
	{
		os << "[" << rhs.x << ", " << rhs.y << "]";
		return os;
	}

	static const Vector2<T> Zero;

    T x, y;
};

typedef Vector2<bool> Vector2B;
typedef Vector2<float> Vector2F;
typedef Vector2<double> Vector2D;
typedef Vector2<int> Vector2I;
typedef Vector2<uint> Vector2U;
typedef Vector2<uchar> Vector2UB;

/**
 * \class	Vector3
 *
 * \brief	3D vector.
 */

template<typename T>
class Vector3
{
public:
	inline explicit Vector3(T xyz = T(0))
	{
		x = y = z = xyz;
	}

	inline Vector3(T x, T y, T z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	inline explicit Vector3(T d[3])
	{
		this->x = d[0];
		this->y = d[1];
		this->z = d[2];
	}

	inline Vector3(const Vector3<T> &other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
	}

	inline Vector3(const T x, const Vector2<T> &v)
	{
		this->x = x; this->y = v.x; this->z = v.y;
	}

	inline Vector3(const Vector2<T> &v, const T z)
	{
		this->x = v.x; this->y = v.y; this->z = z;
	}

	inline T getR() const { return x; }
	inline T setR(const T &v) { x = v; }
	inline T getG() const { return y; }
	inline T setG(const T &v) { y = v; }
	inline T getB() const { return z; }
	inline T setB(const T &v) { z = v; }

	inline Vector2<T> getXX() const { return Vector2<T>(x, x); }
	inline Vector2<T> getXY() const { return Vector2<T>(x, y); }
	inline Vector2<T> getXZ() const { return Vector2<T>(x, z); }
	inline Vector2<T> getYX() const { return Vector2<T>(y, x); }
	inline Vector2<T> getYY() const { return Vector2<T>(y, y); }
	inline Vector2<T> getYZ() const { return Vector2<T>(y, z); }
	inline Vector2<T> getZX() const { return Vector2<T>(z, x); }
	inline Vector2<T> getZY() const { return Vector2<T>(z, y); }
	inline Vector2<T> getZZ() const { return Vector2<T>(z, z); }

	inline void setXX(const Vector2<T> &v) { x = v.y; }
	inline void setXY(const Vector2<T> &v) { x = v.x; y = v.y; }
	inline void setXZ(const Vector2<T> &v) { x = v.x; z = v.y; }
	inline void setYX(const Vector2<T> &v) { y = v.x; x = v.y; }
	inline void setYY(const Vector2<T> &v) { y = v.y; }
	inline void setYZ(const Vector2<T> &v) { y = v.x; z = v.y; }
	inline void setZX(const Vector2<T> &v) { z = v.x; x = v.y; }
	inline void setZY(const Vector2<T> &v) { z = v.x; y = v.y; }
	inline void setZZ(const Vector2<T> &v) { z = v.y; }

	inline void set(const T x, const T y, const T z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	inline void normalize()
	{
		T len = length();
		if(len > T(0))
		{
			x /= len;
			y /= len;
			z /= len;
		}
	}

	template<typename U>
	inline U dot(const Vector3<T> &v2) const
	{
		return (x * v2.x) + (y * v2.y) + (z * v2.z);
	}

	inline Vector3<T> cross(const Vector3<T>& v2) const
	{
		return Vector3<T>((y * v2.z) - (z * v2.y), (z * v2.x) - (x * v2.z), (x * v2.y) - (y * v2.x));
	}

#ifdef SAUCE_USE_FLOAT
	inline float length() const { return sqrtf(x * x + y * y + z * z); }
#else
	inline double length() const { return sqrt(x * x + y * y + z * z); }
#endif
 
	inline Vector3<T>& operator=(const Vector3<T>& v2)
	{
		if(this == &v2) return *this;
		x = v2.x;
		y = v2.y;
		z = v2.z;
		return *this;
	}

	inline Vector3<T>& operator+=(const Vector3<T>& v2)
	{
		x += v2.x;
		y += v2.y;
		z += v2.z;
		return *this;
	}

	inline Vector3<T>& operator-=(const Vector3<T>& v2)
	{
		x -= v2.x;
		y -= v2.y;
		z -= v2.z;
		return *this;
	}

	template<typename U>
	inline Vector3<T>& operator*=(const U scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}

	template<typename U>
	inline Vector3<T>& operator/=(const U scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;
		return *this;
	}

	inline const Vector3<T> operator+(const Vector3<T> &v2) const
	{
		return Vector3(*this) += v2;
	}

	inline const Vector3<T> operator-(const Vector3<T> &v2) const
	{
		return Vector3(*this) -= v2;
	}

	template<typename U>
	inline const Vector3<T> operator*(const U scalar) const
	{
		return Vector3(*this) *= scalar;
	}

	template<typename U>
	inline const Vector3<T> operator/(const U scalar) const
	{
		return Vector3(*this) /= scalar;
	}

	inline bool operator==(const Vector3<T>& v2) const
	{
		return x == v2.x && y == v2.y && z == v2.z;
	}

	inline bool operator!=(const Vector3<T>& v2) const
	{
		return (x != v2.x) || (y != v2.y) || (z != v2.z) ;
	}

	inline bool operator<(const Vector3<T> &v2) const
	{
		TUPLE_CMP(this->x, v2.x);
		TUPLE_CMP(this->y, v2.y);
		TUPLE_CMP(this->z, v2.z);
		return false;
	}

	inline bool operator>(const Vector3<T> &v2) const
	{
		TUPLE_CMP2(this->x, v2.x);
		TUPLE_CMP2(this->y, v2.y);
		TUPLE_CMP2(this->z, v2.z);
		return false;
	}

	inline string toString() const
	{
		stringstream ss;
		ss << *this;
		return ss.str();
	}

	friend ostream& operator<<(ostream &os, const Vector3<T> &rhs)
	{
		os << "[" << rhs.x << ", " << rhs.y << ", " << rhs.z << "]";
		return os;
	}

    T x, y, z;
};

typedef Vector3<bool> Vector3B;
typedef Vector3<float> Vector3F;
typedef Vector3<double> Vector3D;
typedef Vector3<int> Vector3I;
typedef Vector3<uint> Vector3U;
typedef Vector3<uchar> Vector3UB;
typedef Vector3<uchar> ColorRGB;

/**
 * \class	Vector4
 *
 * \brief	4D vector.
 */

template<typename T>
class Vector4
{
public:
	inline explicit Vector4(T xyzw = T(0))
	{
		x = y = z = w = xyzw;
	}

	inline Vector4(T x, T y, T z, T w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	inline explicit Vector4(const T d[4])
	{
		this->x = d[0];
		this->y = d[1];
		this->z = d[2];
		this->w = d[3];
	}

	inline Vector4(const Vector2<T> &v0, const Vector2<T> &v1)
	{
		this->x = v0.x;
		this->y = v0.y;
		this->z = v1.x;
		this->w = v1.y;
	}

	inline Vector4(const T x, const T y, const Vector2<T> &v)
	{
		this->x = x;
		this->y = y;
		this->z = v.x;
		this->w = v.y;
	}

	inline Vector4(const T x, const Vector2<T> &v, const T w)
	{
		this->x = x;
		this->y = v.x;
		this->z = v.y;
		this->w = w;
	}

	inline Vector4(const Vector2<T> &v, const T z, const T w)
	{
		this->x = v.x;
		this->y = v.y;
		this->z = z;
		this->w = w;
	}

	inline Vector4(const T x, const Vector3<T> &v)
	{
		this->x = x;
		this->y = v.x;
		this->z = v.y;
		this->w = v.z;
	}

	inline Vector4(const Vector3<T> &v, const T w)
	{
		this->x = v.x;
		this->y = v.y;
		this->z = v.z;
		this->w = w;
	}

	inline Vector3<T> getRGB() const { return Vector3<T>(x, y, z); }
	inline void setRGB(const Vector3<T> &rgb) { x = rgb.x; y = rgb.y; z = rgb.z; }

	inline Vector2<T> getXX() const { return Vector2<T>(x, x); }
	inline Vector2<T> getXY() const { return Vector2<T>(x, y); }
	inline Vector2<T> getXZ() const { return Vector2<T>(x, z); }
	inline Vector2<T> getYX() const { return Vector2<T>(y, x); }
	inline Vector2<T> getYY() const { return Vector2<T>(y, y); }
	inline Vector2<T> getYZ() const { return Vector2<T>(y, z); }
	inline Vector2<T> getZX() const { return Vector2<T>(z, x); }
	inline Vector2<T> getZY() const { return Vector2<T>(z, y); }
	inline Vector2<T> getZZ() const { return Vector2<T>(z, z); }
	
	inline Vector3<T> getXXX() const { return Vector3<T>(x, x, x); }
	inline Vector3<T> getXXY() const { return Vector3<T>(x, x, y); }
	inline Vector3<T> getXXZ() const { return Vector3<T>(x, x, z); }
	inline Vector3<T> getXXW() const { return Vector3<T>(x, x, w); }
	inline Vector3<T> getXYX() const { return Vector3<T>(x, y, x); }
	inline Vector3<T> getXYY() const { return Vector3<T>(x, y, y); }
	inline Vector3<T> getXYZ() const { return Vector3<T>(x, y, z); }
	inline Vector3<T> getXYW() const { return Vector3<T>(x, y, w); }
	inline Vector3<T> getXZX() const { return Vector3<T>(x, z, x); }
	inline Vector3<T> getXZY() const { return Vector3<T>(x, z, y); }
	inline Vector3<T> getXZZ() const { return Vector3<T>(x, z, z); }
	inline Vector3<T> getXZW() const { return Vector3<T>(x, z, w); }
	inline Vector3<T> getXWX() const { return Vector3<T>(x, w, x); }
	inline Vector3<T> getXWY() const { return Vector3<T>(x, w, y); }
	inline Vector3<T> getXWZ() const { return Vector3<T>(x, w, z); }
	inline Vector3<T> getXWW() const { return Vector3<T>(x, w, w); }
	
	inline Vector3<T> getYXX() const { return Vector3<T>(y, x, x); }
	inline Vector3<T> getYXY() const { return Vector3<T>(y, x, y); }
	inline Vector3<T> getYXZ() const { return Vector3<T>(y, x, z); }
	inline Vector3<T> getYXW() const { return Vector3<T>(y, x, w); }
	inline Vector3<T> getYYX() const { return Vector3<T>(y, y, x); }
	inline Vector3<T> getYYY() const { return Vector3<T>(y, y, y); }
	inline Vector3<T> getYYZ() const { return Vector3<T>(y, y, z); }
	inline Vector3<T> getYYW() const { return Vector3<T>(y, y, w); }
	inline Vector3<T> getYZX() const { return Vector3<T>(y, z, x); }
	inline Vector3<T> getYZY() const { return Vector3<T>(y, z, y); }
	inline Vector3<T> getYZZ() const { return Vector3<T>(y, z, z); }
	inline Vector3<T> getYZW() const { return Vector3<T>(y, z, w); }
	inline Vector3<T> getYWX() const { return Vector3<T>(y, w, x); }
	inline Vector3<T> getYWY() const { return Vector3<T>(y, w, y); }
	inline Vector3<T> getYWZ() const { return Vector3<T>(y, w, z); }
	inline Vector3<T> getYWW() const { return Vector3<T>(y, w, w); }
	
	inline Vector3<T> getZXX() const { return Vector3<T>(z, x, x); }
	inline Vector3<T> getZXY() const { return Vector3<T>(z, x, y); }
	inline Vector3<T> getZXZ() const { return Vector3<T>(z, x, z); }
	inline Vector3<T> getZXW() const { return Vector3<T>(z, x, w); }
	inline Vector3<T> getZYX() const { return Vector3<T>(z, y, x); }
	inline Vector3<T> getZYY() const { return Vector3<T>(z, y, y); }
	inline Vector3<T> getZYZ() const { return Vector3<T>(z, y, z); }
	inline Vector3<T> getZYW() const { return Vector3<T>(z, y, w); }
	inline Vector3<T> getZZX() const { return Vector3<T>(z, z, x); }
	inline Vector3<T> getZZY() const { return Vector3<T>(z, z, y); }
	inline Vector3<T> getZZZ() const { return Vector3<T>(z, z, z); }
	inline Vector3<T> getZZW() const { return Vector3<T>(z, z, w); }
	inline Vector3<T> getZWX() const { return Vector3<T>(z, w, x); }
	inline Vector3<T> getZWY() const { return Vector3<T>(z, w, y); }
	inline Vector3<T> getZWZ() const { return Vector3<T>(z, w, z); }
	inline Vector3<T> getZWW() const { return Vector3<T>(z, w, w); }
	
	inline Vector3<T> getWXX() const { return Vector3<T>(w, x, x); }
	inline Vector3<T> getWXY() const { return Vector3<T>(w, x, y); }
	inline Vector3<T> getWXZ() const { return Vector3<T>(w, x, z); }
	inline Vector3<T> getWXW() const { return Vector3<T>(w, x, w); }
	inline Vector3<T> getWYX() const { return Vector3<T>(w, y, x); }
	inline Vector3<T> getWYY() const { return Vector3<T>(w, y, y); }
	inline Vector3<T> getWYZ() const { return Vector3<T>(w, y, z); }
	inline Vector3<T> getWYW() const { return Vector3<T>(w, y, w); }
	inline Vector3<T> getWZX() const { return Vector3<T>(w, z, x); }
	inline Vector3<T> getWZY() const { return Vector3<T>(w, z, y); }
	inline Vector3<T> getWZZ() const { return Vector3<T>(w, z, z); }
	inline Vector3<T> getWZW() const { return Vector3<T>(w, z, w); }
	inline Vector3<T> getWWX() const { return Vector3<T>(w, w, x); }
	inline Vector3<T> getWWY() const { return Vector3<T>(w, w, y); }
	inline Vector3<T> getWWZ() const { return Vector3<T>(w, w, z); }
	inline Vector3<T> getWWW() const { return Vector3<T>(w, w, w); }
	
	inline void setXX(const Vector2<T> &v) { x = v.y; }
	inline void setXY(const Vector2<T> &v) { x = v.x; y = v.y; }
	inline void setXZ(const Vector2<T> &v) { x = v.x; z = v.y; }
	inline void setYX(const Vector2<T> &v) { y = v.x; x = v.y; }
	inline void setYY(const Vector2<T> &v) { y = v.y; }
	inline void setYZ(const Vector2<T> &v) { y = v.x; z = v.y; }
	inline void setZX(const Vector2<T> &v) { z = v.x; x = v.y; }
	inline void setZY(const Vector2<T> &v) { z = v.x; y = v.y; }
	inline void setZZ(const Vector2<T> &v) { z = v.y; }
	
	inline void setXYZ(const Vector3<T> &v) { x = v.x; y = v.y; z = v.z; }
 
	inline void set(const T x, const T y, const T z, const T w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	template<typename U>
	inline void rotate(const U angle)
	{
		// TODO: Missing implementation
	}

	template<typename U>
	inline U angle() const
	{
		// TODO: Missing implementation
		return T(0);
	}

	template<typename U>
	inline U angle(const Vector4& v2) const
	{
		// TODO: Missing implementation
		return T(0);
	}

	inline void normalize()
	{
		T len = length();
		if(len > T(0))
		{
			x /= len;
			y /= len;
			z /= len;
			w /= len;
		}
	}

	inline Vector4 normalized() const
	{
		T len = length();
		if(len > T(0))
		{
			return Vector4(x / len, y / len, z / len, w / len);
		}
		return Vector4();
	}

	template<typename U>
	inline U dot(const Vector4& v2) const
	{
		return (x * v2.x) + (y * v2.y) + (z * v2.z) + (w * v2.w);
	}

	inline Vector4 cross(const Vector4& v2) const
	{
		// TODO: Missing implementation
		return Vector4();
	}

	inline T distance(const Vector4& v2) const
	{
		// TODO: Missing implementation
		return T(0);
	}

	inline T magnitude() const
	{
		return sqrtf(x * x + y * y + z * z + w * w);
	}

	inline T length() const
	{
		return sqrtf(x * x + y * y + z * z + w * w);
	}
 
	// Operators
	inline Vector4& operator=(const Vector4& v2)
	{
		if(this == &v2) return *this;
		x = v2.x;
		y = v2.y;
		z = v2.z;
		w = v2.w;
		return *this;
	}

	inline Vector4& operator+=(const Vector4& v2)
	{
		x += v2.x;
		y += v2.y;
		z += v2.z;
		return *this;
	}

	inline Vector4& operator-=(const Vector4& v2)
	{
		x -= v2.x;
		y -= v2.y;
		z -= v2.z;
		w -= v2.w;
		return *this;
	}

	template<typename U>
	inline Vector4& operator*=(const U scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		w *= scalar;
		return *this;
	}

	template<typename U>
	inline Vector4& operator/=(const U scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;
		w /= scalar;
		return *this;
	}

	inline const Vector4 operator+(const Vector4 &v2) const
	{
		return Vector4(*this) += v2;
	}

	inline const Vector4 operator-(const Vector4 &v2) const
	{
		return Vector4(*this) -= v2;
	}

	template<typename U>
	inline const Vector4 operator*(const U scalar) const
	{
		return Vector4(*this) *= scalar;
	}

	template<typename U>
	inline const Vector4 operator/(const U scalar) const
	{
		return Vector4(*this) /= scalar;
	}

	inline bool operator==(const Vector4& v2) const
	{
		return ((x == v2.x) && (y == v2.y)) && (z == v2.z) && (w == v2.w);
	}

    T x, y, z, w;
};

typedef Vector4<bool> Vector4B;
typedef Vector4<float> Vector4F;
typedef Vector4<double> Vector4D;
typedef Vector4<int> Vector4I;
typedef Vector4<uint> Vector4U;
typedef Vector4<uchar> Vector4UB;

class SAUCE_API Color : public Vector4<uchar>
{
public:
	Color();
	Color(const uchar r, const uchar g, const uchar b, const uchar a = 255);

	inline uchar getR() const { return x; }
	inline void setR(const uchar &v) { x = v; }
	inline uchar getG() const { return y; }
	inline void setG(const uchar &v) { y = v; }
	inline uchar getB() const { return z; }
	inline void setB(const uchar &v) { z = v; }
	inline uchar getA() const { return w; }
	inline void setA(const uchar &v) { w = v; }

	static const Color Black;
	static const Color White;
	static const Color Red;
	static const Color Green;
	static const Color Blue;
	static const Color Yellow;
};

#endif // SAUCE_VECTOR_H
