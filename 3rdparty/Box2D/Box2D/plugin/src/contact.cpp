#include "contact.h"
#include "box2d.h"
#include "body.h"
#include "plugin.h"
#include <Box2D/Box2D.h>
#include <x2d/scripts/funccall.h>

b2ContactWrapper::b2ContactWrapper(b2Contact *contact, b2BodyWrapper *thisBody, b2BodyWrapper *otherBody) :
	m_thisBody(thisBody),
	m_otherBody(otherBody),
	m_contact(contact)
{
	m_thisBody->addRef();
	m_otherBody->addRef();
}

b2ContactWrapper::~b2ContactWrapper()
{
	m_thisBody->release();
	m_otherBody->release();
}

void b2ContactWrapper::setEnabled(bool enabled)
{
	m_contact->SetEnabled(enabled);
}

void b2ContactWrapper::swapBodies()
{
	b2BodyWrapper *tmp = m_thisBody;
	m_thisBody = m_otherBody;
	m_otherBody = tmp;
}

void b2ContactWrapper::call(ContactType type)
{
	void *func = 0;
	switch(type)
	{
	case BeginContact: func = m_thisBody->m_beginContactFunc; break;
	case EndContact: func = m_thisBody->m_endContactFunc; break;
	case PreSolve: func = m_thisBody->m_preSolveFunc; break;
	case PostSolve: func = m_thisBody->m_postSolveFunc; break;
	}

	if(func)
	{
		FunctionCall *funcCall = CreateFuncCall();
		funcCall->Prepare(func);
		//funcCall->SetObject(&m_thisBody, 0x40000000);
		b2ContactWrapper *thiss = this;
		addRef();
		funcCall->SetArgument(0, &thiss, 0x40000000);
		funcCall->Execute();
	}
}

b2BodyWrapper *b2ContactWrapper::getThisBody() const
{
	return m_thisBody;
}

b2BodyWrapper *b2ContactWrapper::getOtherBody() const
{
	return m_otherBody;
}