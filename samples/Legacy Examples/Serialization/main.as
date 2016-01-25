class A : Serializable
{
	int i = 128;
	float f = 3.14f;
	bool b = false;
	string s = "Hello World";
	
	void serialize(StringStream &ss)
	{
		ss.write(i);
		ss.write(f);
		ss.write(b);
		ss.write(s);
	}
	
	void deserialize(StringStream &ss)
	{
		ss.read(i);
		ss.read(f);
		ss.read(b);
		ss.read(s);
	}
	
	void print()
	{
		Console.log(""+i);
		Console.log(""+f);
		Console.log(b?"true":"false");
		Console.log(s);
	}
}


void main()
{
	A a();
	
	a.i = 128;
	a.f = 3.14f;
	a.b = false;
	a.s = "Hello World";
	
	Console.log("a:");
	a.print();

	Scripts.serialize(@a, ":/a.obj");
	
	A @a2 = cast<A@>(Scripts.deserialize(":/a.obj"));
	
	Console.log("a2:");
	a2.print();
}

void draw()
{
}

void update()
{
}