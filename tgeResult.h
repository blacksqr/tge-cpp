#pragma once

#include "tgeCommon.h"

namespace tge
{
	/** Supports only one interpreter for efficiency.
	*/
	class Result
	{
		friend class Interpreter;

	public:
		Result(const Result& rhs);
		Result& operator=(const Result& rhs);

		bool	hasError() const;
		const String& getErroMsg(void);
		CmdStatus status() const;
		Obj&	  obj() const;

		// Not preferred
		Obj*	  objptr();	
	protected:
		Result(CmdStatus status);
	private:
		int32		mErrLn;
		CmdStatus	mStatus;
		Obj*		mResultObj;
	};
}