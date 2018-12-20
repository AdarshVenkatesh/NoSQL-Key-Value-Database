////////////////////////////////////////////////////////////////////////////////////////
// Persist.cpp : store and retrieve NoSqlDb contents                                  //
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
* This package creates xml format of database and also helps to
* retrieve it back to key/value pair database
* public Interfaces:
* ------------------
* testReq1() : creates an dbelement in database
* testReq2() : forms the database by adding key/value pairs
* testPersist() : persists and retrieves database
* createPackageStructureXml1() : creates package structure for the project
* createPackageStructureXml2() : creates package structure for the project
* Required Files:
* ---------------
* DbCore.h, DbCore.cpp
* DateTime.h, DateTime.cpp
* Utilities.h, Utilities.cpp
* Persist.h, Persist.cpp
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
#include "../DateTime/DateTime.h"
#include "Persist.h"

using namespace NoSqlDb;
using namespace PersistDb;
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



//----< Creates a db element with key >----------------------------

bool testReq1()
{
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
//----< creates DbCore by adding key/value pairs >---------------------------

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
	DbCore<PayLoad>::Keys keys = db.keys();
	showKeys(db);
	std::cout << "\n\n  make all the new elements children of element with key \"Fawcett\"";
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
	std::cout << "\n  \n database keys are: ";
	showKeys(db);
	dbp.db() = db;
	putLine();
	return true;
}

//----< persists and retrieves database >---------------------------
bool testPersist()
{

	DbProvider dbp;
	DbCore<PayLoad> db = dbp.db();
	Persist<PayLoad> p(db);
	Utilities::Title("Persisting test db to file DbXml.xml");
	if (p.saveToFile("DbXml.xml"))
		std::cout << "\n  db saved";
	else
		std::cout << "\n  filesave failed";

	p.restoreFromFile("DbXml.xml");
	return true;
}

//----< creates package structure of project in xml format >---------------------------
bool createPackageStructureXml1()
{
	DbCore<PayLoad> db;
	DbProvider dbp;
	dbp.db() = db;
	DbElement<PayLoad> demoElem7;
	demoElem7.name("Date Time Package");
	demoElem7.descrip("Provides date time format");
	demoElem7.dateTime(DateTime().now());
	PayLoad payload7;
	payload7.filePath("../DateTime");
	payload7.categories().push_back("Date");
	payload7.categories().push_back("Time");
	demoElem7.payLoad(payload7);
	db["DateTime"] = demoElem7;
	DbElement<PayLoad> demoElem = db["DateTime"];
	demoElem.name("DbCore Package");
	demoElem.descrip("Holds DbCore,DbElement and Payload");
	PayLoad payload;
	payload.filePath("../DbCore");
	payload.categories().push_back("DbCore");
	payload.categories().push_back("DbElement");
	payload.categories().push_back("PayLoad");
	demoElem.payLoad(payload);
	db["DbCore"] = demoElem;
	db["DbCore"].children().push_back("DateTime");
	db["DbCore"].children().push_back("Utilities");
	DbElement<PayLoad> demoElem1;
	demoElem1.name("Executive package");
	demoElem1.descrip("Starting point of Project");
	PayLoad payload1;
	payload1.filePath("../Executive");
	payload1.categories().push_back("Execute");
	demoElem1.payLoad(payload1);
	db["Executive"] = demoElem1;
	db["Executive"].children().push_back("Test");
	DbElement<PayLoad> demoElem2;
	demoElem2.name("Persist package");
	demoElem2.descrip("Used to persist data into xml and retrieve back");
	PayLoad payload2;
	payload2.filePath("../Persist");
	payload2.categories().push_back("Persist");
	demoElem2.payLoad(payload2);
	db["Persist"] = demoElem2;
	db["Persist"].children().push_back("DbCore");
	db["Persist"].children().push_back("XmlDocument");
	return true;
	}

//----< creates package structure of project in xml format >---------------------------
bool createPackageStructureXml2()
{
	DbCore<PayLoad> db;
	DbProvider dbp;
	dbp.db() = db;
	DbElement<PayLoad> demoElem3;
	demoElem3.name("Query package");
	demoElem3.descrip("Used to perform Queries on database");
	PayLoad payload3;
	payload3.filePath("../Query");
	payload3.categories().push_back("Query");
	demoElem3.payLoad(payload3);
	db["Query"] = demoElem3;
	db["Query"].children().push_back("DbCore");
	db["Query"].children().push_back("DateTime");

	DbElement<PayLoad> demoElem4;
	demoElem4.name("Test package");
	demoElem4.descrip("Contains all the tests");
	PayLoad payload4;
	payload4.filePath("../Test");
	payload4.categories().push_back("Test");
	demoElem4.payLoad(payload4);
	db["Test"] = demoElem4;
	db["Test"].children().push_back("DbCore");
	db["Test"].children().push_back("DateTime");
	db["Test"].children().push_back("Query");
	db["Test"].children().push_back("Persist");

	DbElement<PayLoad> demoElem5;
	demoElem5.name("XmlDocument package");
	demoElem5.descrip("Helps in XDocument functions");
	PayLoad payload5;
	payload5.filePath("../XmlDocument");
	payload5.categories().push_back("XmlDocument");
	demoElem5.payLoad(payload5);
	db["XmlDocument"] = demoElem5;
	db["XmlDocument"].children().push_back("XmlElement");
	db["XmlDocument"].children().push_back("XmlParser");
	db["XmlDocument"].children().push_back("XmlElementParts");
	return true;

}

//----< test stub >----------------------------------------------------

using namespace Utilities;
#ifdef PERSIST
int main()
{
	Utilities::Title("Testing DbCore - He said, she said database");
	putLine();

	TestExecutive ex;

	// define test structures with test function and message

	TestExecutive::TestStr ts1{ testReq1, "Use C++11" };
	TestExecutive::TestStr ts2{ testReq2, "Use streams and new and delete" };
	TestExecutive::TestStr ts3a{ testPersist, "Queries " };
	TestExecutive::TestStr ts4{ createPackageStructureXml1, "Package structure xml"};

	ex.registerTest(ts1);
	ex.registerTest(ts2);
	ex.registerTest(ts3a);
	ex.registerTest(ts4);


	bool result = ex.doTests();
	if (result == true)
		std::cout << "\n  all tests passed";
	else
		std::cout << "\n  at least one test failed";

	putLine(2);
	return 0;
}
#endif // PERSIST


