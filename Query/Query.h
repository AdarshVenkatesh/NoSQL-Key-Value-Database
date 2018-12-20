////////////////////////////////////////////////////////////////////////////////////////
// Query.h :  retrieve NoSqlDb contents                                    //
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
* show() : displays the keys that matches with the condition specified
* keys() : returns keys of that query class
* select() : supports querying on metadata by accepting condition
* showValue() : shows value of specified key
* showChildren() : displays children of specified key
* showMatchKeys() : displays matched keys by performing reg pattern query on keys
* selectName() : supports querying on metadata name
* selectDesc() supports querying on metadata description
* selectPayloadFilepath() : supports querying on payload filepath
* selectPayloadCategory() : supports querying on payload category
* selectDateInterval() : supports querying on metadata date when interval specified
* selectDate() : supports querying on metadata date when one end of interval specified
* keysUnion() : unions keys of two query classes
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
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include "../DbCore/DbCore.h"
#include <regex>
#include "../DateTime/DateTime.h"

using namespace NoSqlDb;

namespace NoSqlQuery
{
	/////////////////////////////////////////////////////////////////////
	// Condition class
	// -  provide condition class to support select functionality
	template<typename T>
	class Conditions
	{
	private:
		std::string name_;
		std::string descrip_;
		DateTime date_;
		T payLoad_;
	public:

		std::string name() const { return name_; }
		std::string& name() { return name_; }
		Conditions<T>& name(const std::string& name) { name_ = name; return *this; }

		std::string descrip() const { return descrip_; }
		std::string& descrip() { return descrip_; }
		Conditions<T>& descrip(const std::string& descrip) { descrip_ = descrip; return *this; }

		T payLoad() const { return descrip_; }
		T& payLoad() { return descrip_; }
		Conditions<T>& payLoad(const T& payLoad) { payLoad_ = payLoad; return *this; }

		DateTime date() const { return date_; }
		DateTime& date() { return date_; }
		Conditions<T>& date(const DateTime& date) { date_ = date; return *this; }

	};

	/////////////////////////////////////////////////////////////////////
	// Query class
	// -  provide querying on database functionalities

	template <typename T>
	class Query
	{
	public:
		using Key = std::string;
		using Keys = std::vector<Key>;
		Query<T>& from(Keys& keys);
		Query(const DbCore<T>& dbCore) { db_ = dbCore; keys_ = db_.keys(); }
		void Query<T>::show();
		Keys& keys();
		Query<T>& select(const Conditions<T>& conds);
		Query<T>& showValue(const std::string& arg);
		Query<T>& showChildren(const std::string& arg);
		Query<T>& showMatchKeys(const std::string& arg);
		Query<T>& selectName(const std::string& arg);
		Query<T>& selectDesc(const std::string& arg);
		Query<T>& selectPayloadFilepath(const std::string& arg);
		Query<T>& selectPayloadCategory(const std::string& arg);
		Query<T>& selectDateInterval(DateTime dt1, DateTime dt2);
		Query<T>& selectDate(DateTime dt1);
		Query<T>& keysUnion(Keys& keys);


	private:
		DbCore<PayLoad>::DbStore dbStore_;
		DbCore<T> db_;
		Keys keys_;
	};

	//supports queries on condition class
	template <typename T>
	Query<T>& Query<T>::select(const Conditions<T>& conds)
	{
		std::string name = conds.name();
		std::string descrip = conds.descrip();
		DateTime date = conds.date();
		if (!name.empty())
			selectName(name);

		if (!descrip.empty())
			selectDesc(descrip);

		return *this;
	}

	//unions keys of two query classes
	template <typename T>
	Query<T>& Query<T>::keysUnion(Keys& keys)
	{
		Query<T>::Keys keys2 = keys;
		for (auto k : keys2)
		{
			if (std::find(keys_.begin(), keys_.end(), k) != keys_.end())
			{
				continue;
			}
			else
				keys_.push_back(k);
		}
		return *this;
	}


	//prints the value for a specified key 

	template <typename T>
	Query<T>& Query<T>::showValue(const std::string& arg)
	{
		DbCore<T>::DbStore dbStore_ = db_.dbStore();
		std::cout << "\n\n--------------------------------------------------------";
		std::cout << "\n searching the key \"" << arg << "\" in the database to retrieve its value(DbElement)";
		if (db_.contains(arg))
		{
			std::cout << "\n found the key \"" << arg << "\" in the database" << "\n The value for the key is";
			DbElement<T>& dbElem = dbStore_[arg];
			putLine();
			showHeader();
			showElem(dbElem);
		}
		else
			std::cout << "\n  didn't found the key \"" << arg << "\" in the database";
		return *this;
	}


	//prints the children for a specified key
	template <typename T>
	Query<T>& Query<T>::showChildren(const std::string& arg)
	{
		DbCore<T>::DbStore dbStore_ = db_.dbStore();
		std::cout << "\n\n--------------------------------------------------------";
		std::cout << "\n searching the key \"" << arg << "\" in the database to retireve his children";
		if (db_.contains(arg))
		{
			std::cout << "\n found the key \"" << arg << "\" in the database" << "\n The children for the key are";
			typename DbElement<T>::Children children = dbStore_[arg].children();  // note Keys& - we don't want copy of children
			if (children.size() > 0)
			{
				std::cout << "\n    child keys: ";
				for (auto key : children)
				{
					std::cout << " " << key;
				}
			}
			else
				std::cout << "\n There are no children for the key " << arg;
		}
		else
			std::cout << "\n  didn't found the key \"" << arg << "\" in the database";
		return *this;
	}


	//prints the keys that match with the regular expression pattern
	template <typename T>
	Query<T>& Query<T>::showMatchKeys(const std::string& arg)
	{
		DbCore<T>::Keys keys = db_.keys();
		Query<T>::Keys resultKeys;
		std::cout << "\n\n--------------------------------------------------------";
		std::cout << "\n Searching all the keys to find keys that match with pattern \"" << arg << "\"";
		std::regex regx(arg);
		for (DbCore<T>::Key k : keys)
		{
			if (std::regex_match(k, regx))
			{
				resultKeys.push_back(k);
			}
		}

		if (resultKeys.size() > 0)
		{
			std::cout << "\n The matched keys with the given pattern are : ";
			for (DbCore<T>::Key k : resultKeys)
				std::cout << k << " ";
		}
		else
			std::cout << "\n    didn't find match in the keys for the pattern  \"" << arg << "\"";
		if (resultKeys.size() > 0)
			keys_ = resultKeys;

		return *this;
	}


	//prints the keys that match with the regular expression pattern in the meta data's name
	template <typename T>
	Query<T>& Query<T>::selectName(const std::string& arg)
	{
		DbCore<T>::DbStore dbStore_ = db_.dbStore();
		DbCore<T>::Keys keys = keys_;
		keys_.clear();
		std::cout << "\n\n--------------------------------------------------------";
		std::cout << "\n  Searching  metadata names that will match the pattern \"" << arg << "\"";
		std::regex regx(arg);
		for (DbCore<T>::Key k : keys)
		{
			DbElement<T> dbElem = dbStore_[k];
			std::string dbElemName = dbElem.name();
			if (std::regex_match(dbElemName, regx))
			{
				keys_.push_back(k);
			}
		}
		if (keys_.size() > 0)
		{
			std::cout << "\n found keys whose metadata name matches with the pattern \"" << arg << "\"\n The matched keys are:\n";
			for (DbCore<T>::Key k : keys_)
				std::cout << k << " ";
		}
		else
			std::cout << "\n   didn't find match for the pattern \"" << arg << "\" in the name";
		return *this;
	}


	//prints the keys that match with the regular expression pattern in the meta data's description
	template <typename T>
	Query<T>& Query<T>::selectDesc(const std::string& arg)
	{
		DbCore<T>::DbStore dbStore_ = db_.dbStore();
		DbCore<T>::Keys keys = keys_;
		keys_.clear();
		std::cout << "\n\n--------------------------------------------------------";
		std::cout << "\n  Searching  metadata description match for the pattern \"" << arg << "\"";
		std::regex regx(arg);
		for (DbCore<T>::Key k : keys)
		{
			DbElement<T> dbElem = dbStore_[k];
			std::string dbElemDesc = dbElem.descrip();
			if (std::regex_match(dbElemDesc, regx))
			{
				keys_.push_back(k);
			}
		}
		if (keys_.size() > 0)
		{
			std::cout << "\n found  metadata description match for the pattern \"" << arg << "\"\n The matched keys are:\n";
			for (DbCore<T>::Key k : keys_)
				std::cout << k << " ";
		}
		else
			std::cout << "\n   didn't find match for the pattern \"" << arg << "\" in the description";

		return *this;
	}


	//prints the keys that match with the regular expression pattern in the payload's filepath
	template <typename T>
	Query<T>& Query<T>::selectPayloadFilepath(const std::string& arg)
	{
		DbCore<T>::DbStore dbStore_ = db_.dbStore();
		DbCore<T>::Keys keys = keys_;
		keys_.clear();
		std::cout << "\n--------------------------------------------------------";
		std::cout << "\n  Searching payload filepath pattern match for the pattern \"" << arg << "\"";
		std::regex regx(arg);
		for (DbCore<T>::Key k : keys)
		{
			DbElement<T> dbElem = dbStore_[k];
			PayLoad dbElemPayload = dbElem.payLoad();
			std::string filepath = dbElemPayload.filePath();
			if (std::regex_match(filepath, regx))
			{
				keys_.push_back(k);
			}
		}

		if (keys_.size() > 0)
		{
			std::cout << "\n found payload filepath pattern match for the pattern \"" << arg << "\"\n The matched keys are:\n";
			for (DbCore<T>::Key k : keys_)
				std::cout << k << " ";
		}
		else
			std::cout << "\n   didn't find match for the pattern \"" << arg << "\" in the payload filepath";
		return *this;
	}


	//prints the keys that match with the regular expression pattern in the payload's categories
	template <typename T>
	Query<T>& Query<T>::selectPayloadCategory(const std::string& arg)
	{
		DbCore<T>::DbStore dbStore_ = db_.dbStore();
		DbCore<T>::Keys keys = keys_;
		keys_.clear();
		std::cout << "\n--------------------------------------------------------";
		std::cout << "\n  Searching payload categories that has the category \"" << arg << "\"";
		for (DbCore<T>::Key k : keys)
		{
			DbElement<T> dbElem = dbStore_[k];
			PayLoad dbElemPayload = dbElem.payLoad();
			PayLoad::Categories categories = dbElemPayload.categories();
			for (auto category : categories)
			{
				if (category==arg)
				{
					keys_.push_back(k);
				}
			}
		}

		if (keys_.size() > 0)
		{
			std::cout << "\n Found payload categories that has the category \"" << arg << "\"\n The keys of this category are:\n";
			for (DbCore<T>::Key k : keys_)
				std::cout << k << " ";
		}
		else
			std::cout << "\n   didn't find match for the category \"" << arg << "\" in the payload categories of entire database";
		return *this;
	}

	//prints the keys that exists in the time interval

	template <typename T>
	Query<T>& Query<T>::selectDateInterval(DateTime dt1, DateTime dt2)
	{
		DbCore<T>::Keys keys = keys_;
		keys_.clear();
		std::cout << "\n\n--------------------------------------------------------";
		std::cout << "\n When both ends of time interval is specified.";
		std::cout << "\n Searching date interval match Query for dates between:";
		std::cout << "\n    " << dt1.time() << " and " << dt2.time();

		for (DbCore<T>::Key k : keys)
		{
			DbElement<T> dbElem = dbStore_[k];
			DateTime dt = dbElem.dateTime();
			if (dt2 > dt && dt > dt1)
			{
				keys_.push_back(k);
			}
		}

		if (keys_.size() > 0)
		{
			std::cout << "\n    matches : ";
			for (DbCore<T>::Key k : keys_)
				std::cout << k << " ";
		}
		else
			std::cout << "\n    didn't find match keys between times \"" << dt1.time() << "\" and \"" << dt2.time() << "\"";
		return *this;
	}

	//prints the keys that are from the specified time to present time

	template <typename T>
	Query<T>& Query<T>::selectDate(DateTime dt1)
	{
		DbCore<T>::Keys keys = keys_;
		keys_.clear();
		DateTime dt2 = DateTime().now();
		DateTime::Duration dur = DateTime::makeDuration(0, 0, 1, 0);
		dt2 = dt2 + dur;
		std::cout << "\n\n--------------------------------------------------------";
		std::cout << "\n When only one end of interval is provided the other taken as present time.";
		std::cout << "\n Searching date interval match Query for dates between:";
		std::cout << "\n    " << dt1.time() << " and " << dt2.time();
		for (DbCore<T>::Key k : keys)
		{
			DbElement<T> dbElem = dbStore_[k];
			DateTime dt = dbElem.dateTime();
			if (dt2 > dt && dt > dt1)
			{
				keys_.push_back(k);
			}
		}

		if (keys_.size() > 0)
		{
			std::cout << "\n The keys that lies in the time interval are : ";
			for (DbCore<T>::Key k : keys_)
				std::cout << k << " ";
		}
		else
			std::cout << "\n    didn't find match keys between times \"" << dt1.time() << "\" and \"" << dt2.time() << "\"";
		return *this;
	}


	//copies keys of another query class to this class
	template<typename T>
	Query<T>& Query<T>::from(Keys& keys)
	{
		keys_.clear();
		keys_ = keys;
		return *this;
	}

	//----< returns current key set for db >-----------------------------

	template<typename T>
	typename Query<T>::Keys& Query<T>::keys()
	{
		return keys_;
	}
	template <typename T>
	void Query<T>::show()
	{
		DbCore<T>::Keys keys = keys_;
		if (keys.size() > 0)
			std::cout << "\n \n  The keys are";
		else
			std::cout << "\n\n There are no keys to show";
		for (DbCore<T>::Key k : keys)
		{
			std::cout << " \"" << k << "\"";
		}
		std::cout << "\n done";
	}

}
