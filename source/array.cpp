// Header
#include "x2d/math/array.h"
#include "x2d/scripts.h"
#include "scripts/scriptarray.h"

Array *CreateArray(const string &type, const uint size)
{
	// Create decl
	string decl = "array<";
	decl += type;
	decl += ">";

    // Obtain a pointer to the engine
	ScriptType *arrayType = scriptEngine->GetObjectTypeById(scriptEngine->GetTypeIdByDecl(decl.c_str()));

    // Create the array object
    return new CScriptArray(size, arrayType);
}