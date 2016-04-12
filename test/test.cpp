#include <angelscript.h>
#include <scriptstdstring/scriptstdstring.h>
#include <scriptbuilder/scriptbuilder.h>
#include <angeljit.hpp>

// Implement a simple message callback function
void MessageCallback(const asSMessageInfo *msg, void *param)
{
	const char *type = "ERR ";
	if (msg->type == asMSGTYPE_WARNING)
		type = "WARN";
	else if (msg->type == asMSGTYPE_INFORMATION)
		type = "INFO";
	printf("%s (%d, %d) : %s : %s\n", msg->section, msg->row, msg->col, type, msg->message);
}

int main(int argc, char**)
{
	asIScriptEngine* engine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
	const char* ver = asGetLibraryVersion();
	engine->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL);

	//Create the JIT Compiler. The build flags are explained below,
	//as well as in as_jit.h
	angeljit::Compiler* jit = new angeljit::Compiler();

	//Enable JIT helper instructions; without these,
	//the JIT will not be invoked
	engine->SetEngineProperty(asEP_INCLUDE_JIT_INSTRUCTIONS, 1);

	//Bind the JIT compiler to the engine
	engine->SetJITCompiler(jit);

	CScriptBuilder builder;
	int r = builder.StartNewModule(engine, "test");
	if (r < 0)
	{
		// If the code fails here it is usually because there
		// is no more memory to allocate the module
		printf("Unrecoverable error while starting a new module.\n");
		return -1;
	}
	r = builder.AddSectionFromFile("test.as");
	if (r < 0)
	{
		printf("Please correct the errors in the script and try again.\n");
		return -1;
	}
	r = builder.BuildModule();
	if (r < 0)
	{
		printf("Please correct the errors in the script and try again.\n");
		return -1;
	}

	asIScriptModule *mod = engine->GetModule("test");
	asIScriptFunction *func = mod->GetFunctionByDecl("void main()");
	if (func == 0)
	{
		printf("The script must have the function 'void main()'. Please add it and try again.\n");
		return -1;
	}

	asIScriptContext *ctx = engine->CreateContext();
	ctx->Prepare(func);
	r = ctx->Execute();
	if (r != asEXECUTION_FINISHED)
	{
		// The execution didn't complete as expected. Determine what happened.
		if (r == asEXECUTION_EXCEPTION)
		{
			// An exception occurred, let the script writer know what happened so it can be corrected.
			printf("An exception '%s' occurred. Please correct the code and try again.\n", ctx->GetExceptionString());
		}
	}

	ctx->Release();
	engine->ShutDownAndRelease();
	delete jit;

	return 0;
}