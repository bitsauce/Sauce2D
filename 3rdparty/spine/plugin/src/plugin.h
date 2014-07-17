// Default plugin definition
extern "C"
{
       // Plugin factory function
       __declspec(dllexport) int CreatePlugin(class xdEngine*);
 
       // Plugin cleanup function
       __declspec(dllexport) void ReleasePlugin();
}

extern xdEngine *xdengine;