#include "MainMenu.h"

SinglePlayerCanvas::SinglePlayerCanvas(Gui* gui, Window* window) :
	Canvas(window)
{
	Background *bg = new Background(this);

	m_aspectRatioContainer = new AspectRatioContainer(bg, window, 1280, 1280.0f / 720.0f);
	m_aspectRatioContainer->setAnchor(0.5f, 0.5f);
	m_aspectRatioContainer->setOrigin(0.5f, 0.5f);

	m_lineEdit = new LineEdit(m_aspectRatioContainer, 230, 40);
	m_lineEdit->setDefaultText("World name");
	m_lineEdit->setSize(230.0f / 1280.0f, 40.0f / 720.0f);
	m_lineEdit->setPosition(0.0f, 0.0f);
	m_lineEdit->setOrigin(0.5f, 1.0f);
	m_lineEdit->setAnchor(0.5f, 0.3f);
	m_lineEdit->setAcceptFunc(bind(&Gui::pushCanvas, gui, new DialogBoxCanvas(gui, window, "Loading")));

	m_slider = new Slider(m_aspectRatioContainer);
	m_slider->setSize(230.0f / 1280.0f, 40.0f / 720.0f);
	m_slider->setPosition(0.0f, 0.1f);
	m_slider->setOrigin(0.5f, 1.0f);
	m_slider->setAnchor(0.5f, 0.3f);

	m_buttonBack = new Button(m_aspectRatioContainer, 60, 40);
	m_buttonBack->setText("Back");
	m_buttonBack->setSize(60.0f / 1280.0f, 40.0f / 720.0f);
	m_buttonBack->setAnchor(0.5f, 1.0f);
	m_buttonBack->setOrigin(0.5f, 1.0f);
	m_buttonBack->setPosition(-0.1f, -0.25f);
	m_buttonBack->setOnClickCallback(bind(&Gui::popCanvas, gui));

	m_buttonOk = new Button(m_aspectRatioContainer, 60, 40);
	m_buttonOk->setText("OK");
	m_buttonOk->setSize(60.0f / 1280.0f, 40.0f / 720.0f);
	m_buttonOk->setAnchor(0.5f, 1.0f);
	m_buttonOk->setOrigin(0.5f, 1.0f);
	m_buttonOk->setPosition(0.1f, -0.25f);
	m_buttonOk->setOnClickCallback(bind(&Gui::pushCanvas, gui, new DialogBoxCanvas(gui, window, "Loading")));
}
