////////////////////////////////////////////////////////////////////////////////////////
// Persist.h : store and retrieve NoSqlDb contents                                    //
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
* createXml() : creates an xml from the database
* saveToFile() : saves created xml in a file
* restore() : this is where restoration takes place
* restoreFromFile() : reads the xml file
*
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

#include "../Utilities/CodeUtilities/CodeUtilities.h"
#include "../Utilities/StringUtilities/StringUtilities.h"
#include "../Utilities/TestUtilities/TestUtilities.h"
#include "../DbCore/DbCore.h"
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <regex>
#include <fstream>
#include "../DateTime/DateTime.h"
#include "../XmlDocument/XmlDocument.h"
#include "../XmlDocument/XmlElement.h"

using namespace NoSqlDb;
using namespace XmlProcessing;
using sPtr = std::shared_ptr < AbstractXmlElement >;
namespace PersistDb
{
	/////////////////////////////////////////////////////////////////////
	// Persist class
	// -  provide persisting and retireving of db functionalities
	template<typename T>
	class Persist
	{
	public:
		using Xml = std::string;
		using SPtr = std::shared_ptr<AbstractXmlElement>;
		using SPtrs = std::vector<SPtr>;
		Persist(const DbCore<T>& dbCore) {db_ = dbCore;}
		Xml createXml();
		bool saveToFile(const std::string& fileSpec);
		void restore(const Xml& xml);
		bool restoreFromFile(const std::string& fileSpec);
	private:
		DbCore<T> db_;
	};

	/*
	* This is where the restoration takes place.
	*/
	template<typename T>
	void Persist<T>::restore(const Xml& xml)
	{
		Utilities::title("\n  Building Database from XmlDocument");
		using Sptr = std::shared_ptr<AbstractXmlElement>;
		using Key = std::string;
		DbCore<T> newDb;
		XmlDocument newXDoc(xml, XmlDocument::str);
		std::vector<Sptr> records = newXDoc.descendents("dbRecord").select();
		for (auto pRecord : records)
		{
			Key key;
			DbElement<T> elem;
			std::vector<Sptr> pChildren = pRecord->children();
			for (auto pChild : pChildren)
			{
				if (pChild->tag() == "key")
					key = pChild->children()[0]->value();
				else
				{
					std::vector<Sptr> pValueChildren = pChild->children();
					for (auto pValueChild : pValueChildren)
					{
						if (pValueChild->tag() == "name")
							elem.name(pValueChild->children()[0]->value());
						if (pValueChild->tag() == "description")
							elem.descrip(pValueChild->children()[0]->value());
						if (pValueChild->tag() == "date")
							elem.dateTime(pValueChild->children()[0]->value());
						if (pValueChild->tag() == "children")
						{
							std::vector<std::string> children;
							std::vector<Sptr> pChildren = pValueChild->children();
							for (auto pChild : pChildren)
							{
								children.push_back(pChild->children()[0]->value());
							}
							elem.children(children);
						}
						if (pValueChild->tag() == "payload")
						{
							PayLoad payload = restorepayload(pValueChild);
							elem.payLoad(payload);
						}
					}
				}
			}
			newDb[key] = elem;
		}
		std::cout << "\n \nThe restored Database from xml is:";
		showDb(newDb);
	}

	/*
	* This is where the restoration of payload takes place.
	*/
	PayLoad restorepayload(std::shared_ptr<AbstractXmlElement> pValueChild)
	{
		PayLoad payLoad;
		using Sptr = std::shared_ptr<AbstractXmlElement>;
		std::vector<Sptr> pPayLoadChildren = pValueChild->children();
		for (auto pPayLoadChild : pPayLoadChildren)
		{
			if (pPayLoadChild->tag() == "filepath")
				payLoad.filePath(pPayLoadChild->children()[0]->value());
			if (pPayLoadChild->tag() == "categories")
			{
				std::vector<std::string> categories;
				std::vector<Sptr> pCategories = pPayLoadChild->children();
				for (auto pCategory : pCategories)
					categories.push_back(pCategory->children()[0]->value());
				payLoad.categories(categories);
			}
		}
		return payLoad;
	}

	//----< restores db contents from file, using restore >------------

	template<typename T>
	bool Persist<T>::restoreFromFile(const std::string& fileSpec)
	{
		std::ifstream in(fileSpec);
		if (!in.good())
		{
			std::cout << "\n  failed to open file";
			return false;
		}
		Xml xml;

		// build XML string from file

		while (in.good())
		{
			char ch = in.get();

			/////////////////////////////////////////////////////
			// This code removes whitespace from XML contents
			// but is unnecessart and adds a small CPU load.
			// I left it here because I intent to use it in
			// another package, for display.
			/////////////////////////////////////////////////////
			// if (ch == '\n' || ch == '\t' || ch == ' ')
			//   continue;
			/////////////////////////////////////////////////////

			if (!in.good())
				break;
			xml += ch;
		}
		in.close();

		restore(xml);
		return true;
	}

	/*
	* This is where the persistance of db takes place.
	*/

	template <typename T>
	typename Persist<T>::Xml Persist<T>::createXml()
	{
		std::cout << "\n  Creating XML representation of Database using XmlDocument";
		std::cout << "\n ----------------------------------------------------------------";
		using Sptr = std::shared_ptr<AbstractXmlElement>;
		Sptr pDb = makeTaggedElement("db");
		Sptr pDocElem = makeDocElement(pDb);
		XmlDocument xDoc(pDocElem);

		for (auto item : db_)
		{
			Sptr pRecord = makeTaggedElement("dbRecord");
			pDb->addChild(pRecord);
			Sptr pKey = makeTaggedElement("key", item.first);
			pRecord->addChild(pKey);

			Sptr pValue = makeTaggedElement("value");
			pRecord->addChild(pValue);
			Sptr pName = makeTaggedElement("name", item.second.name());
			pValue->addChild(pName);
			Sptr pDescrip = makeTaggedElement("description", item.second.descrip());
			pValue->addChild(pDescrip);
			Sptr pDate = makeTaggedElement("date", item.second.dateTime());
			pValue->addChild(pDate);

			SPtr pChildren = XmlProcessing::makeTaggedElement("children");
			DbCore<T>::Keys childKeys = item.second.children();
			for (DbCore<T>::Key child : childKeys)
			{
				SPtr pChild = XmlProcessing::makeTaggedElement("childKey", static_cast<std::string>(child));
				pChildren->addChild(pChild);
			}
			pValue->addChild(pChildren);
			Sptr pPayLoad = makeTaggedElement("payload");
			pValue->addChild(pPayLoad);
			Sptr pPayLoadFilepath = makeTaggedElement("filepath",item.second.payLoad().filePath());
			pPayLoad->addChild(pPayLoadFilepath);
			SPtr pCategories = XmlProcessing::makeTaggedElement("categories");
			DbCore<T>::Keys categories = item.second.payLoad().categories();
			for (DbCore<T>::Key child : categories)
			{
				SPtr pChild1 = XmlProcessing::makeTaggedElement("category", static_cast<std::string>(child));
				pCategories->addChild(pChild1);
			}
			pPayLoad->addChild(pCategories);
		}
		std::string Xml = xDoc.toString();
		return Xml;
	}


	//----< saves db contents to file, using save >--------------------

	template<typename T>
	bool Persist<T>::saveToFile(const std::string& fileSpec)
	{
		std::ofstream out(fileSpec);
		if (!out.good())
			return false;
		Persist<T> persist(db_);
		Xml xml = persist.createXml();
		out << xml;
		std::cout << "\n" << xml;
		out.close();
		return true;
	}
}