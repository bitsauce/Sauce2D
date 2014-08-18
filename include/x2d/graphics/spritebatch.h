#ifndef GRAPHICS_SPRITE_BATCH_H
#define GRAPHICS_SPRITE_BATCH_H

#include "batch.h"

class Sprite;

class SpriteBatch : public Batch
{
	AS_DECL_REF
public:

	~SpriteBatch();

	void add(Sprite *sprite);
	Sprite *get(int index);
	int getSize() const;

	void draw();
	void clear();
	void makeStatic();

private:
	vector<Sprite*> m_sprites;
	vector<Sprite*> m_returnedSprites;
	map<Sprite*, uint> m_offsets;
	
	static SpriteBatch *Factory() { return new SpriteBatch(); }

	// Serialization
	void serialize(StringStream &ss) const;
	static SpriteBatch *Factory(StringStream &ss);
};

#endif // GRAPHICS_SPRITE_BATCH_H