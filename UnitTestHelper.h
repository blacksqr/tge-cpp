#include <UnitTest++.h>
#include <TestReporter.h>
#include <string>
#include <vector>
#include <algorithm>
using namespace UnitTest;
using namespace std;


class MyTestReporter : public TestReporter
{

private:
	virtual void ReportTestStart(TestDetails const& test)
	{
		cout<<"\tStart : "<<test.testName<<endl;
	}
	virtual void ReportFailure(TestDetails const& test, char const* failure)
	{
		cout<<"\t*!Failed: "<<test.testName<<" at ln "<<test.lineNumber<<" in "<<test.filename<<endl
			<<"\t*!Msg   : "<<test.testName<<" at ln "<<failure<<endl;
	}
	virtual void ReportTestFinish(TestDetails const& test, float secondsElapsed)
	{
		cout<<"\tFinish: "<<test.testName<<endl;
	}
	virtual void ReportSummary(int totalTestCount, int failedTestCount, int failureCount, float secondsElapsed)
	{
		if (failureCount > 0)
			printf("FAILURE: %d out of %d tests failed (%d failures).\n", failedTestCount, totalTestCount, failureCount);
		else
			printf("Success: %d tests passed.\n", totalTestCount);

		printf("Test time: %.2f seconds.\n", secondsElapsed);
	}

	int mCounter;

};

class TestSuiteManager
{
public:
	TestSuiteManager() :
	  mRunner( *(new MyTestReporter))
	{
		mSuites.clear();
	}
	void addSuite(const string& suiteName)
	{
		mSuites.push_back(suiteName);
	}
	void removeSuite(const string& suiteName)
	{
		vector<string>::iterator it = find(mSuites.begin(), mSuites.end(), suiteName);

		if (it!=mSuites.end())
		{
			mSuites.erase(it);
		}
	}
	void run(const string& suiteName)
	{
		vector<string>::iterator it = find(mSuites.begin(), mSuites.end(), suiteName);

		if (it!=mSuites.end())
		{
			mRunner.RunTestsIf(Test::GetTestList(), (*it).c_str(), True(), 0);
		}
	}
	void runAll(bool detail=false)
	{
		TestList tl = Test::GetTestList();
		int m = mSuites.size();
		for (int i = 0; i<m;++i)
		{
			cout<<(i+1)<<" * " << mSuites[i]<<" begin *"<<std::endl;
			mRunner.RunTestsIf(Test::GetTestList(), mSuites[i].c_str(), True(), 0);
			cout<<(i+1)<<" * " << mSuites[i]<<" end *"<<std::endl;
			cout<<endl;
		}
	}
protected:
	vector<string>	mSuites;
	TestRunner		mRunner;
};