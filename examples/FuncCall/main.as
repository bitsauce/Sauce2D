class Creature
{
	int level;

	// Returns previous level
	int incLevel(int lvl)
	{
		int prevLevel = level;
		level += lvl;
		Console.log("Current Level: " + level);
		return lvl;
	}
}

string print(string str)
{
	Console.log(str);
	return str + " [printed]";
}

funcdef void FunctionCallback(string);

class DelegateCaller
{
	DelegateCaller()
	{
		funccall call(@FunctionCallback(print));
		call.setArg(0, "Hello World");
		call.execute();
	}

	void print(string str)
	{
		Console.log(str);
	}
}

void main()
{
	// Test calling global script function
	{
		funccall call("string print(string)");
		call.setArg(0, "Hello World");
		call.execute();
		
		string str;
		call.getReturn(str);
		Console.log(str);
	}
	
	// Test calling script object method
	{
		Creature creature;
		creature.level = 10;
	
		funccall call(@creature, "int incLevel(int)");
		call.setArg(0, 5);
		call.execute();
		
		int level;
		call.getReturn(level);
		Console.log("Previous Level: "+level);
	}
	
	// Test calling application object method
	{
		array<int> integers(10);
		for(int i = 0; i < integers.size; i++)
			integers[i] = Math.getRandomInt();
		
		funccall call(@integers, "int find(const int&in) const");
		call.setArg(0, integers[5]);
		call.execute();
		
		int find;
		call.getReturn(find);
		Console.log("Found: "+find+" (5 is correct)");
	}
	
	// Test calling function delegate
	{
		DelegateCaller c();
	}
}

void draw()
{
}

void update()
{
}