#ifndef B2_CONTACT_WRAPPER_H
#define B2_CONTACT_WRAPPER_H

#include <x2d/base.h>
#include <x2d/math.h>

class b2Contact;
class b2BodyWrapper;

class b2ContactWrapper
{
public:
	b2ContactWrapper(b2Contact *contact, b2BodyWrapper *thisBody, b2BodyWrapper *otherBody);
	~b2ContactWrapper();

	enum ContactType
	{
		BeginContact,
		EndContact,
		PreSolve,
		PostSolve
	};

	RefCounter refCounter;
	void addRef() { refCounter.add(); }
	void release() { if(refCounter.release() == 0) delete this; }

	void setEnabled(bool enabled);
	
	void swapBodies();
	void call(ContactType stage);

	b2BodyWrapper *getOtherBody() const;
	b2BodyWrapper *getThisBody() const;

private:
	b2Contact *m_contact;
	b2BodyWrapper *m_thisBody;
	b2BodyWrapper *m_otherBody;
};

#endif // B2_CONTACT_WRAPPER_H