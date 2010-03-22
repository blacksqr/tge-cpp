#include <iostream>
#include "UnitTestHelper.h"
#include <conio.h>
using namespace std;

int main()
{
    TestSuiteManager tm;
	tm.addSuite("Basic");
	tm.runAll(true);

	_getch();
    return 0;
}
