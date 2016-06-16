#ifndef SAUCE_MATRIX_H
#define SAUCE_MATRIX_H

#include <Sauce/Config.h>
#include <Sauce/Math/Vector.h>

/**
 * \class	Matrix2
 *
 * \brief	2x2 Matrix
 */

class SAUCE_API Matrix2
{
public:
	inline Matrix2()
	{
		identity();
	}

    inline Matrix2(const float src[4])
	{
		set(src);
	}

	inline Matrix2(float xx, float xy, float yx, float yy)
	{
		set(xx, xy, yx, yy);
	}

    inline void set(const float src[4])
	{
		m[0] = src[0]; m[1] = src[1]; m[2] = src[2]; m[3] = src[3];
	}

	inline void set(float xx, float xy, float yx, float yy)
	{
		m[0] = xx; m[1] = xy; m[2] = yx; m[3] = yy;
	}

	inline void setRow(int index, const float row[2])
	{
		m[index * 2] = row[0]; m[index * 2 + 1] = row[1];
	}

	inline void setRow(int index, const Vector2F &v)
	{
		m[index * 2] = v.x; m[index * 2 + 1] = v.y;
	}

	inline void setColumn(int index, const float col[2])
	{
		m[index] = col[0]; m[index + 2] = col[1];
	}

	inline void setColumn(int index, const Vector2F &v)
	{
		m[index] = v.x; m[index + 2] = v.y;
	}

	inline const float* get() const
	{
		return m;
	}

	inline float getDeterminant()
	{
		return m[0] * m[3] - m[1] * m[2];
	}

    inline Matrix2 &identity()
	{
		m[0] = m[3] = 1.0f;
		m[1] = m[2] = 0.0f;
		return *this;
	}

	inline Matrix2 &transpose()
	{
		float tmp = m[1];
		m[1] = m[2];
		m[2] = tmp;
		return *this;
	}

	inline Matrix2 &invert()
	{
		float determinant = m[0] * m[3] - m[1] * m[2];
		if(fabs(determinant) <= 0.00001f)
		{
			return identity();
		}

		float tmp = m[0];   // copy the first element
		float invDeterminant = 1.0f / determinant;
		m[0] = invDeterminant * m[3];
		m[1] = -invDeterminant * m[1];
		m[2] = -invDeterminant * m[2];
		m[3] = invDeterminant * tmp;

		return *this;
	}

    // Operators
	inline Matrix2 operator+(const Matrix2& rhs) const
	{
		return Matrix2(m[0] + rhs[0], m[1] + rhs[1], m[2] + rhs[2], m[3] + rhs[3]);
	}

	inline Matrix2 operator-(const Matrix2& rhs) const
	{
		return Matrix2(m[0] - rhs[0], m[1] - rhs[1], m[2] - rhs[2], m[3] - rhs[3]);
	}

	inline Matrix2 &operator+=(const Matrix2& rhs)
	{
		m[0] += rhs[0];  m[1] += rhs[1];  m[2] += rhs[2];  m[3] += rhs[3];
		return *this;
	}
	
	inline Matrix2 &operator-=(const Matrix2& rhs)
	{
		m[0] -= rhs[0];  m[1] -= rhs[1];  m[2] -= rhs[2];  m[3] -= rhs[3];
		return *this;
	}

	inline Vector2F operator*(const Vector2F &rhs) const
	{
		return Vector2F(m[0] * rhs.x + m[1] * rhs.y, m[2] * rhs.x + m[3] * rhs.y);
	}

	inline Matrix2 operator*(const Matrix2& rhs) const
	{
		return Matrix2(m[0] * rhs[0] + m[1] * rhs[2], m[0] * rhs[1] + m[1] * rhs[3],
			m[2] * rhs[0] + m[3] * rhs[2], m[2] * rhs[1] + m[3] * rhs[3]);
	}

	inline Matrix2 &operator*=(const Matrix2& rhs)
	{
		*this = *this * rhs;
		return *this;
	}

	inline bool operator==(const Matrix2& rhs) const
	{
		return (m[0] == rhs[0]) && (m[1] == rhs[1]) && (m[2] == rhs[2]) && (m[3] == rhs[3]);
	}

	inline bool operator!=(const Matrix2& rhs) const
	{
		return (m[0] != rhs[0]) || (m[1] != rhs[1]) || (m[2] != rhs[2]) || (m[3] != rhs[3]);
	}

	inline float operator[](int index) const
	{
		return m[index];
	}

	inline float &operator[](int index)
	{
		return m[index];
	}

    inline friend Matrix2 operator-(const Matrix2& rhs)
	{
		return Matrix2(-rhs[0], -rhs[1], -rhs[2], -rhs[3]);
	}

	inline friend Matrix2 operator*(float scalar, const Matrix2& rhs)
	{
		return Matrix2(scalar * rhs[0], scalar * rhs[1], scalar * rhs[2], scalar * rhs[3]);
	}

	inline friend Vector2F operator*(const Vector2F &vec, const Matrix2& rhs)
	{
		return Vector2F(vec.x * rhs[0] + vec.y * rhs[2], vec.x * rhs[1] + vec.y * rhs[3]);
	}

	inline friend ostream& operator<<(ostream& os, const Matrix2& m)
	{
		os << "(" << m[0] << ",\t" << m[1] << ")\n"
			<< "(" << m[2] << ",\t" << m[3] << ")\n";
		return os;
	}

private:
    float m[4];
};

/*********************************************************************
**	3x3 Matrix														**
**********************************************************************/
class SAUCE_API Matrix3
{
public:
    // constructors
    Matrix3();  // init with identity
    Matrix3(const float src[9]);
    Matrix3(float xx, float xy, float xz,
            float yx, float yy, float yz,
            float zx, float zy, float zz);

    void        set(const float src[9]);
    void        set(float xx, float xy, float xz,
                    float yx, float yy, float yz,
                    float zx, float zy, float zz);
    void        setRow(int index, const float row[3]);
    void        setRow(int index, const Vector3<float>& v);
    void        setColumn(int index, const float col[3]);
    void        setColumn(int index, const Vector3<float>& v);

    const float* get() const;
    float        getDeterminant();

    Matrix3&    identity();
    Matrix3&    transpose();                            // transpose itself and return reference
    Matrix3&    invert();

    // operators
    Matrix3     operator+(const Matrix3& rhs) const;
    Matrix3     operator-(const Matrix3& rhs) const;
    Matrix3&    operator+=(const Matrix3& rhs);
    Matrix3&    operator-=(const Matrix3& rhs);
	Vector3<float>     operator*(const Vector3<float>& rhs) const;
    Matrix3     operator*(const Matrix3& rhs) const;
    Matrix3&    operator*=(const Matrix3& rhs);
    bool        operator==(const Matrix3& rhs) const;
    bool        operator!=(const Matrix3& rhs) const;
    float       operator[](int index) const;
    float&      operator[](int index);

    friend Matrix3 operator-(const Matrix3& m);
    friend Matrix3 operator*(float scalar, const Matrix3& m);
    friend Vector3<float> operator*(const Vector3<float>& vec, const Matrix3& m);
    friend std::ostream& operator<<(std::ostream& os, const Matrix3& m);

private:
    float m[9];
};

inline Matrix3::Matrix3()
{
    // initially identity matrix
    identity();
}

inline Matrix3::Matrix3(const float src[9])
{
    set(src);
}

inline Matrix3::Matrix3(float xx, float xy, float xz,
                        float yx, float yy, float yz,
                        float zx, float zy, float zz)
{
    set(xx, xy, xz,  yx, yy, yz,  zx, zy, zz);
}

inline void Matrix3::set(const float src[9])
{
    m[0] = src[0];  m[1] = src[1];  m[2] = src[2];
    m[3] = src[3];  m[4] = src[4];  m[5] = src[5];
    m[6] = src[6];  m[7] = src[7];  m[8] = src[8];
}

inline void Matrix3::set(float xx, float xy, float xz,
                         float yx, float yy, float yz,
                         float zx, float zy, float zz)
{
    m[0] = xx;  m[1] = xy;  m[2] = xz;
    m[3] = yx;  m[4] = yy;  m[5] = yz;
    m[6] = zx;  m[7] = zy;  m[8] = zz;
}

inline void Matrix3::setRow(int index, const float row[3])
{
    m[index*3] = row[0];  m[index*3 + 1] = row[1];  m[index*3 + 2] = row[2];
}

inline void Matrix3::setRow(int index, const Vector3<float>& v)
{
    m[index*3] = v.x;  m[index*3 + 1] = v.y;  m[index*3 + 2] = v.z;
}

inline void Matrix3::setColumn(int index, const float col[3])
{
    m[index] = col[0];  m[index + 3] = col[1];  m[index + 6] = col[2];
}

inline void Matrix3::setColumn(int index, const Vector3<float>& v)
{
    m[index] = v.x;  m[index + 3] = v.y;  m[index + 6] = v.z;
}

inline const float* Matrix3::get() const
{
    return m;
}

inline Matrix3& Matrix3::identity()
{
    m[0] = m[4] = m[8] = 1.0f;
    m[1] = m[2] = m[3] = m[5] = m[6] = m[7] = 0.0f;
    return *this;
}

inline Matrix3 Matrix3::operator+(const Matrix3& rhs) const
{
    return Matrix3(m[0]+rhs[0], m[1]+rhs[1], m[2]+rhs[2],
                   m[3]+rhs[3], m[4]+rhs[4], m[5]+rhs[5],
                   m[6]+rhs[6], m[7]+rhs[7], m[8]+rhs[8]);
}

inline Matrix3 Matrix3::operator-(const Matrix3& rhs) const
{
    return Matrix3(m[0]-rhs[0], m[1]-rhs[1], m[2]-rhs[2],
                   m[3]-rhs[3], m[4]-rhs[4], m[5]-rhs[5],
                   m[6]-rhs[6], m[7]-rhs[7], m[8]-rhs[8]);
}

inline Matrix3& Matrix3::operator+=(const Matrix3& rhs)
{
    m[0] += rhs[0];  m[1] += rhs[1];  m[2] += rhs[2];
    m[3] += rhs[3];  m[4] += rhs[4];  m[5] += rhs[5];
    m[6] += rhs[6];  m[7] += rhs[7];  m[8] += rhs[8];
    return *this;
}

inline Matrix3& Matrix3::operator-=(const Matrix3& rhs)
{
    m[0] -= rhs[0];  m[1] -= rhs[1];  m[2] -= rhs[2];
    m[3] -= rhs[3];  m[4] -= rhs[4];  m[5] -= rhs[5];
    m[6] -= rhs[6];  m[7] -= rhs[7];  m[8] -= rhs[8];
    return *this;
}

inline Vector3<float> Matrix3::operator*(const Vector3<float>& rhs) const
{
    return Vector3<float>(m[0]*rhs.x + m[1]*rhs.y + m[2]*rhs.z,
                   m[3]*rhs.x + m[4]*rhs.y + m[5]*rhs.z,
                   m[6]*rhs.x + m[7]*rhs.y + m[8]*rhs.z);
}

inline Matrix3 Matrix3::operator*(const Matrix3& rhs) const
{
    return Matrix3(m[0]*rhs[0] + m[1]*rhs[3] + m[2]*rhs[6],  m[0]*rhs[1] + m[1]*rhs[4] + m[2]*rhs[7],  m[0]*rhs[2] + m[1]*rhs[5] + m[2]*rhs[8],
                   m[3]*rhs[0] + m[4]*rhs[3] + m[5]*rhs[6],  m[3]*rhs[1] + m[4]*rhs[4] + m[5]*rhs[7],  m[3]*rhs[2] + m[4]*rhs[5] + m[5]*rhs[8],
                   m[6]*rhs[0] + m[7]*rhs[3] + m[8]*rhs[6],  m[6]*rhs[1] + m[7]*rhs[4] + m[8]*rhs[7],  m[6]*rhs[2] + m[7]*rhs[5] + m[8]*rhs[8]);
}

inline Matrix3& Matrix3::operator*=(const Matrix3& rhs)
{
    *this = *this * rhs;
    return *this;
}

inline bool Matrix3::operator==(const Matrix3& rhs) const
{
    return (m[0] == rhs[0]) && (m[1] == rhs[1]) && (m[2] == rhs[2]) &&
           (m[3] == rhs[3]) && (m[4] == rhs[4]) && (m[5] == rhs[5]) &&
           (m[6] == rhs[6]) && (m[7] == rhs[7]) && (m[8] == rhs[8]);
}

inline bool Matrix3::operator!=(const Matrix3& rhs) const
{
    return (m[0] != rhs[0]) || (m[1] != rhs[1]) || (m[2] != rhs[2]) ||
           (m[3] != rhs[3]) || (m[4] != rhs[4]) || (m[5] != rhs[5]) ||
           (m[6] != rhs[6]) || (m[7] != rhs[7]) || (m[8] != rhs[8]);
}

inline float Matrix3::operator[](int index) const
{
    return m[index];
}

inline float& Matrix3::operator[](int index)
{
    return m[index];
}

inline Matrix3 operator-(const Matrix3& rhs)
{
    return Matrix3(-rhs[0], -rhs[1], -rhs[2], -rhs[3], -rhs[4], -rhs[5], -rhs[6], -rhs[7], -rhs[8]);
}

inline Matrix3 operator*(float s, const Matrix3& rhs)
{
    return Matrix3(s*rhs[0], s*rhs[1], s*rhs[2], s*rhs[3], s*rhs[4], s*rhs[5], s*rhs[6], s*rhs[7], s*rhs[8]);
}

inline Vector3<float> operator*(const Vector3<float>& v, const Matrix3& m)
{
    return Vector3<float>(v.x*m[0] + v.y*m[3] + v.z*m[6],  v.x*m[1] + v.y*m[4] + v.z*m[7],  v.x*m[2] + v.y*m[5] + v.z*m[8]);
}


inline std::ostream& operator<<(std::ostream& os, const Matrix3& m)
{
    os << "(" << m[0] << ",\t" << m[1] << ",\t" << m[2] << ")\n"
       << "(" << m[3] << ",\t" << m[4] << ",\t" << m[5] << ")\n"
       << "(" << m[6] << ",\t" << m[7] << ",\t" << m[8] << ")\n";
    return os;
}

/*********************************************************************
**	4x4 Matrix														**
**********************************************************************/
class SAUCE_API Matrix4
{
public:
    // constructors
    Matrix4();  // init with identity
    Matrix4(const float src[16]);
    Matrix4(float xx, float xy, float xz, float xw,
            float yx, float yy, float yz, float yw,
            float zx, float zy, float zz, float zw,
            float wx, float wy, float wz, float ww);

    void        set(const float src[16]);
    void        set(float xx, float xy, float xz, float xw,
                    float yx, float yy, float yz, float yw,
                    float zx, float zy, float zz, float zw,
                    float wx, float wy, float wz, float ww);
    void        setRow(int index, const float row[4]);
    void        setRow(int index, const Vector4<float>& v);
    void        setRow(int index, const Vector3<float> &v);
    void        setColumn(int index, const float col[4]);
    void        setColumn(int index, const Vector4<float>& v);
    void        setColumn(int index, const Vector3<float> &v);

    const float* get() const;
    const float* getTranspose();                        // return transposed matrix
    float        getDeterminant();

	Vector4<float> getRow(const int row) const
	{
		return Vector4<float>(&m[row*4]);
	}

	Vector4<float> getColumn(const int col) const
	{
		return Vector4<float>(m[col], m[3+col], m[7+col], m[11+col]);
	}

    Matrix4&    identity();
    Matrix4&    transpose();
    Matrix4&    invert();
    Matrix4&    invertEuclidean();
    Matrix4&    invertAffine();
    Matrix4&    invertProjective();
    Matrix4&    invertGeneral();
    Matrix4&    translate(float x, float y, float z);
    Matrix4&    translate(const Vector3<float> &v);
    Matrix4&    rotate(float angle, const Vector3<float> &axis);
    Matrix4&    rotate(float angle, float x, float y, float z);
    Matrix4&    rotateX(float angle);
    Matrix4&    rotateY(float angle);
    Matrix4&    rotateZ(float angle);
    Matrix4&    scale(float scale);
    Matrix4&    scale(float sx, float sy, float sz);
    Matrix4     operator+(const Matrix4& rhs) const;
    Matrix4     operator-(const Matrix4& rhs) const;
    Matrix4&    operator+=(const Matrix4& rhs);
    Matrix4&    operator-=(const Matrix4& rhs);
    Vector4<float>     operator*(const Vector4<float>& rhs) const;
    Vector3<float>     operator*(const Vector3<float> &rhs) const;
    Matrix4     operator*(const Matrix4& rhs) const;
    Matrix4&    operator*=(const Matrix4& rhs);
    bool        operator==(const Matrix4& rhs) const;
    bool        operator!=(const Matrix4& rhs) const;
    float       operator[](int index) const;
    float&      operator[](int index);

    friend Matrix4 operator-(const Matrix4& m);
    friend Matrix4 operator*(float scalar, const Matrix4& m);
    friend Vector3<float> operator*(const Vector3<float> &vec, const Matrix4& m);
    friend Vector4<float> operator*(const Vector4<float>& vec, const Matrix4& m);
    friend std::ostream& operator<<(std::ostream& os, const Matrix4& m);

private:
    float       getCofactor(float m0, float m1, float m2,
                            float m3, float m4, float m5,
                            float m6, float m7, float m8);

    float m[16];
    float tm[16];                                       // transpose m
};

inline Matrix4::Matrix4()
{
    // initially identity matrix
    identity();
}

inline Matrix4::Matrix4(const float src[16])
{
    set(src);
}

inline Matrix4::Matrix4(float xx, float xy, float xz, float xw,
                        float yx, float yy, float yz, float yw,
                        float zx, float zy, float zz, float zw,
                        float wx, float wy, float wz, float ww)
{
    set(xx, xy, xz, xw,  yx, yy, yz, yw,  zx, zy, zz, zw,  wx, wy, wz, ww);
}

inline void Matrix4::set(const float src[16])
{
    m[0] = src[0];  m[1] = src[1];  m[2] = src[2];  m[3] = src[3];
    m[4] = src[4];  m[5] = src[5];  m[6] = src[6];  m[7] = src[7];
    m[8] = src[8];  m[9] = src[9];  m[10]= src[10]; m[11]= src[11];
    m[12]= src[12]; m[13]= src[13]; m[14]= src[14]; m[15]= src[15];
}

inline void Matrix4::set(float xx, float xy, float xz, float xw,
                         float yx, float yy, float yz, float yw,
                         float zx, float zy, float zz, float zw,
                         float wx, float wy, float wz, float ww)
{
    m[0] = xx;  m[1] = xy;  m[2] = xz;  m[3] = xw;
    m[4] = yx;  m[5] = yy;  m[6] = yz;  m[7] = yw;
    m[8] = zx;  m[9] = zy;  m[10]= zz;  m[11]= zw;
    m[12]= wx;  m[13]= wy;  m[14]= wz;  m[15]= ww;
}

inline void Matrix4::setRow(int index, const float row[4])
{
    m[index*4] = row[0];  m[index*4 + 1] = row[1];  m[index*4 + 2] = row[2];  m[index*4 + 3] = row[3];
}

inline void Matrix4::setRow(int index, const Vector4<float>& v)
{
    m[index*4] = v.x;  m[index*4 + 1] = v.y;  m[index*4 + 2] = v.z;  m[index*4 + 3] = v.w;
}

inline void Matrix4::setRow(int index, const Vector3<float> &v)
{
    m[index*4] = v.x;  m[index*4 + 1] = v.y;  m[index*4 + 2] = v.z;
}

inline void Matrix4::setColumn(int index, const float col[4])
{
    m[index] = col[0];  m[index + 4] = col[1];  m[index + 8] = col[2];  m[index + 12] = col[3];
}

inline void Matrix4::setColumn(int index, const Vector4<float>& v)
{
    m[index] = v.x;  m[index + 4] = v.y;  m[index + 8] = v.z;  m[index + 12] = v.w;
}

inline void Matrix4::setColumn(int index, const Vector3<float> &v)
{
    m[index] = v.x;  m[index + 4] = v.y;  m[index + 8] = v.z;
}

inline const float* Matrix4::get() const
{
    return m;
}

inline const float* Matrix4::getTranspose()
{
    tm[0] = m[0];   tm[1] = m[4];   tm[2] = m[8];   tm[3] = m[12];
    tm[4] = m[1];   tm[5] = m[5];   tm[6] = m[9];   tm[7] = m[13];
    tm[8] = m[2];   tm[9] = m[6];   tm[10]= m[10];  tm[11]= m[14];
    tm[12]= m[3];   tm[13]= m[7];   tm[14]= m[11];  tm[15]= m[15];
    return tm;
}

inline Matrix4& Matrix4::identity()
{
    m[0] = m[5] = m[10] = m[15] = 1.0f;
    m[1] = m[2] = m[3] = m[4] = m[6] = m[7] = m[8] = m[9] = m[11] = m[12] = m[13] = m[14] = 0.0f;
    return *this;
}

inline Matrix4 Matrix4::operator+(const Matrix4& rhs) const
{
    return Matrix4(m[0]+rhs[0],   m[1]+rhs[1],   m[2]+rhs[2],   m[3]+rhs[3],
                   m[4]+rhs[4],   m[5]+rhs[5],   m[6]+rhs[6],   m[7]+rhs[7],
                   m[8]+rhs[8],   m[9]+rhs[9],   m[10]+rhs[10], m[11]+rhs[11],
                   m[12]+rhs[12], m[13]+rhs[13], m[14]+rhs[14], m[15]+rhs[15]);
}

inline Matrix4 Matrix4::operator-(const Matrix4& rhs) const
{
    return Matrix4(m[0]-rhs[0],   m[1]-rhs[1],   m[2]-rhs[2],   m[3]-rhs[3],
                   m[4]-rhs[4],   m[5]-rhs[5],   m[6]-rhs[6],   m[7]-rhs[7],
                   m[8]-rhs[8],   m[9]-rhs[9],   m[10]-rhs[10], m[11]-rhs[11],
                   m[12]-rhs[12], m[13]-rhs[13], m[14]-rhs[14], m[15]-rhs[15]);
}

inline Matrix4& Matrix4::operator+=(const Matrix4& rhs)
{
    m[0] += rhs[0];    m[1] += rhs[1];    m[2] += rhs[2];    m[3] += rhs[3];
    m[4] += rhs[4];    m[5] += rhs[5];    m[6] += rhs[6];    m[7] += rhs[7];
    m[8] += rhs[8];    m[9] += rhs[9];    m[10] += rhs[10];  m[11] += rhs[11];
    m[12] += rhs[12];  m[13] += rhs[13];  m[14] += rhs[14];  m[15] += rhs[15];
    return *this;
}

inline Matrix4& Matrix4::operator-=(const Matrix4& rhs)
{
    m[0] -= rhs[0];    m[1] -= rhs[1];    m[2] -= rhs[2];    m[3] -= rhs[3];
    m[4] -= rhs[4];    m[5] -= rhs[5];    m[6] -= rhs[6];    m[7] -= rhs[7];
    m[8] -= rhs[8];    m[9] -= rhs[9];    m[10] -= rhs[10];  m[11] -= rhs[11];
    m[12] -= rhs[12];  m[13] -= rhs[13];  m[14] -= rhs[14];  m[15] -= rhs[15];
    return *this;
}

inline Vector4<float> Matrix4::operator*(const Vector4<float>& rhs) const
{
    return Vector4<float>(m[0]*rhs.x  + m[1]*rhs.y  + m[2]*rhs.z  + m[3]*rhs.w,
                   m[4]*rhs.x  + m[5]*rhs.y  + m[6]*rhs.z  + m[7]*rhs.w,
                   m[8]*rhs.x  + m[9]*rhs.y  + m[10]*rhs.z + m[11]*rhs.w,
                   m[12]*rhs.x + m[13]*rhs.y + m[14]*rhs.z + m[15]*rhs.w);
}

inline Vector3<float> Matrix4::operator*(const Vector3<float> &rhs) const
{
    return Vector3<float>(m[0]*rhs.x + m[1]*rhs.y + m[2]*rhs.z,
                   m[4]*rhs.x + m[5]*rhs.y + m[6]*rhs.z,
                   m[8]*rhs.x + m[9]*rhs.y + m[10]*rhs.z);
}

inline Matrix4 Matrix4::operator*(const Matrix4& n) const
{
    return Matrix4(m[0]*n[0]  + m[1]*n[4]  + m[2]*n[8]  + m[3]*n[12],   m[0]*n[1]  + m[1]*n[5]  + m[2]*n[9]  + m[3]*n[13],   m[0]*n[2]  + m[1]*n[6]  + m[2]*n[10]  + m[3]*n[14],   m[0]*n[3]  + m[1]*n[7]  + m[2]*n[11]  + m[3]*n[15],
                   m[4]*n[0]  + m[5]*n[4]  + m[6]*n[8]  + m[7]*n[12],   m[4]*n[1]  + m[5]*n[5]  + m[6]*n[9]  + m[7]*n[13],   m[4]*n[2]  + m[5]*n[6]  + m[6]*n[10]  + m[7]*n[14],   m[4]*n[3]  + m[5]*n[7]  + m[6]*n[11]  + m[7]*n[15],
                   m[8]*n[0]  + m[9]*n[4]  + m[10]*n[8] + m[11]*n[12],  m[8]*n[1]  + m[9]*n[5]  + m[10]*n[9] + m[11]*n[13],  m[8]*n[2]  + m[9]*n[6]  + m[10]*n[10] + m[11]*n[14],  m[8]*n[3]  + m[9]*n[7]  + m[10]*n[11] + m[11]*n[15],
                   m[12]*n[0] + m[13]*n[4] + m[14]*n[8] + m[15]*n[12],  m[12]*n[1] + m[13]*n[5] + m[14]*n[9] + m[15]*n[13],  m[12]*n[2] + m[13]*n[6] + m[14]*n[10] + m[15]*n[14],  m[12]*n[3] + m[13]*n[7] + m[14]*n[11] + m[15]*n[15]);
}

inline Matrix4& Matrix4::operator*=(const Matrix4& rhs)
{
    *this = *this * rhs;
    return *this;
}

inline bool Matrix4::operator==(const Matrix4& n) const
{
    return (m[0] == n[0])   && (m[1] == n[1])   && (m[2] == n[2])   && (m[3] == n[3]) &&
           (m[4] == n[4])   && (m[5] == n[5])   && (m[6] == n[6])   && (m[7] == n[7]) &&
           (m[8] == n[8])   && (m[9] == n[9])   && (m[10] == n[10]) && (m[11] == n[11]) &&
           (m[12] == n[12]) && (m[13] == n[13]) && (m[14] == n[14]) && (m[15] == n[15]);
}

inline bool Matrix4::operator!=(const Matrix4& n) const
{
    return (m[0] != n[0])   || (m[1] != n[1])   || (m[2] != n[2])   || (m[3] != n[3]) ||
           (m[4] != n[4])   || (m[5] != n[5])   || (m[6] != n[6])   || (m[7] != n[7]) ||
           (m[8] != n[8])   || (m[9] != n[9])   || (m[10] != n[10]) || (m[11] != n[11]) ||
           (m[12] != n[12]) || (m[13] != n[13]) || (m[14] != n[14]) || (m[15] != n[15]);
}

inline float Matrix4::operator[](int index) const
{
    return m[index];
}

inline float& Matrix4::operator[](int index)
{
    return m[index];
}

inline Matrix4 operator-(const Matrix4& rhs)
{
    return Matrix4(-rhs[0], -rhs[1], -rhs[2], -rhs[3], -rhs[4], -rhs[5], -rhs[6], -rhs[7], -rhs[8], -rhs[9], -rhs[10], -rhs[11], -rhs[12], -rhs[13], -rhs[14], -rhs[15]);
}

inline Matrix4 operator*(float s, const Matrix4& rhs)
{
    return Matrix4(s*rhs[0], s*rhs[1], s*rhs[2], s*rhs[3], s*rhs[4], s*rhs[5], s*rhs[6], s*rhs[7], s*rhs[8], s*rhs[9], s*rhs[10], s*rhs[11], s*rhs[12], s*rhs[13], s*rhs[14], s*rhs[15]);
}

inline Vector4<float> operator*(const Vector4<float>& v, const Matrix4& m)
{
    return Vector4<float>(v.x*m[0] + v.y*m[4] + v.z*m[8] + v.w*m[12],  v.x*m[1] + v.y*m[5] + v.z*m[9] + v.w*m[13],  v.x*m[2] + v.y*m[6] + v.z*m[10] + v.w*m[14], v.x*m[3] + v.y*m[7] + v.z*m[11] + v.w*m[15]);
}

inline Vector3<float> operator*(const Vector3<float> &v, const Matrix4& m)
{
    return Vector3<float>(v.x*m[0] + v.y*m[4] + v.z*m[8],  v.x*m[1] + v.y*m[5] + v.z*m[9],  v.x*m[2] + v.y*m[6] + v.z*m[10]);
}

inline std::ostream& operator<<(std::ostream& os, const Matrix4& m)
{
    os << "(" << m[0]  << ",\t" << m[1]  << ",\t" << m[2]  <<  ",\t" << m[3] << ")\n"
       << "(" << m[4]  << ",\t" << m[5]  << ",\t" << m[6]  <<  ",\t" << m[7] << ")\n"
       << "(" << m[8]  << ",\t" << m[9]  << ",\t" << m[10] <<  ",\t" << m[11] << ")\n"
       << "(" << m[12] << ",\t" << m[13] << ",\t" << m[14] <<  ",\t" << m[15] << ")\n";
    return os;
}

#endif // SAUCE_MATRIX_H
