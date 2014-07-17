#ifndef GFX_CAMERA_H
#define GFX_CAMERA_H

#include <x2d/math.h>

// NOTE: Camera == Transform == Matrix4

class Camera
{
public:
	Camera();

	Matrix4 getProjectionMatix();

private:
	Vector2 m_position;
	Vector2 m_size;
	float m_rotation;
};

#endif // GFX_CAMERA_H