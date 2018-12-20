////////////////////////////////////////////////////////////////////////////////////////
// Query.cpp :  retrieve NoSqlDb contents                                    //
// ver 1.0                                                                            //
//                                                                                    //
//Language     : C++                                                                  //
// Platform    : Lenovo 510S Ideapad, Win Pro 10, Visual Studio 2017                  //
// Application : CSE-687 OOD Project 1                                                //
// Author      : Adarsh Venkatesh Bodineni,Syracuse University                        //
// Source      : Dr. Jim Fawcett, EECS, SU                                            //
////////////////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package supports queries on the database
* public Interfaces:
* ------------------
* testReq1() : creates an dbelement in database
* testReq2() : forms the database by adding key/value pairs
* testQuery() : supports queries on database
*
* Required Files:
* ---------------
* DbCore.h, DbCore.cpp
* DateTime.h, DateTime.cpp
* Utilities.h, Utilities.cpp
* Query.h, Query.cpp
*
* Build Process:
* --------------
* devenv Cpp11 - NoSqlDb.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 6th feb 2018
*/



#include "../Utilities/StringUtilities/StringUtilities.h"
#include "../Utilities/TestUtilities/TestUtilities.h"
#include <iostream>
#include <iomanip>
#include <functional>
#include <vector>
#include "../DbCore/DbCore.h"
#include "Query.h"
#include "../DateTime/DateTime.h"

using namespace NoSqlDb;
using namespace NoSqlQuery;

//----< reduce the number of characters to type >----------------------

auto putLine = [](size_t n = 1, std::ostream& out = std::cout)
{
	Utilities::putline(n, out);
};



///////////////////////////////////////////////////////////////////////
// DbProvider class
// - provides mechanism to share a test database between test functions
//   without explicitly passing as a function argument.

class DbProvider
{
public:
	DbCore<PayLoad>& db() { return db_; }

private:
	static DbCore<PayLoad> db_;
};
DbCore<PayLoad> DbProvider::db_;
 

///////////////////////////////////////////////////////////////////////
// test functions



//----<  Creates a db element with key >----------------------------

bool testReq1()
{
	// create database to hold std::string payload

	Utilities::title("Demonstrating Requirement #3a - creating DbElement");
	std::cout << "\n  Creating a db element with key \"Fawcett\":";

	// create database to hold payload

	DbCore<PayLoad> db;
	DbProvider dbp;
	dbp.db() = db;

	// create some demo elements and insert into db

	DbElement<PayLoad> demoElem;

	demoElem.name("Jim");
	demoElem.descrip("Instructor for CSE687");
	demoElem.dateTime(DateTime().now());
	PayLoad payload;
	payload.filePath("../Fawcett.txt");
	payload.categories().push_back("Prof.");
	payload.categories().push_back("Dr.");
	demoElem.payLoad(payload);

	if (demoElem.name() != "Jim")
		return false;
	if (demoElem.descrip() != "Instructor for CSE687")
		return false;
	if (demoElem.dateTime().now() != DateTime().now())
		return false;
	if (demoElem.payLoad().filePath() != "../Fawcett.txt")
		return false;
	if (demoElem.payLoad().categories().size() <= 0)
		return false;

	showHeader();
	showElem(demoElem);

	db["Fawcett"] = demoElem;
	dbp.db() = db;
	putLine();
	return true;

}
//----<  creates DbCore by adding key/value pairs >---------------------------

bool testReq2()
{
	Utilities::title("Demonstrating Requirement #3b and #4a- creating DbCore by adding key/value pairs");
	DbProvider dbp;
	DbCore<PayLoad> db = dbp.db();
	DbElement<PayLoad> demoElem = db["Fawcett"];
	demoElem.name("Ammar");
	demoElem.descrip("TA for CSE687");
	PayLoad payload;
	payload.filePath("../AmmarSalman.txt");
	payload.categories().push_back("TA");
	payload.categories().push_back("PhD");
	demoElem.payLoad(payload);
	db["Salman"] = demoElem;
	demoElem.name("Jianan");
	PayLoad payload1;
	payload1.filePath("../Jianan.txt");
	payload1.categories().push_back("TA");
	demoElem.payLoad(payload1);
	db["Sun"] = demoElem;
	DbElement<PayLoad> demoElem1 = db["Sun"];
	demoElem1.name("Nikhil");
	demoElem1.descrip("TA for OOD");
	db["Prashar"] = demoElem1;
	demoElem1.name("Pranjul");
	db["Arora"] = demoElem1;
	demoElem1.name("Akash");
	db["Anjanappa"] = demoElem1;
	std::cout << "\n  after adding elements with keys: ";
	DbCore<std::string>::Keys keys = db.keys();
	showKeys(db);
	std::cout << "\n \n make all the new elements children of element with key \"Fawcett\"";
	db["Fawcett"].children().push_back("Salman");
	db["Fawcett"].children().push_back("Sun");
	db["Fawcett"].children().push_back("Prashar");
	db["Fawcett"].children().push_back("Arora");
	db["Fawcett"].children().push_back("Anjanappa");
	showHeader();
	showElem(db["Fawcett"]);
	db["Salman"].children().push_back("Sun");
	db["Salman"].children().push_back("Prashar");
	db["Salman"].children().push_back("Arora");
	db["Salman"].children().push_back("Anjanappa");
	std::cout << "\n \n showing all the database elements:";
	showDb(db);
	std::cout << "\n \n database keys are: ";
	showKeys(db);
	dbp.db() = db;
	putLine();
	return true;
}

//----<  queries on database >---------------------------

	bool testQuery()
	{
		Utilities::title("Demonstrating Requirement #6 & #7 - Qeuries");
		DbProvider dbp;
		DbCore<PayLoad> db = dbp.db();
		Query<PayLoad> q1(db);

		q1.showValue( "Salman");
		q1.showChildren("Fawcett");
		q1.showMatchKeys(".*n.*");

		Query<PayLoad> q7(db);
		Conditions<PayLoad> conds1;
		conds1.name(".*i.*");
		q7.select(conds1);

		Query<PayLoad> q2(db);
		Conditions<PayLoad> conds2;
		conds2.descrip(".*TA.*");
		q2.select(conds2);

		Query<PayLoad> q3(db);
		DateTime dt = DateTime().now();
		DateTime::Duration dur = DateTime::makeDuration(0, 1, 0, 0);
		q3.selectDate(dt-dur);

		Query<PayLoad> q4(db);
		q4.selectDateInterval(dt - dur, dt + dur);
		std::cout << "\n\n--------------------------------------------------------------------------------------------";
		std::cout << "\n ****Testing conds2 from keys of q7 that has keys matched with the pattern \".*i.*\" ***";
		std::cout << "\n Below is demonstration of \"and\"ing of multiple queries";
		Query<PayLoad> q5(db);
		q5.from(q7.keys()).select(conds2);

		std::cout << "\n\n--------------------------------------------------------------------------------------------";
		std::cout << "\n Another way of \"and\"ing i.e applying different conditions on same query";
		Query<PayLoad> q6(db);
		q6.select(conds1).select(conds2);

		std::cout << "\n\n--------------------------------------------------------------------------------------------";
		std::cout << "\n ***Performing union of keys from the q1's keys and q7's keys***";
		std::cout << "\n Below is the demonstration of \"or\"ing of keys";
		q1.keysUnion(q7.keys()).show();

		Utilities::title("Demonstrating Requirement #9 - Payload that has filepath and vector of categories");
		Query<PayLoad> q8(db);
		q8.selectPayloadFilepath(".*Faw.*");
		Query<PayLoad> q9(db);
		q9.selectPayloadCategory("TA");
		return true;
	}


	//----< test stub >----------------------------------------------------
#ifdef QUERY

int main()
{
	Utilities::Title("Testing DbCore - He said, she said database");
	putLine();

	TestExecutive ex;

	// define test structures with test function and message

	TestExecutive::TestStr ts1{ testReq1, "Use C++11" };
	TestExecutive::TestStr ts2{ testReq2, "Use streams and new and delete" };
	TestExecutive::TestStr ts3a{ testQuery, "Queries " };
	//TestExecutive::TestStr ts3b{ testR3b, "Create DbCore<std::string>" };

	ex.registerTest(ts1);
	ex.registerTest(ts2);
	ex.registerTest(ts3a);
	//ex.registerTest(ts3b);


	bool result = ex.doTests();
	if (result == true)
		std::cout << "\n  all tests passed";
	else
		std::cout << "\n  at least one test failed";

	putLine(2);
	return 0;
}


#endif // Query



