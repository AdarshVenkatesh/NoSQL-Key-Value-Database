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
* testQuery() : supports querying on database
* testPersist() : supports persisting and retrieving of database from and to xml tespectively
* Required Files:
* ---------------
* DbCore.h, DbCore.cpp
* DateTime.h, DateTime.cpp
* Utilities.h, Utilities.cpp
* Persist.h,Persist.cpp
* Query.h,Query.cpp
*
* Build Process:
* --------------
* devenv Cpp11 - NoSqlDb.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 6th feb 2018
*/

#include "../Utilities/CodeUtilities/CodeUtilities.h"
#include "../DbCore/DbCore.h"
#include "../Utilities/StringUtilities/StringUtilities.h"
#include "../Utilities/TestUtilities/TestUtilities.h"
#include <iostream>
#include <iomanip>
#include <functional>
#include <vector>
#include "../DateTime/DateTime.h"
#include "../Query/Query.h"
#include "../Persist/Persist.h"
#include "../XmlDocument/XmlDocument.h"
#include "../XmlDocument/XmlElement.h"

using namespace XmlProcessing;
using namespace NoSqlQuery;
using namespace NoSqlDb;
using namespace PersistDb;
namespace Test
{

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
		std::cout << "\n \n showing all the database elements:";
		showDb(db);
		std::cout << "\n\n  database keys are: ";
		showKeys(db);
		dbp.db() = db;
		putLine();
		return true;
	}

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
	bool testR5a()
	{
		Utilities::title("Demonstrating Requirement #5a - removing a child of a specified key");

		DbProvider dbp;
		DbCore<PayLoad> db = dbp.db();
		if (db.contains("Salman"))
		{
			std::cout << "\n Attempting to delete the child \"Arora\" from \"Salman\" children";
			db.removeChild("Salman", "Arora");
			dbp.db() = db;
			putLine();
			DbElement<PayLoad> demoElem = db["Salman"];
			std::cout << "\n The DbElement of \"Salman\" after removing \"Arora\" from his children is:";
			showElem(demoElem);
			return true;
		}
		return false;
	}
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

	//----<  queries on database >---------------------------
	bool testQuery()
	{
		Utilities::title("Demonstrating Requirement #6 & #7- Qeuries");
		DbProvider dbp;
		DbCore<PayLoad> db = dbp.db();
		Query<PayLoad> q1(db);

		q1.showValue("Salman");
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
		q3.selectDate(dt - dur);

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

	//----< persists and retrieves database >---------------------------
	bool testPersist()
	{
		Utilities::title("Demonstrating Requirement #8 - persisting database in xml and restoring it back");
		DbProvider dbp;
		DbCore<PayLoad> db = dbp.db();
		Persist<PayLoad> p(db);
		Utilities::Title("Persisting test db to file DbXml.xml");
		if (p.saveToFile("DbXml.xml"))
			std::cout << "\n  db saved";
		else
			std::cout << "\n  filesave failed";

		std::cout << "\n The DbXml.xml is stored in Executive package";
			
		p.restoreFromFile("DbXml.xml");

		std::cout << "\n\n The PackageStructureXml.xml is stored in root folder\n \n";
		return true;
	}
}