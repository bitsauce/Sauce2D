enum Anchor
{
	TOP		= 1,
	LEFT	= 2,
	BOTTOM	= 4,
	RIGHT	= 8,
	
	HORIZONTAL = 16,
	VERTICAL = 32,
	
	CENTER = HORIZONTAL | VERTICAL,
	
	TOP_LEFT = TOP | LEFT,
	TOP_RIGHT = TOP | RIGHT,
	BOTTOM_LEFT = BOTTOM | LEFT,
	BOTTOM_RIGHT = BOTTOM | RIGHT,
	
	TOP_CENTER = TOP | VERTICAL,
	LEFT_CENTER = LEFT | HORIZONTAL,
	RIGHT_CENTER = RIGHT | VERTICAL,
	BOTTOM_CENTER = BOTTOM | HORIZONTAL
}

enum UiState
{
	UI_PRESSED = 1,
	UI_HOVERED = 2,
	UI_ACTIVE = 4
}

class UiObject
{
	// Rect in screen relative coordinates
	private Rect rect;
	private UiObject @parent;
	private uint state;
	uint anchor;
		
	UiObject(UiObject @parent)
	{
		@this.parent = @parent;
		this.anchor = TOP_LEFT;
		this.state = 0;
	}
	
	Vector2 getSize(const bool recursive)
	{
		if(recursive)
		{
			return parent.getSize(recursive) * rect.size;
		}
		return rect.size;
	}
}

class UiRoot : UiObject
{
	UiRoot()
	{
		super(@null);
		rect.set(0.0f, 0.0f, 1.0f, 1.0f);
	}
	
	Vector2 getSize(const bool)
	{
		return rect.size * Vector2(Window.getSize());
	}
}

//UiRoot @uiRoot = @UiRoot();