#include "UiObject.h"

UiObject::UiObject() :
	m_anchor(0.0f, 0.0f),
	m_rect(),
	m_hovered(false),
	m_pressed(false),
	m_focused(false)
{
}

UiObject::~UiObject()
{
}

bool UiObject::isFocused() const
{
	return m_focused;
}

bool UiObject::isHovered() const
{
	return m_hovered;
}

bool UiObject::isPressed() const
{
	return m_pressed;
}

void UiObject::onMouseEvent(MouseEvent *e)
{
	switch(e->getType())
	{
		case MouseEvent::MOVE:
		{
			if(getRect().contains(e->getPosition()))
			{
				m_hovered = true;
				HoverEvent event(HoverEvent::ENTER, e);
				onHover(&event);
			}
			else
			{
				m_hovered = false;
				HoverEvent event(HoverEvent::LEAVE, e);
				onHover(&event);
			}
		}
		break;
		
		case MouseEvent::DOWN:
		{
			if(m_hovered)
			{
				m_pressed = true;
				ClickEvent event(ClickEvent::BEGIN, e);
				onClick(&event);
			}
		}
		break;

		case MouseEvent::UP:
		{
			if(m_pressed)
			{
				if(m_hovered)
				{
					ClickEvent event(ClickEvent::DONE, e);
					onClick(&event);
					if(!m_focused)
					{
						m_focused = true;
						FocusEvent event(FocusEvent::GAINED);
						onFocus(&event);
					}
				}
				else
				{
					ClickEvent event(ClickEvent::CANCELED, e);
					onClick(&event);
				}
				m_pressed = false;
			}
			else if(!m_hovered && m_focused)
			{
				m_focused = false;
				FocusEvent event(FocusEvent::LOST);
				onFocus(&event);
			}
		}
		break;
	}
	SceneObject::onMouseEvent(e);
}

void UiObject::setPosition(const Vector2 &position)
{
	m_rect.position = position;
}

Vector2 UiObject::getPosition() const
{
	UiObject *parent = dynamic_cast<UiObject*>(getParent());
	if(!parent) return m_rect.position;

	Vector2 parentPos = parent->getPosition();
	Vector2 parentSize = parent->getSize();
	Vector2 pos = m_rect.position;
	Vector2 size = m_rect.size;

	parentPos += parentSize * m_anchor;
	pos -= size * m_anchor;

	return parentPos + pos * parentSize;
}

void UiObject::setSize(const Vector2 &size)
{
	setSize(size.x, size.y);
}

void UiObject::setSize(const float width, const float height)
{
	m_rect.size.set(width, height);
}

Vector2 UiObject::getSize() const
{
	UiObject *parent = dynamic_cast<UiObject*>(getParent());
	if(!parent) return m_rect.size;

	if(m_rect.size.y <= 0.0f)
	{
		return Vector2(m_rect.size.x, m_rect.size.x * m_aspectRatio) * parent->getSize().x;
	}

	return m_rect.size * parent->getSize();
}

void UiObject::setWidth(const float width, const float aspectRatio)
{
	m_rect.size.set(width, -1.0f);
	m_aspectRatio = aspectRatio;
}

Rect UiObject::getRect() const
{
	return Rect(getPosition(), getSize());
}

void UiObject::setAnchor(const Vector2 &anchor)
{
	m_anchor = anchor;
}

void UiObject::setFocused(const bool focused)
{
	m_focused = focused;
}
