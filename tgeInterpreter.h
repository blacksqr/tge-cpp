#pragma once
#include "tgeCommon.h"
#include "tgeObj.h"
#include "tgeResult.h"

namespace tge 
{

	/** Wrapper for TCL interpreter. */
	class Interpreter
	{
	public:
		typedef std::map<String, Cmd*> CmdMap;

	public:
		Interpreter (void);
		virtual ~Interpreter(void);

		/** For effieciency, evaluate things all in GLOBAL namespace. */
		Result eval(Obj* objPtr) throw (tge::except);
		Result eval(const String& str) throw (tge::except);
		
		bool registerCmd(const String& name, Cmd* cmdObject);
		bool unregisterCmd(const String& name);
	

		/** Sets vars in GLOBAL space. Need to ref to them later with :: qualifiers.*/
		void setVar(const String& name, const Obj& obj);
		/** Retrieves a var matching name in GLOBAL space. */
		Obj& getVar(const String& name) throw(tge::except);
		void removeVar(const String& name);

		/** Queries if some commands exists. */
		bool commandExists(const String& cmdName);

		static Cmd*	getUnknownCmd(void);

		/** _Panic throws!
		*/
		void _panic(const String& msg, const String& wh = "") throw (except);
		Tcl_Interp*		_interp(void);
		/// link to TCL
		static int _objCmdProc(
			ClientData clientData,
			Tcl_Interp *interp,
			int objc,
			Tcl_Obj *const objv[]);
	protected:

	protected:
		static UnknownCmd* msUnknownCmd;
		Tcl_Interp*		mInterp;
		CmdStatus		mStatus;
		String			mError;

		CmdMap			mInternalCmdList;
	private:


	};
}