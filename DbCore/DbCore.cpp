////////////////////////////////////////////////////////////////////////////////////////
// DbCore.cpp : Implements NoSql Database.                                              //
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
* This package creates database i.e key/value pairs
* Also helps in removing keys,adding and deleting of children for specifies key
* Also suppports editing of meta data of value
* public Interfaces:
* ------------------
* testR1() : demonstrates that I have used C++11 version
* testR2() : demonstrates use of streams and operators new and delete
* testR3a(): creates dbelement and adds to database
* testR3b() : adds key/value pairs to database
* testR4b() : removes record from database
* testR5a() : removes child of a specified key
* testR5b() : adds child to a specified key
* testR5c() : supports editing of metadata
*
* Required Files:
* ---------------
* DbCore.h, DbCore.cpp
* DateTime.h, DateTime.cpp
* Utilities.h, Utilities.cpp
*
* Build Process:
* --------------
* devenv Cpp11 - NoSqlDb.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 6th feb 2018
*/

#include "DbCore.h"
#include "../Utilities/StringUtilities/StringUtilities.h"
#include "../Utilities/TestUtilities/TestUtilities.h"
#include <iostream>
#include <iomanip>
#include <functional>
#include <vector>

using namespace NoSqlDb;


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

	//----< demo requirement #1 >------------------------------------------
	 bool testR1()
	{
		Utilities::title("Demonstrating Requirement #1");
		std::cout << "\n  "
			<< typeid(std::function<bool()>).name()
			<< ", declared in this function, "
			<< "\n  is only valid for C++11 and later versions.";
		putLine();
		return true; // would not compile unless C++11
	}

	//----< demo requirement #2 >------------------------------------------

	 bool testR2()
	{
		Utilities::title("Demonstrating Requirement #2");
		std::cout << "\n  A visual examination of all the submitted code "
			<< "will show only\n  use of streams and operators new and delete.";
		putLine();
		return true;
	}

	//----< demo first part of requirement #3 >----------------------------

	 bool testR3a()
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
	//----< demo second part of requirement #3 and first requirement of #4 >---------------------------

	 bool testR3b()
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
		// display the results
		std::cout << "\n \n showing all the database elements:";
		showDb(db);
		std::cout << "\n \n   database keys are: ";
		showKeys(db);
		 dbp.db()=db;
		return true;
	}

	 //----< demo second requirement of #4 >---------------------------
	 bool testR4b()
	 {
		 Utilities::title("Demonstrating Requirement #4b - deleting keys");

		 DbProvider dbp;
		 DbCore<PayLoad> db = dbp.db();
		 if (db.contains("Prashar"))
		 {
			 std::cout << "\n Attempting to delete the key \"Prashar\" from the database ";
			 std::cout << "\n The key \"Prashar\" will be removed from all the child relationships also.";
			 db.removeRecord("Prashar");
			 dbp.db() = db;
			 putLine();
			 std::cout << "\n Database after deleting the key \"Prashar\" from the database is:";
			 showDb(db);
			 putLine();
			 return true;
		 }
		 return false;
	 }

	 //----< demo first part requirement of #5 >---------------------------
	 bool testR5a()
	 {
		 Utilities::title("Demonstrating Requirement #5a - removing a child of a specified key");

		 DbProvider dbp;
		 DbCore<PayLoad> db = dbp.db();
		 if (db.contains("Salman"))
		 {
			 std::cout << "\n Attempting to delete the child \"Arora\" from \"Salman\" children";
			 db.removeChild("Salman","Arora" );
			 dbp.db() = db;
			 putLine();
			 DbElement<PayLoad> demoElem = db["Salman"];
			 std::cout << "\n The DbElement of \"Salman\" after removing \"Arora\" from his children is:";
			 showElem(demoElem);
			 return true;
		 }
		 return false;
	 }

	 //----< demo second part requirement of #5 >---------------------------
	 bool testR5b()
	 {
		 Utilities::title("Demonstrating Requirement #5b - adding a child to a specified key");

		 DbProvider dbp;
		 DbCore<PayLoad> db = dbp.db();
		 if (db.contains("Salman"))
		 {
			 std::cout << "\n Attempting to add the child \"Arora\" to \"Salman\" children";
			 db.addChild("Salman", "Arora");
			 dbp.db() = db;
			 DbElement<PayLoad> demoElem = db["Salman"];
			 std::cout << "\n The DbElement of \"Salman\" after adding \"Arora\" to his children is:";
			 showElem(demoElem);
			 return true;
		 }
		 return false;
	 }

	 //----< demo third part requirement of #5 >---------------------------
	 bool testR5c()
	 {
		 Utilities::title("Demonstrating Requirement #5c - editing Metadata");

		 DbProvider dbp;
		 DbCore<PayLoad> db = dbp.db();
		 if (db.contains("Salman"))
		 {
			 std::cout << "\n The DbElement of \"Salman\" before editing metadata:";
			 showElem(db["Salman"]);
			 std::cout << " \n \nAttempting to edit metadata of \"Salman\"";
			 DbElement<PayLoad> dbElem = db["Salman"];
			 dbElem.name("Adarsh");
			 dbElem.dateTime(DateTime().now());
			 dbElem.descrip("The meta data is changed");
			 PayLoad payLoad;
			 payLoad.filePath("../FilePath.txt");
			 payLoad.categories().push_back("Student");
			 payLoad.categories().push_back("PhD");
			 dbElem.payLoad(payLoad);
			 db.saveValue("Salman", dbElem);
			 std::cout << "\n The DbElement of \"Salman\" after editing metadata:";
			 showHeader();
			 showElem(db["Salman"]);
			 dbp.db() = db;
			 putLine();
			 return true;
		 }
		 return false;
	 }


	//----< test stub >----------------------------------------------------

#ifdef TEST_DBCORE

	using namespace Utilities;

	int main()
	{
		 Utilities::Title("Testing DbCore - He said, she said database");
		 putLine();

		 TestExecutive ex;

		 // define test structures with test function and message

		 TestExecutive::TestStr ts1{ testR1, "Use C++11" };
		 TestExecutive::TestStr ts2{ testR2, "Use streams and new and delete" };
		 TestExecutive::TestStr ts3a{ testR3a, "Create DbElement<PayLoad>" };
		 TestExecutive::TestStr ts3b{ testR3b, "Create DbCore<PayLoad>" };
		 TestExecutive::TestStr ts4b{ testR4b, "Delete key DbCore<PayLoad>" };
		 TestExecutive::TestStr ts5a{ testR5a, "Delete child of a key DbCore<PayLoad>" };
		 TestExecutive::TestStr ts5b{ testR5b, "adding a child to a key's children" };
		 TestExecutive::TestStr ts5c{ testR5c, "Editing text meta data" };
		 // register test structures with TestExecutive instance, ex

		 ex.registerTest(ts1);
		 ex.registerTest(ts2);
		 ex.registerTest(ts3a);
		 ex.registerTest(ts3b);
		 ex.registerTest(ts4b);
		 ex.registerTest(ts5a);
		 ex.registerTest(ts5b);
		 ex.registerTest(ts5c);

		 // run tests

		 bool result = ex.doTests();
		 if (result == true)
		   std::cout << "\n  all tests passed";
		 else
		   std::cout << "\n  at least one test failed";

		 putLine(2);
		return 0;
	}
#endif
