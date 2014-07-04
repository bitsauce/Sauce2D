#include "plugin.h"

#include <x2d/scriptengine.h>
#include <x2d/util.h>
#include <x2d/graphics.h>
#include <x2d/graphics/batch.h>
#include <x2d/graphics/texture.h>
#include <x2d/filesystem.h>
#include <spine/spine.h>

#include <spine/extension.h>

#ifndef SPINE_MESH_VERTEX_COUNT_MAX
#define SPINE_MESH_VERTEX_COUNT_MAX 1000
#endif

void _spAtlasPage_createTexture(spAtlasPage* self, const char* path)
{
	Texture *texture = xdGraphics::CreateTexture(path);
	if(texture)
	{
		self->rendererObject = texture;
		self->width = texture->getWidth();
		self->height = texture->getHeight();
	}
}

void _spAtlasPage_disposeTexture(spAtlasPage* self)
{
	if(self->rendererObject) {
		((Texture*)self->rendererObject)->release();
	}
}

char* _spUtil_readFile(const char* path, int* length)
{
	string content;
	char *data = 0;
	if(xdFileSystem::ReadFile(path, content))
	{
		*length = content.size();
		data = MALLOC(char, *length);
		memcpy(data, content.c_str(), *length);
	}
	return data;
}

class spAnimationWrapper
{
public:
	spAnimationWrapper(spSkeleton *skeleton, spAnimation *anim) :
		m_animation(anim),
		m_skeleton(skeleton),
		m_lastTime(0.0f),
		m_looping(false)
	{
	}

	void addRef() { refCounter.add(); }
	void release() { if(refCounter.release() == 0) delete this; }

	void setTime(const float time)
	{
		m_lastTime = time;
	}

	void setLooping(const bool looping)
	{
		m_looping = looping;
	}

	void apply(const float dt)
	{
		float time = m_lastTime + dt;
		spAnimation_apply(m_animation, m_skeleton, m_lastTime, time, m_looping, 0, 0);
		m_lastTime = time;
	}

	void mix(const float dt, const float alpha)
	{
		float time = m_lastTime + dt;
		spAnimation_mix(m_animation, m_skeleton, m_lastTime, time, m_looping, 0, 0, alpha);
		m_lastTime = time;
	}

private:
	RefCounter refCounter;
	spAnimation *m_animation;
	spSkeleton *m_skeleton;
	float m_lastTime;
	bool m_looping;
};

class spSkeletonWrapper
{
public:
	spSkeletonWrapper(const string &jsonFile, const string &atlasFile, const float scale = 1.0f)
	{
		m_worldVertices = MALLOC(float, SPINE_MESH_VERTEX_COUNT_MAX);
		m_atlas = spAtlas_createFromFile(atlasFile.c_str(), 0);
		spSkeletonJson* json = spSkeletonJson_create(m_atlas);
		json->scale = scale;
		m_skeletonData = spSkeletonJson_readSkeletonDataFile(json, jsonFile.c_str());
		if (!m_skeletonData) {
			LOG("spSkeleton.Constructor: %s", json->error);
			return;
		}
		LOG("spSkeleton.Constructor: Default skin name '%s'", m_skeletonData->defaultSkin->name);

		m_skeleton = spSkeleton_create(m_skeletonData);

		// Pre-load animation objects
		for(int i = 0; i < m_skeletonData->animationCount; i++) {
			spAnimationState_create(spAnimationStateData_create(m_skeletonData));
			spAnimation *anim = m_skeletonData->animations[i];
			m_animations[anim->name] = new spAnimationWrapper(m_skeleton, anim);
		}
		spSkeletonJson_dispose(json);
	}

	~spSkeletonWrapper()
	{
		for(int i = 0; i < m_skeletonData->animationCount; i++) {
			spAnimation *anim = m_skeletonData->animations[i];
			m_animations[anim->name]->release();
		}
		spAtlas_dispose(m_atlas);
		spSkeleton_dispose(m_skeleton);
		spSkeletonData_dispose(m_skeletonData);
	}
	
	void addRef() { refCounter.add(); }
	void release() { if(refCounter.release() == 0) delete this; }

	spAnimationWrapper *findAnimation(const string &name)
	{
		spAnimationWrapper *anim = m_animations.find(name) != m_animations.end() ? m_animations[name] : 0;
		if(anim) anim->addRef();
		return anim;
	}

	void setPosition(const Vector2 &pos)
	{
		m_skeleton->x = pos.x;
		m_skeleton->y = pos.y;
	}

	void setFlipX(const bool flip)
	{
		m_skeleton->flipX = flip;
	}

	void setFlipY(const bool flip)
	{
		m_skeleton->flipY = flip;
	}

	void draw(Batch *batch)
	{
		if(!batch)
			return;
		spSkeleton_updateWorldTransform(m_skeleton);

		Vertex vertices[4];
		for(int i = 0; i < m_skeleton->slotCount; i++)
		{
			spSlot *slot = m_skeleton->drawOrder[i];
			spAttachment *attachment = slot->attachment;
			if(!attachment) continue;
			Texture *texture = 0;
			if(attachment->type == SP_ATTACHMENT_REGION)
			{
				spRegionAttachment* regionAttachment = SUB_CAST(spRegionAttachment, attachment);
				texture = (Texture*)((spAtlasRegion*)regionAttachment->rendererObject)->page->rendererObject;
				spRegionAttachment_computeWorldVertices(regionAttachment, slot->skeleton->x, slot->skeleton->y, slot->bone, m_worldVertices);

				uchar r = m_skeleton->r * slot->r * 255;
				uchar g = m_skeleton->g * slot->g * 255;
				uchar b = m_skeleton->b * slot->b * 255;
				uchar a = m_skeleton->a * slot->a * 255;

				vertices[0].color.set(r, g, b, a);
				vertices[0].position.x = m_worldVertices[SP_VERTEX_X1];
				vertices[0].position.y = m_worldVertices[SP_VERTEX_Y1];
				vertices[0].texCoord.x = regionAttachment->uvs[SP_VERTEX_X1];
				vertices[0].texCoord.y = 1.0f - regionAttachment->uvs[SP_VERTEX_Y1];
				
				vertices[1].color.set(r, g, b, a);
				vertices[1].position.x = m_worldVertices[SP_VERTEX_X2];
				vertices[1].position.y = m_worldVertices[SP_VERTEX_Y2];
				vertices[1].texCoord.x = regionAttachment->uvs[SP_VERTEX_X2];
				vertices[1].texCoord.y = 1.0f - regionAttachment->uvs[SP_VERTEX_Y2];
				
				vertices[2].color.set(r, g, b, a);
				vertices[2].position.x = m_worldVertices[SP_VERTEX_X3];
				vertices[2].position.y = m_worldVertices[SP_VERTEX_Y3];
				vertices[2].texCoord.x = regionAttachment->uvs[SP_VERTEX_X3];
				vertices[2].texCoord.y = 1.0f - regionAttachment->uvs[SP_VERTEX_Y3];
				
				vertices[3].color.set(r, g, b, a);
				vertices[3].position.x = m_worldVertices[SP_VERTEX_X4];
				vertices[3].position.y = m_worldVertices[SP_VERTEX_Y4];
				vertices[3].texCoord.x = regionAttachment->uvs[SP_VERTEX_X4];
				vertices[3].texCoord.y = 1.0f - regionAttachment->uvs[SP_VERTEX_Y4];

				texture->addRef();
				batch->setTexture(texture);
				batch->addVertices(vertices, 4, QUAD_INDICES, 6);
				batch->setTexture(0);
			} /*else if (attachment->type == ATTACHMENT_MESH) {
				MeshAttachment* mesh = (MeshAttachment*)attachment;
				if (mesh->verticesCount > SPINE_MESH_VERTEX_COUNT_MAX) continue;
				texture = (Texture*)((AtlasRegion*)mesh->rendererObject)->page->rendererObject;
				MeshAttachment_computeWorldVertices(mesh, slot->skeleton->x, slot->skeleton->y, slot, worldVertices);

				Uint8 r = skeleton->r * slot->r * 255;
				Uint8 g = skeleton->g * slot->g * 255;
				Uint8 b = skeleton->b * slot->b * 255;
				Uint8 a = skeleton->a * slot->a * 255;
				vertex.color.r = r;
				vertex.color.g = g;
				vertex.color.b = b;
				vertex.color.a = a;

				Vector2u size = texture->getSize();
				for (int i = 0; i < mesh->trianglesCount; ++i) {
					int index = mesh->triangles[i] << 1;
					vertex.position.x = worldVertices[index];
					vertex.position.y = worldVertices[index + 1];
					vertex.texCoords.x = mesh->uvs[index] * size.x;
					vertex.texCoords.y = mesh->uvs[index + 1] * size.y;
					vertexArray->append(vertex);
				}

			} else if (attachment->type == ATTACHMENT_SKINNED_MESH) {
				SkinnedMeshAttachment* mesh = (SkinnedMeshAttachment*)attachment;
				if (mesh->uvsCount > SPINE_MESH_VERTEX_COUNT_MAX) continue;
				texture = (Texture*)((AtlasRegion*)mesh->rendererObject)->page->rendererObject;
				SkinnedMeshAttachment_computeWorldVertices(mesh, slot->skeleton->x, slot->skeleton->y, slot, worldVertices);

				Uint8 r = skeleton->r * slot->r * 255;
				Uint8 g = skeleton->g * slot->g * 255;
				Uint8 b = skeleton->b * slot->b * 255;
				Uint8 a = skeleton->a * slot->a * 255;
				vertex.color.r = r;
				vertex.color.g = g;
				vertex.color.b = b;
				vertex.color.a = a;

				Vector2u size = texture->getSize();
				for (int i = 0; i < mesh->trianglesCount; ++i) {
					int index = mesh->triangles[i] << 1;
					vertex.position.x = worldVertices[index];
					vertex.position.y = worldVertices[index + 1];
					vertex.texCoords.x = mesh->uvs[index] * size.x;
					vertex.texCoords.y = mesh->uvs[index + 1] * size.y;
					vertexArray->append(vertex);
				}
			}*/
		}
	}

	static spSkeletonWrapper *Factory(const string &jsonFile, const string &atlasFile, const float scale)
	{
		return new spSkeletonWrapper(jsonFile, atlasFile, scale);
	}

private:
	RefCounter refCounter;
	spSkeleton *m_skeleton;
	spSkeletonData *m_skeletonData;
	spAtlas* m_atlas;
	float *m_worldVertices;
	map<string, spAnimationWrapper*> m_animations;
};

int CreatePlugin(xdScriptEngine *scriptEngine)
{
	int r = 0;

	spBone_setYDown(true);
	
	r = scriptEngine->registerRefType("spSkeleton", asMETHOD(spSkeletonWrapper, addRef), asMETHOD(spSkeletonWrapper, release));
	r = scriptEngine->registerRefType("spAnimation", asMETHOD(spAnimationWrapper, addRef), asMETHOD(spAnimationWrapper, release));

	r = scriptEngine->registerObjectFactory("spSkeleton", "spSkeleton @f(const string &in, const string &in, const float scale = 1.0f)", asFUNCTION(spSkeletonWrapper::Factory));
	r = scriptEngine->registerObjectMethod("spSkeleton", "spAnimation @findAnimation(const string &in)", asMETHOD(spSkeletonWrapper, findAnimation));
	r = scriptEngine->registerObjectMethod("spSkeleton", "void setPosition(const Vector2 &in)", asMETHOD(spSkeletonWrapper, setPosition));
	r = scriptEngine->registerObjectMethod("spSkeleton", "void setFlipX(const bool)", asMETHOD(spSkeletonWrapper, setFlipX));
	r = scriptEngine->registerObjectMethod("spSkeleton", "void setFlipY(const bool)", asMETHOD(spSkeletonWrapper, setFlipY));
	r = scriptEngine->registerObjectMethod("spSkeleton", "void draw(Batch@)", asMETHOD(spSkeletonWrapper, draw));
	
	r = scriptEngine->registerObjectMethod("spAnimation", "void setTime(const float)", asMETHOD(spAnimationWrapper, setTime));
	r = scriptEngine->registerObjectMethod("spAnimation", "void setLooping(const bool)", asMETHOD(spAnimationWrapper, setLooping));
	r = scriptEngine->registerObjectMethod("spAnimation", "void apply(const float)", asMETHOD(spAnimationWrapper, apply));
	r = scriptEngine->registerObjectMethod("spAnimation", "void mix(const float, const float)", asMETHOD(spAnimationWrapper, mix));

	return 0;
}

void ReleasePlugin()
{
}