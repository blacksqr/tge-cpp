#pragma once

#include "tgeCommon.h"

namespace tge
{
	class Cmd
	{

	public:
		static Cmd* getUnknownCmd(void);

		Cmd(void);
		virtual ~Cmd(void);

		/** Override this method. 
			@note isNull() check should perform for arg1, arg2 and argRest to determine whether
			they really exist. The script engine will set the non-existing param null.
		*/
		virtual CmdStatus run(const String& cmdName, const Obj& arg1, const Obj& arg2, const Obj& argRest, size_t argRestc);

		void _setInterp(Interpreter* interp);

		Interpreter* _getInterp(void);

		/** Helper to set current cmd's result. */
		void _setObjResult(const Obj& result);

	protected:
		
	protected:
		String			mCmdName;
		/** This variable is set once registered the current command to some Interpreter. */
		Interpreter*	mInterp;
	};


	class UnknownCmd : public Cmd
	{
	public:
		// CmdStatus run(const String& cmdName, const Obj& argList, size_t argc);
		virtual CmdStatus run(const String& cmdName, const Obj& arg1, const Obj& arg2, const Obj& argRest, size_t argRestc);
	};
}