void print()
{
	int i = 0;
	while(true)
	{
		Console.log("Loop count: " + ++i);
	}
}

Thread @logThread;
void main()
{
	@logThread = @Thread(@FuncCall("print"));
	logThread.start();
}

void draw()
{
}

void update()
{
	Console.log("Hello World");
}