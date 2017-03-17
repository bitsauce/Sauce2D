#ifndef LABEL_H
#define LABEL_H

#include "UiObject.h"

class Label : public UiObject
{
public:
	Label(UiObject *parent);

	void onDraw(DrawEvent *e);

private:
	Resource<Font> m_font;
	string m_text;
};

#endif // LABEL_H