#include "tgeCommon.h"
#include "UnitTestHelper.h"
#include "tgeEngine.h"
#include "tgeInterpreter.h"
using namespace tge;

SUITE(Basic)
{
	TEST(t01)
	{
		Engine engine;
		Interpreter* i = engine.getInterpreter("Default");
		
		try
		{
			i->eval("Deafult interp evals.");
			engine.createInterpreter("First");

			// engine.createInterpreter("First");
		}
		catch (const tge::except& e)
		{
			e.out();
		}
	}
}