
// Load spine skeleton from file
spSkeleton @skeleton = @spSkeleton(":/data/spineboy.json", ":/data/spineboy.atlas", 0.5f);

// Drawing batch
Batch @batch = @Batch();

// Current player animation
spAnimation @currentAnim;

// Player position
Vector2 position(200, 600);

void main()
{
	Console.log("** Spine Example **");
}

bool lmb = false;

void update()
{
	// Movement
	if(Input.getKeyState(KEY_D))
	{
		@currentAnim = @skeleton.findAnimation("walk");
		position.x += 5;
		skeleton.setFlipX(false);
	}else if(Input.getKeyState(KEY_A)){
		@currentAnim = @skeleton.findAnimation("walk");
		position.x -= 5;
		skeleton.setFlipX(true);
	}else{
		@currentAnim = @skeleton.findAnimation("idle");
	}
	
	if(Input.getKeyState(KEY_LMB))
	{
		if(!lmb) {
			spAnimation @anim = @skeleton.findAnimation("shoot");
			anim.setTime(0.0f);
		}
		lmb = true;
	}else /*if(anim.done())*/{
		lmb = false;
	}
	
	// Update skeleton and animation
	skeleton.setPosition(position);
	currentAnim.setLooping(true);
	currentAnim.apply(Graphics.dt);
}

void draw()
{
	// Draw skeleton to batch
	skeleton.draw(@batch);
	batch.draw();
	batch.clear();
}