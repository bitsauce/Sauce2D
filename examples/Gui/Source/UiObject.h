#ifndef UI_OBJECT_H
#define UI_OBJECT_H

#include "Config.h"
#include "UiEvents.h"

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

	/**
	 * \fn	void UiObject::setFocused(const bool focused);
	 *
	 * \brief	Make active.
	 *
	 * \param	focused	The focused.
	 */

	void setFocused(const bool focused);

	// Click event
	virtual void onClick(ClickEvent*) { }
	bool isPressed() const;

	// Set/get position
	void setPosition(const Vector2F &position);
	void setPosition(const float x, const float y);
	Vector2F getPosition() const;

	void setPositionPx(const Vector2I &position);
	void setPositionPx(const int x, const int y);
	Vector2I getPositionPx() const;

	// Set/get size
	void setSize(const Vector2F &size);
	void setSize(const float width, const float height);
	Vector2F getSize() const;

	void setSizePx(const Vector2I &size);
	void setSizePx(const int w, const int h);
	Vector2I getSizePx() const;

	void setWidth(const float width, const float aspectRatio = -1.0f);
	void setWidthPx(const int width, const float aspectRatio = -1.0f);
	void setHeight(const float height, const float aspectRatio = -1.0f);
	void setHeightPx(const int height, const float aspectRatio = -1.0f);

	void setRect(const RectF &rect);
	RectF getRect() const;
	void setRectPx(const RectI &rect);
	RectI getRectPx() const;

	/**
	 * \fn	void UiObject::setAnchor(const float x, const float y);
	 *
	 * \brief	Set anchoring point.
	 *
	 * \param	x	The x coordinate of anchor.
	 * \param	y	The y coordinate of anchor.
	 */

	void setAnchor(const float x, const float y);
	void setAnchor(const Vector2F &anchor);
	Vector2F getAnchor() const;

	void setAnchorPx(const int x, const int y);
	void setAnchorPx(const Vector2I &anchor);
	Vector2I getAnchorPx() const;

	virtual Vector2I getDrawPosition();
	virtual Vector2I getDrawSize();
	RectI getDrawRect();

	/**
	 * \fn	virtual void UiObject::onMouseEvent(MouseEvent *e);
	 *
	 * \brief	Override mouse event to handle UI specific functionality.
	 *
	 * \param [in,out]	e	If non-null, the MouseEvent to process.
	 */

	virtual void onMouseEvent(MouseEvent *e);
	
protected:
	Vector2I getParentSize() const;
	float getAspectRatio() const;

	/** \brief	Rectangle of the ui element in relative coordinates [0, 1]. */
	RectF m_rect;

	/** \brief	The aspect ratio. */
	float m_aspectRatio;

	/** \brief	Screen anchor in relative coordinates [0, 1]. */
	Vector2F m_anchor;

	/** \brief	true if cursor is hovering the ui object. */
	bool m_hovered;
	
	/** \brief	true if ui object is pressed. */
	bool m_pressed;

	/** \brief	true if ui object has focus. */
	bool m_focused;
};

#endif // UI_OBJECT_H