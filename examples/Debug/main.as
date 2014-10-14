class A
{
	int i = 0;
	
	A()
	{
		Console.log("Construct A");
	}
	
	void foo()
	{
		for(int i = 0; i < 10; i++)
		{
			bar();
		}
	}
	
	void bar()
	{
		i++;
	}
}

class B : A
{
	B()
	{
		super();
		Console.log("Construct B");
	}
}

A a();

float heavyFunction()
{
	float sum = 0;
	for(int i = 0; i < 10000; i++)
	{
		sum += Math.cos(i*0.1f)*Math.sin(i*0.2f);
	}
	return sum;
}

int lightFunciton()
{
	int sum = 0;
	for(int i = 0; i < 100; i++)
		sum += i;
	return sum;
}

void main()
{
	B b();
}

void draw()
{
	heavyFunction();
}

void update()
{
	for(int i=0; i < 2; i++)
		lightFunciton();
	a.foo();
}