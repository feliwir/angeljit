#pragma once
#include <angelscript.h>
#include <memory>
#include "asmjit/asmjit.h"

namespace angeljit
{
	int compileFunc(asDWORD* bc, asDWORD* end,std::shared_ptr<asmjit::Assembler>& assembler,asmjit::JitRuntime& runtime);
}