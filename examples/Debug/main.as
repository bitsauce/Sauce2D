float getPI()
{
	return 3.14f;
}

void main()
{
	float sum = 0;
	for(int i = 0; i < 10000; i++)
	{
		sum += Math.cos(i*0.1f)*Math.sin(i*0.2f);
	}
	Console.log("sum: "+sum);
}

void draw()
{
	Console.log("Draw");
}

void update()
{
	Console.log("Update");
}