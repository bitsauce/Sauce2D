void main()
{
}

void draw()
{
	Batch @batch = @Batch();
	Shape @shape = @Shape(Rect(0, 0, 50, 50));
	shape.setFillColor(Vector4(1, 0, 0, 1));
	shape.draw(@batch);
	batch.draw();
}

void update()
{
}