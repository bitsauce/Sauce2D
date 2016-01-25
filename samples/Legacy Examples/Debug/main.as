Stickman stman;
Texture @ballTexture = @Texture(":/bilder/ball.png");
Ball ball;
Texture @bakgrunn = @Texture(":/bilder/bakgrunn.png");

void main() {
	stman.x = 200;
	stman.y = 380;
	ball.sprite.setSize(30, 30);
	ball.sprite.setPosition(550, 370);
	
}

void draw() {
	Batch @batch = @Batch();
	Shape @bg = @Shape(Rect(0, 0, 800, 600));
	bg.setFillTexture(@bakgrunn);
	bg.draw(@batch);
	
	Shape @bakke = @Shape(Rect(0, 400, 800, 200));
	bakke.setFillColor(Vector4(1, 1, 1, 1));
	bakke.draw(@batch);
	
	stman.draw(@batch);
	
	ball.draw(@batch);
	
	batch.draw();
}

void update() {
	stman.update();
	ball.update();
}

class Stickman {
	int age = 21;
	string name = "mann";
	float x = 0;
	float y = 0;
	float Vx = 0;
	float Vy = 0;
	
	void update() {
		if (Input.getKeyState(KEY_RIGHT)) {
			Vx = 20;
			Vy = -20;
		} else if (Input.getKeyState(KEY_LEFT)) {
			Vx = -20;
			Vy = -20;
		} else {
			Vx = 0;
		}

		Vy += 10;
		x += Vx;
		y += Vy;	
	
		if (y > 380) {
			y=380;
			Vy = 0;
		}
		
		if (Rect(ball.sprite.getX(), ball.sprite.getY(), 30, 30).contains(Rect(x, y, 5, 20))) {
			Console.log("Game over");
		}
	}
	
	void draw(Batch @batch) {
		Shape @stickman = @Shape(Rect(x, y, 5, 20));
		stickman.setFillColor(Vector4(1, 1, 1, 1));
		stickman.draw(@batch);
	}
}

class Ball {
	Sprite @sprite = @Sprite(TextureRegion(@ballTexture));
	float Vx = 0;
	
	void update() {
		if (sprite.getX() < 550) {
			Vx = 2;
		} else if (sprite.getX() > 700) {
			Vx = -2;
		}
		sprite.setPosition(sprite.getX()+Vx, sprite.getY());
	}
	
	void draw(Batch @batch) {
		sprite.draw(@batch);
	}
	
}