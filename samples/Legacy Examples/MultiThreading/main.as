void print()
{
	Console.log("Hello World!");
}

class KeyListener
{
	void waitForKeypress(VirtualKey a)
	{
		while(!Input.getKeyState(a));
	}
}

void main()
{
	// Test call simple function on thread
	/*{
		thread th(funccall(@print));
	}*/
	
	// Test call method on thread
	{
		KeyListener k;
		funccall call(@k, "void waitForKeypress(VirtualKey)");
		call.setArg(0, KEY_G);
		
		Console.log("Listening...");
		thread th(call);
		th.join();
		Console.log("Key pressed");
	}
	
}

void draw()
{
}

void update()
{
}