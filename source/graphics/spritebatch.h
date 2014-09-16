#ifndef GRAPHICS_SPRITE_BATCH_H
#define GRAPHICS_SPRITE_BATCH_H

#include "batch.h"

class XSprite;

class XSpriteBatch : public XBatch
{
	AS_DECL_REF
public:

	~XSpriteBatch();

	void add(XSprite *sprite);
	XSprite *get(int index);
	int getSize() const;

	void draw();
	void clear();
	void makeStatic();

private:
	vector<XSprite*> m_sprites;
	vector<XSprite*> m_returnedSprites;
	map<XSprite*, uint> m_offsets;
	
	static XSpriteBatch *Factory() { return new XSpriteBatch(); }
};

#endif // GRAPHICS_SPRITE_BATCH_H