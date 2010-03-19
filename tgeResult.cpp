#include "tgeResult.h"
#include "tgeObj.h"
#include "tgeInterpreter.h"
#include <boost/lexical_cast.hpp>
namespace tge
{
	Result::Result(CmdStatus status) :
	mResultObj(new Obj), mStatus(status), mErrLn(-1)
	{
		Tcl_Obj* obj = Tcl_GetObjResult(mResultObj->getInterp()->_interp());
		mErrLn = mResultObj->getInterp()->_interp()->errorLine;
		mResultObj->_setTclObj(obj);
	}
	Result::Result(const Result& rhs)
	{
		mResultObj =  rhs.mResultObj;
		mResultObj->setInterp(rhs.mResultObj->getInterp());
		mStatus = rhs.mStatus;
		mErrLn = rhs.mErrLn;
	}
	Result& Result::operator=(const Result& rhs)
	{
		mResultObj = rhs.mResultObj;
		mResultObj->setInterp(rhs.mResultObj->getInterp());
		mStatus = rhs.mStatus;
		mErrLn = rhs.mErrLn;
		return *this;
	}
	bool	Result::hasError() const
	{
		return mStatus!=TGE_OK;
	}
	const String& Result::getErroMsg(void)
	{
		static String ss;
		
		if (hasError())
		{
			ss = "TGE Error: ";
			ss.append( static_cast<String>(*mResultObj) ).
			append("\n").
			append(" at line ").
			append(boost::lexical_cast<String>(mErrLn)).
			append(".");
		}
		else
		{
			ss = "";
		}
		return ss;
	}
	CmdStatus Result::status() const
	{
		return mStatus;
	}
	Obj&	  Result::obj() const
	{
		assert(mResultObj);
		return *mResultObj;
	}
	Obj*	  Result::objptr()
	{
		return mResultObj;
	}

		
}