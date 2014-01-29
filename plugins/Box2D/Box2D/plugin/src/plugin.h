// Default plugin definition
extern "C"
{
       // Plugin factory function
       __declspec(dllexport) void CreatePlugin();
 
       // Plugin cleanup function
       __declspec(dllexport) void ReleasePlugin();
}