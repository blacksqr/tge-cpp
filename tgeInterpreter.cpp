#include "tgeInterpreter.h"
#include "tgeCmd.h"

#include <exception>
// Unnamed namespace 
namespace 
{
int _unknownCmdProc(
		ClientData clientData,
		Tcl_Interp *interp,
		int objc,
		Tcl_Obj *const objv[])
{	
	tge::String name(Tcl_GetString(objv[1]));
	tge::Interpreter* interpreter = static_cast<tge::Interpreter*>(clientData);
	interpreter->_panic("Unknown command : " + name);
	return tge::TGE_OK;
}


}
namespace tge
{

	UnknownCmd* Interpreter::msUnknownCmd = NULL;

	Interpreter::Interpreter(void) :
	mInterp(Tcl_CreateInterp())
	{

		Tcl_CreateObjCommand(
			mInterp, 
			"unknown", 
			_unknownCmdProc, 
			(void*)this, 
			NULL
			);
		// Set recursion limit to 50
		Tcl_SetRecursionLimit(mInterp, 50);

	}
	Interpreter::~Interpreter(void)
	{
		// Tcl_DeleteInterp(mInterp);
	}

	Result Interpreter::eval(Obj* objPtr)
	{
		if (objPtr==NULL)
		{
			String errmsg;
			errmsg.append("Evaluation of NULL Tcl Object exception. ").
				append(" Interpreter::eval(Obj* objPtr) ");
			throw tge::except(errmsg);

		}
		// 
		Result r((CmdStatus)Tcl_EvalObjEx(mInterp, objPtr->_getTclObj(), 0));
		if (r.hasError())
		{
			
			throw tge::except(r.getErroMsg());
		}
		else
		{
			
		}
		return r;
	}

	Result Interpreter::eval(const String& script)
	{
		Result r((CmdStatus)Tcl_Eval(mInterp, script.c_str()));
		if (r.hasError())
		{
			
			throw tge::except(r.getErroMsg());
		}
		else
		{
			
		}
		return r;
	}

	bool Interpreter::commandExists(const String& cmdName)
	{
		Tcl_CmdInfo inf;
		int r = Tcl_GetCommandInfo(mInterp, cmdName.c_str(), &inf);
		return (r!=0);
	}

	bool Interpreter::registerCmd(const String& name, Cmd* cmdObject)
	{
		if ( commandExists(name) )
		{
			_panic("Command " + name+" already registered.");
			return false;
		}
		Tcl_Command tc = Tcl_CreateObjCommand(
			mInterp, 
			name.c_str(), 
			_objCmdProc, 
			(void*)this, 
			NULL
			);

		cmdObject->_setInterp(this);
		mInternalCmdList.insert(std::make_pair(name, cmdObject));
		return true;
	}
	bool Interpreter::unregisterCmd(const String& name)
	{
		CmdMap::iterator it = mInternalCmdList.find(name);
		if (it!=mInternalCmdList.end())
		{
			mInternalCmdList.erase(it);
		}
		return TCL_OK==Tcl_DeleteCommand(mInterp, name.c_str());
	}

	void Interpreter::setVar(const String& name, const Obj& obj)
	{
		// char * buf = Tcl_Alloc(obj._getTclObj()->length);
		Tcl_Obj* to = Tcl_DuplicateObj(obj._getTclObj());
		Tcl_IncrRefCount(to);
		Tcl_SetVar2Ex(mInterp, name.c_str(), NULL, to , TCL_GLOBAL_ONLY);
	}
	Obj& Interpreter::getVar(const String& name)
	{
		static Obj obj;
		Tcl_Obj* to = Tcl_GetVar2Ex(mInterp, name.c_str(), NULL, TCL_GLOBAL_ONLY);
		obj.setInterp(this);
		if (to==NULL)
		{
			String errmsg;
			errmsg.append("Variable ").append(name).append(" not exits.");
			throw tge::except( errmsg );
			obj.setNull();
		}
		else
		{
			obj._setTclObj(to);
		}
		return obj;
	}
	void Interpreter::removeVar(const String& name)
	{
		Tcl_UnsetVar(mInterp, name.c_str(), TCL_GLOBAL_ONLY);
	}

	Cmd* Interpreter::getUnknownCmd(void)
	{
		if (NULL==msUnknownCmd)
		{
			msUnknownCmd = new UnknownCmd;
		}
		return msUnknownCmd;
	}


	void Interpreter::_panic(const String& msg,  const String& wh )  throw (except)
	{
		// Tcl_Panic("Internal Error: %s", msg.c_str());
		char szTmp[256];
		sprintf(szTmp, "TGE Error : %s  - [%s]", msg.c_str(), wh.c_str());
		throw tge::except(szTmp);
	}
	Tcl_Interp* Interpreter::_interp(void)
	{
		return mInterp;
	}
	int Interpreter::_objCmdProc(
		ClientData clientData,
		Tcl_Interp *interp,
		int objc,
		Tcl_Obj *const objv[])
	{
		String name(Tcl_GetString(objv[0]));
		Interpreter* interpreter = static_cast<Interpreter*>(clientData);
		Cmd* cmd = interpreter->mInternalCmdList[name];
		if ( NULL == cmd)
		{
			cmd = getUnknownCmd();
		}
		// 1 ~ N as list
		Obj arg1;
		Obj arg2;
		Obj obj;

		arg1.setNull();
		arg2.setNull();
		obj.setNull();

		size_t count = 0;

		if (objc>1)
		{
			arg1._setTclObj(Tcl_DuplicateObj(objv[1]));
			count = 0;
		}
		if (objc>2)
		{
			arg2._setTclObj(Tcl_DuplicateObj(objv[2]));
			count = 0;
		}
		if (objc>3)
		{
			count = objc - 3;
			// Here new list is a bottleneck!! TODO : optimise this in future
			Tcl_Obj* nlist = Tcl_DuplicateObj ( Tcl_NewListObj(count, &objv[3]) );
			obj._setTclObj(nlist);
		}
		assert(interp == interpreter->_interp()) ;
		CmdStatus ccc = cmd->run(name, arg1, arg2, obj, count);
		interpreter->mStatus = ccc;
		return ccc;
	}
}