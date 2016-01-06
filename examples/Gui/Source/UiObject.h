#ifndef UI_OBJECT_H
#define UI_OBJECT_H

#include "Config.h"
#include "UiEvents.h"

// TODO: Need absoulute pixel positioning and aspect ratio
class UiObject : public SceneObject
{
public:
	UiObject();
	virtual ~UiObject();

	// Mouse hover
	virtual void onHover(HoverEvent*) { }
	bool isHovered() const;

	// Active state
	virtual void onFocus(FocusEvent*) { }
	bool isFocused() const;

	// Click event
	virtual void onClick(ClickEvent*) { }
	bool isPressed() const;

	// Set/get position
	void setPosition(const Vector2 &position);
	Vector2 getPosition() const;

	// Set/get size
	void setSize(const Vector2 &size);
	void setSize(const float width, const float height);
	virtual Vector2 getSize() const;

	void setWidth(const float width, const float aspectRatio = -1.0f);
	void setHeight(const float height, const float aspectRatio = -1.0f);

	// Get rectangle
	Rect getRect() const;

	// Set anchor
	void setAnchor(const Vector2 &anchor);

	// Make active
	void setFocused(const bool focused);

	// Override mouse event to handle UI specific functionality
	virtual void onMouseEvent(MouseEvent *e);
	
protected:
	// Rectangle of the object in relative coordinates [0-1]
	Rect m_rect;

	float m_aspectRatio;

	// Screen anchor [0-1]
	Vector2 m_anchor;

	// Object state
	bool m_hovered;
	bool m_pressed;
	bool m_focused;
};

#endif // UI_OBJECT_H