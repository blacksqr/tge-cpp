#include "tgeCmd.h"
#include "tgeInterpreter.h"

namespace tge
{
	Cmd::Cmd(void)
	{
	}
	Cmd::~Cmd(void)
	{

	}
	CmdStatus Cmd::run(const String& cmdName, const Obj& arg1, const Obj& arg2, const Obj& argRest, size_t argRestc)
	{
		return TGE_OK;
	}
	void Cmd::_setObjResult(const Obj& result)
	{
		assert(mInterp);
		Tcl_SetObjResult(mInterp->_interp(), result._getTclObj());
	}

	void Cmd::_setInterp(Interpreter* interp)
	{
		mInterp = interp;
	}
	Interpreter* Cmd::_getInterp(void)
	{
		return mInterp;
	}


	

	CmdStatus UnknownCmd::run(const String& cmdName, const Obj& arg1, const Obj& arg2, const Obj& argRest, size_t argRestc)
	{
		mInterp->_panic("Command " + cmdName + " not registered!");
		return TGE_OK;
	}

}