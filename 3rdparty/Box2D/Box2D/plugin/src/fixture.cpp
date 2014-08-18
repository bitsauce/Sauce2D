#include "fixture.h"
#include "body.h"
#include "box2d.h"
#include "plugin.h"
#include <x2d/scriptengine.h>
#include <Box2D/Box2D.h>

int b2FixtureWrapper::TypeId = 0;

b2FixtureWrapper::b2FixtureWrapper(b2BodyWrapper *body, b2Fixture *fixture) :
	m_body(body),
	m_fixture(fixture)
{
	m_fixture->SetUserData(this);
}

b2FixtureWrapper::~b2FixtureWrapper()
{
	if(m_body) {
		vector<b2FixtureWrapper*>::iterator itr = find(m_body->m_fixtures.begin(), m_body->m_fixtures.end(), this);
		if(itr != m_body->m_fixtures.end())
			m_body->m_fixtures.erase(itr);
	}
}

void b2FixtureWrapper::destroy()
{
	if(m_fixture)
	{
		b2d->destroyFixture(m_body->m_body, &m_fixture);
	}
}

void b2FixtureWrapper::setFriction(const float friction)
{
	if(!m_fixture)
		return;

	m_fixture->SetFriction(friction);
}

void b2FixtureWrapper::setDensity(const float density)
{
	if(!m_fixture)
		return;

	m_fixture->SetDensity(density);
	m_fixture->GetBody()->ResetMassData();
}

void b2FixtureWrapper::setRestitution(const float restitution)
{
	if(!m_fixture)
		return;

	m_fixture->SetRestitution(restitution);
}

void b2FixtureWrapper::setSensor(const bool sensor)
{
	if(!m_fixture)
		return;

	m_fixture->SetSensor(sensor);
}

bool b2FixtureWrapper::isSensor() const
{
	if(!m_fixture)
		return false;

	return m_fixture->IsSensor();
}

void b2FixtureWrapper::setMaskBits(const uint maskBits)
{
	if(!m_fixture)
		return;

	b2Filter data = m_fixture->GetFilterData();
	data.maskBits = maskBits;
	m_fixture->SetFilterData(data);
}

void b2FixtureWrapper::setCategoryBits(const uint categoryBits)
{
	if(!m_fixture)
		return;

	b2Filter data = m_fixture->GetFilterData();
	data.categoryBits = categoryBits;
	m_fixture->SetFilterData(data);
}

b2BodyWrapper *b2FixtureWrapper::getBody() const
{
	if(!m_fixture)
		return 0;

	m_body->addRef();
	return m_body;
}

void b2FixtureWrapper::enumReferences(asIScriptEngine *engine)
{
	if(m_body) {
		engine->GCEnumCallback(m_body);
	}
}

void b2FixtureWrapper::releaseReferences(asIScriptEngine *engine)
{
	if(m_body) {
		m_body->release();
	}
}

void b2FixtureWrapper::serialize(StringStream &ss) const
{
	xdengine->getScriptEngine()->serialize((void*)&m_body, b2BodyWrapper::TypeId | asTYPEID_OBJHANDLE, ss);
	
	b2Shape *shape = m_fixture->GetShape();
	(stringstream&)ss << shape->GetType() << endl;
	switch(shape->GetType())
	{
	case b2Shape::e_circle:
		{
			b2CircleShape *s = (b2CircleShape*)shape;
			(stringstream&)ss << s->m_radius << endl;
			(stringstream&)ss << s->m_p.x << endl;
			(stringstream&)ss << s->m_p.y << endl;
		}
		break;

	case b2Shape::e_edge:
		{
			b2EdgeShape *s = (b2EdgeShape*)shape;
			(stringstream&)ss << s->m_radius << endl;
			(stringstream&)ss << s->m_vertex0.x << endl;
			(stringstream&)ss << s->m_vertex0.y << endl;
			(stringstream&)ss << s->m_vertex1.x << endl;
			(stringstream&)ss << s->m_vertex1.y << endl;
			(stringstream&)ss << s->m_vertex2.x << endl;
			(stringstream&)ss << s->m_vertex2.y << endl;
			(stringstream&)ss << s->m_vertex3.x << endl;
			(stringstream&)ss << s->m_vertex3.y << endl;
			(stringstream&)ss << s->m_hasVertex0 << endl;
			(stringstream&)ss << s->m_hasVertex3 << endl;
		}
		break;

	case b2Shape::e_polygon:
		{
			b2PolygonShape *s = (b2PolygonShape*)shape;
			(stringstream&)ss << s->m_count << endl;
			for(int32 i = 0; i < s->m_count; i++)
			{
				(stringstream&)ss << s->m_vertices[i].x << endl;
				(stringstream&)ss << s->m_vertices[i].y << endl;
			}
		}
		break;

	case b2Shape::e_chain:
		{
			b2ChainShape *s = (b2ChainShape*)shape;
			(stringstream&)ss << s->m_count << endl;
			for(int32 i = 0; i < s->m_count; i++)
			{
				(stringstream&)ss << s->m_vertices[i].x << endl;
				(stringstream&)ss << s->m_vertices[i].y << endl;
			}

			(stringstream&)ss << s->m_prevVertex.x << endl;
			(stringstream&)ss << s->m_prevVertex.y << endl;
			(stringstream&)ss << s->m_nextVertex.x << endl;
			(stringstream&)ss << s->m_nextVertex.y << endl;
			(stringstream&)ss << s->m_hasPrevVertex << endl;
			(stringstream&)ss << s->m_hasNextVertex << endl;
		}
		break;

	default:
		break;
	}

	(stringstream&)ss << m_fixture->GetFriction() << endl;
	(stringstream&)ss << m_fixture->GetRestitution() << endl;
	(stringstream&)ss << m_fixture->GetDensity() << endl;
	(stringstream&)ss << m_fixture->IsSensor() << endl;
	(stringstream&)ss << m_fixture->GetFilterData().categoryBits << endl;
	(stringstream&)ss << m_fixture->GetFilterData().maskBits << endl;
	(stringstream&)ss << m_fixture->GetFilterData().groupIndex << endl;
}

b2FixtureWrapper *b2FixtureWrapper::Factory(StringStream &ss)
{
	b2BodyWrapper *body;
	xdengine->getScriptEngine()->deserialize(&body, b2BodyWrapper::TypeId | asTYPEID_OBJHANDLE, ss);
	
	int32 i;
	float32 f;
	bool b;
	uint16 us;
	int16 s;

	(stringstream&)ss >> i; ((stringstream&)ss).ignore();
	b2Shape *shape = 0;
	switch(i)
	{
	case b2Shape::e_circle:
		{
			b2CircleShape *s = new b2CircleShape();

			(stringstream&)ss >> f; ((stringstream&)ss).ignore();
			s->m_radius = f;
			
			(stringstream&)ss >> f; ((stringstream&)ss).ignore();
			s->m_p.x = f;
			
			(stringstream&)ss >> f; ((stringstream&)ss).ignore();
			s->m_p.y = f;

			shape = s;
		}
		break;

	case b2Shape::e_edge:
		{
			b2EdgeShape *s = new b2EdgeShape();
			
			(stringstream&)ss >> f; ((stringstream&)ss).ignore();
			s->m_radius = f;
			
			(stringstream&)ss >> f; ((stringstream&)ss).ignore();
			s->m_vertex0.x = f;

			(stringstream&)ss >> f; ((stringstream&)ss).ignore();
			s->m_vertex0.y = f;
			
			(stringstream&)ss >> f; ((stringstream&)ss).ignore();
			s->m_vertex1.x = f;
			
			(stringstream&)ss >> f; ((stringstream&)ss).ignore();
			s->m_vertex1.y = f;
			
			(stringstream&)ss >> f; ((stringstream&)ss).ignore();
			s->m_vertex2.x = f;
			
			(stringstream&)ss >> f; ((stringstream&)ss).ignore();
			s->m_vertex2.y = f;
			
			(stringstream&)ss >> f; ((stringstream&)ss).ignore();
			s->m_vertex3.x = f;
			
			(stringstream&)ss >> f; ((stringstream&)ss).ignore();
			s->m_vertex3.y = f;
			
			(stringstream&)ss >> b; ((stringstream&)ss).ignore();
			s->m_hasVertex0 = b;
			
			(stringstream&)ss >> b; ((stringstream&)ss).ignore();
			s->m_hasVertex3 = b;
			
			shape = s;
		}
		break;

	case b2Shape::e_polygon:
		{
			b2PolygonShape *s = new b2PolygonShape();
			
			(stringstream&)ss >> i; ((stringstream&)ss).ignore();
			s->m_count = i;

			for(int32 i = 0; i < s->m_count; i++)
			{
				(stringstream&)ss >> f; ((stringstream&)ss).ignore();
				s->m_vertices[i].x = f;
				
				(stringstream&)ss >> f; ((stringstream&)ss).ignore();
				s->m_vertices[i].y = f;
			}
			
			shape = s;
		}
		break;

	case b2Shape::e_chain:
		{
			b2ChainShape *s = new b2ChainShape;
			
			(stringstream&)ss >> i; ((stringstream&)ss).ignore();
			s->m_count = i;

			for(int32 i = 0; i < s->m_count; i++)
			{
				(stringstream&)ss >> f; ((stringstream&)ss).ignore();
				s->m_vertices[i].x = f;
				
				(stringstream&)ss >> f; ((stringstream&)ss).ignore();
				s->m_vertices[i].y = f;
			}
			
			(stringstream&)ss >> f; ((stringstream&)ss).ignore();
			s->m_prevVertex.x = f;
				
			(stringstream&)ss >> f; ((stringstream&)ss).ignore();
			s->m_prevVertex.y = f;
			
			(stringstream&)ss >> f; ((stringstream&)ss).ignore();
			s->m_nextVertex.x = f;
				
			(stringstream&)ss >> f; ((stringstream&)ss).ignore();
			s->m_nextVertex.y = f;
			
			(stringstream&)ss >> b; ((stringstream&)ss).ignore();
			s->m_hasPrevVertex = b;

			(stringstream&)ss >> b; ((stringstream&)ss).ignore();
			s->m_hasNextVertex = b;

			shape = s;
		}
		break;

	default:
		break;
	}
	

	b2FixtureWrapper *fixture = new b2FixtureWrapper(body, body->m_body->CreateFixture(shape, 1.0f));
	delete shape;
	
	asIScriptEngine *scriptEngine = xdengine->getScriptEngine()->getASEngine();
	scriptEngine->NotifyGarbageCollectorOfNewObject(fixture, scriptEngine->GetObjectTypeById(b2FixtureWrapper::TypeId));
	xdengine->getScriptEngine()->notifySerializerOfNewObject(fixture);

	(stringstream&)ss >> f; ((stringstream&)ss).ignore();
	fixture->m_fixture->SetFriction(f);
	
	(stringstream&)ss >> f; ((stringstream&)ss).ignore();
	fixture->m_fixture->SetRestitution(f);
	
	(stringstream&)ss >> f; ((stringstream&)ss).ignore();
	fixture->m_fixture->SetDensity(f);

	(stringstream&)ss >> b; ((stringstream&)ss).ignore();
	fixture->m_fixture->SetSensor(b);
	
	b2Filter filter;

	(stringstream&)ss >> us; ((stringstream&)ss).ignore();
	filter.categoryBits = us;
	
	(stringstream&)ss >> us; ((stringstream&)ss).ignore();
	filter.maskBits = us;
	
	(stringstream&)ss >> s; ((stringstream&)ss).ignore();
	filter.groupIndex = s;

	fixture->m_fixture->SetFilterData(filter);

	return fixture;
}