#include <Sauce/Math/Vector.h>

const Vector2F Vector2F::Zero;
const Vector2B Vector2B::Zero;
const Vector2D Vector2D::Zero;
const Vector2I Vector2I::Zero;
const Vector2U Vector2U::Zero;
const Vector2UB Vector2UB::Zero;

const Color Color::White = Color(255, 255, 255);
const Color Color::Black = Color(0, 0, 0);
const Color Color::Red = Color(255, 0, 0);
const Color Color::Green = Color(0, 255, 0);
const Color Color::Blue = Color(0, 0, 255);
const Color Color::Yellow = Color(255, 255, 0);

Color::Color() :
	Color(0, 0, 0, 0)
{
}

Color::Color(const uchar r, const uchar g, const uchar b, const uchar a) :
	Vector4<uchar>(r, g, b, a)
{
}