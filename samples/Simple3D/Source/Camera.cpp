#include "Camera.h"

Camera::Camera() :
	m_position(0.f, 0.f, 0.f),
	m_previousPosition(0.f, 0.f, 0.f),
	m_yaw(0.f),
	m_pitch(0.f),
	m_rotationSpeed(0.01f),
	m_moveSpeed(3.0f),
	m_dragging(false)
{
}

void Camera::onTick(TickEvent *e)
{
	// Update camera position
	int moveFwd   = (m_inputState.forward - m_inputState.backward);
	int moveRight = (m_inputState.left - m_inputState.right);
	int moveUp    = (m_inputState.up - m_inputState.down);
	m_previousPosition = m_position;
	m_position += getForwardVector() * moveFwd   * e->getDelta() * m_moveSpeed;
	m_position += getRightVector()   * moveRight * e->getDelta() * m_moveSpeed;
	m_position += getUpVector()      * moveUp    * e->getDelta() * m_moveSpeed;
}

float wrapAngle(float angle) {
	while(angle >= PI * 2) angle -= PI * 2;
	while(angle < 0) angle += PI * 2;
	return angle;
}

void Camera::onMouseEvent(MouseEvent *e)
{
	switch(e->getType())
	{
		case EVENT_MOUSE_DOWN:
		{
			m_dragging = true;
			m_previousMousePosition = e->getPosition();
		}
		break;

		case EVENT_MOUSE_UP:
		{
			m_dragging = false;
		}
		break;

		case EVENT_MOUSE_MOVE:
		{
			if(m_dragging)
			{
				// Update camera rotation
				const Vector2F mousePosition = e->getPosition();
				const Vector2F dt = mousePosition - m_previousMousePosition;
				m_yaw   = wrapAngle(m_yaw   + dt.x * m_rotationSpeed);
				m_pitch = wrapAngle(m_pitch - dt.y * m_rotationSpeed);
				m_previousMousePosition = mousePosition;
			}
		}
		break;
	}
}

void Camera::onKeyEvent(KeyEvent *e)
{
	const bool pressed = e->getType() != EVENT_KEY_UP;
	switch(e->getKeycode())
	{
		case SAUCE_KEY_W: m_inputState.forward = pressed; break;
		case SAUCE_KEY_S: m_inputState.backward = pressed; break;
		case SAUCE_KEY_A: m_inputState.left = pressed; break;
		case SAUCE_KEY_D: m_inputState.right = pressed; break;
		case SAUCE_KEY_LSHIFT: m_inputState.down = pressed; break;
		case SAUCE_KEY_SPACE: m_inputState.up = pressed; break;
	}
}

Vector3F Camera::getForwardVector() const
{
	Vector3F fwd;
	fwd.x = cos(m_pitch) * cos(m_yaw);
	fwd.y = sin(m_pitch);
	fwd.z = cos(m_pitch) * sin(m_yaw);
	return fwd;
}

Vector3F Camera::getRightVector() const
{
	const Vector3F worldUp(0.0f, 1.0f, 0.0f);
	return math::normalize(math::cross(worldUp, getForwardVector()));
}

Vector3F Camera::getUpVector() const
{
	return math::cross(getForwardVector(), getRightVector());
}
