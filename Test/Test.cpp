////////////////////////////////////////////////////////////////////////////////////////
// Test.cpp : Contains all tests                                                      //
// ver 1.0                                                                            //
//                                                                                    //
//Language     : C++                                                                  //
// Platform    : Lenovo 510S Ideapad, Win Pro 10, Visual Studio 2017                  //
// Application : CSE-687 OOD Project 1                                                //
// Author      : Adarsh Venkatesh Bodineni,Syracuse University                        //
// Source      : Dr. Jim Fawcett, EECS, SU                                            //
////////////////////////////////////////////////////////////////////////////////////////
/*
* Package Operations :
*-------------------
* Contains all the tests that demonstrates each requirement.
*Required Files :
*-------------- -
* DateTime.h, DateTime.cpp
* Utilities.h, Utilities.cpp
* Test.h, Test.cpp
*
* Build Process :
*--------------
* devenv Cpp11 - NoSqlDb.sln / rebuild debug
*
* Maintenance History :
*--------------------
* ver 1.0 : 6th feb 2018
*/
#include "Test.h"
#include "../Utilities/TestUtilities/TestUtilities.h"
using namespace Test;


//----< test stub >----------------------------------------------------
#ifdef TEST

int main()
{
	Utilities::Title("Testing DbCore - He said, she said database");
	putLine();

	TestExecutive ex;
	// define test structures with test function and message

	TestExecutive::TestStr ts1{ testR1, "Use C++11" };
	TestExecutive::TestStr ts2{ testR2, "Use streams and new and delete" };
	TestExecutive::TestStr ts3a{testR3a, "Create DbElement<PayLoad>" };
	TestExecutive::TestStr ts3b{ testR3b, "Create DbCore<PayLoad>" };
	TestExecutive::TestStr ts4b{ testR4b, "Delete key DbCore<PayLoad>" };
	TestExecutive::TestStr ts5a{ testR5a, "Delete child of a key DbCore<PayLoad>" };
	TestExecutive::TestStr ts5b{ testR5b, "Adding a child to a key's children" };
	TestExecutive::TestStr ts5c{ testR5c, "Editing text meta data" };
	TestExecutive::TestStr ts6n7{ testQuery, "Queries " };
	TestExecutive::TestStr ts8{ testPersist, "Persisting the database into Xml and restoring back " };
	// register test structures with TestExecutive instance, ex

	ex.registerTest(ts1);
	ex.registerTest(ts2);
	ex.registerTest(ts3a);
	ex.registerTest(ts3b);
	ex.registerTest(ts4b);
	ex.registerTest(ts5a);
	ex.registerTest(ts5b);
	ex.registerTest(ts5c);
	ex.registerTest(ts6n7);
	ex.registerTest(ts8);

	// run tests

	bool result = ex.doTests();
	if (result == true)
		std::cout << "\n  all tests passed";
	else
		std::cout << "\n  at least one test failed";

	putLine(2);
	return 0;
}

#endif // TEST



