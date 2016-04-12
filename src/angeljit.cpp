#include "../include/angeljit.hpp"
#include <asmjit/asmjit.h>
#include <vector>
#include <memory>
#ifdef _ASMJIT_X86_H
#include "arch_x86.hpp"
#endif
using namespace asmjit;

namespace angeljit
{
	struct Compiler::priv
	{
		JitRuntime runtime;
		std::shared_ptr<Assembler> assembler;
	};

	Compiler::Compiler() : m_private(nullptr)
	{
		m_private = new priv();
	}

	int Compiler::CompileFunction(asIScriptFunction *function, asJITFunction *output)
	{
		asUINT   length;
		asDWORD *byteCode = function->GetByteCode(&length);
		if (byteCode == 0 || length == 0)
		{
			output = 0;
			return 1;
		}
		asDWORD *end = byteCode + length;

		//Call the architecture specific compile function
		compileFunc(byteCode, end,m_private->assembler,m_private->runtime);
			
		void* funcPtr = m_private->assembler->make();
		*output = asmjit_cast<asJITFunction>(funcPtr);

		return 0;
	}

	void Compiler::ReleaseJITFunction(asJITFunction func)
	{
		m_private->runtime.release(reinterpret_cast<void*>(func));
	}

	Compiler::~Compiler()
	{
		if (m_private)
			delete m_private;
	}
}