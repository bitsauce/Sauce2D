A funcA()
{
	return A();
}

class A
{
	int i = 0;
	void funcB()
	{
		i++;
	}
}

void main()
{
}

float time = 0.0f;
void draw()
{
	Math.cos(time);
	time += Graphics.dt;
}

void update()
{
	funcA().funcB();
}