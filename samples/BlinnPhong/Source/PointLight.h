#include <Sauce/Sauce.h>

using namespace sauce;

class PointLight : public SceneObject
{
public:
	PointLight();

	void setPosition(const Vector3F value) { m_position = value; }
	Vector3F getPosition() const { return m_position; }

private:
	Vector3F m_position;
};