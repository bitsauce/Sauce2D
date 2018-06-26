#include <Sauce/Sauce.h>

using namespace sauce;

class Camera : public SceneObject
{
public:
	Camera();

	void onTick(TickEvent *e);
	void onMouseEvent(MouseEvent *e);
	void onKeyEvent(KeyEvent *e);

	void setPosition(const Vector3F value) { m_position = value; }
	void setYaw(const float value) { m_yaw = value; }
	void setPitch(const float value) { m_pitch = value; }
	void setMoveSpeed(const float value) { m_moveSpeed = value; }
	void setRotationSpeed(const float value) { m_rotationSpeed = value; }

	Vector3F getForwardVector() const;
	Vector3F getRightVector() const;
	Vector3F getUpVector() const;

	Vector3F getPosition() const { return m_position; }
	float getYaw() const { return m_yaw; }
	float getPitch() const { return m_pitch; }

private:
	Vector3F m_position;
	float m_yaw, m_pitch;

	float m_rotationSpeed;
	float m_moveSpeed;

	Vector2F m_previousMousePosition;
	bool m_dragging;

	struct
	{
		bool up : 1;
		bool down : 1;
		bool left : 1;
		bool right : 1;
		bool forward : 1;
		bool backward : 1;
	} m_inputState;
};