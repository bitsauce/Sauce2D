#include "DialogBoxCanvas.h"
#include "Button.h"
#include "Gui.h"

class Background2 : public UiObject
{
public:
	Background2(UiObject *parent) :
		UiObject(parent)
	{
		setSize(1.f, 1.f);
	}

	void onDraw(DrawEvent *e)
	{
		e->getGraphicsContext()->drawRectangle(getDrawRect(), Color(30, 30, 30, 127));
		UiObject::onDraw(e);
	}
};

DialogBoxCanvas::DialogBoxCanvas(Gui *gui, Window *window, Canvas*c) :
	Canvas(window, true)
{
	Background2 *bg = new Background2(this);

	m_aspectRatioContainer = new AspectRatioContainer(bg, window, 1280, 1280.0f / 720.0f);
	m_aspectRatioContainer->setAnchor(0.5f, 0.5f);
	m_aspectRatioContainer->setOrigin(0.5f, 0.5f);

	m_buttonOk = new Button(m_aspectRatioContainer, 60, 40);
	m_buttonOk->setText("OK");
	m_buttonOk->setSize(60.0f / 1280.0f, 40.0f / 720.0f);
	m_buttonOk->setAnchor(0.5f, 0.65f);
	m_buttonOk->setOrigin(0.5f, 0.5f);
	m_buttonOk->setPosition(0.0f, 0.0f);
	m_buttonOk->setOnClickCallback(bind(&Gui::popCanvas, gui));

	m_buttonOk2 = new Button(m_aspectRatioContainer, 60, 40);
	m_buttonOk2->setText("OK2");
	m_buttonOk2->setSize(60.0f / 1280.0f, 40.0f / 720.0f);
	m_buttonOk2->setAnchor(0.5f, 0.65f);
	m_buttonOk2->setOrigin(0.5f, 0.5f);
	m_buttonOk2->setPosition(0.0f, -0.2f);
	m_buttonOk2->setOnClickCallback(bind(&Gui::pushCanvas, gui, c));
}