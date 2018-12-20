#pragma once
////////////////////////////////////////////////////////////////////////////////////////
// DbCore.h : Implements NoSql Database.                                              //
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
* This package provides three classes:
* - DbCore implements core NoSql database operations, but does not
*   provide editing, querying, or persisting.  Those are left as
*   exercises for students.
* - DbElement provides the value part of our key-value database.
*   It contains fields for name, description, date, child collection
*   and a payload field of the template type. 
*   PayLoad class provides the typename to the payload.
*   It contains fields for filepath and categories.
* The package also provides functions for displaying:
* - set of all database keys
* - database elements
* - all records in the database
*
* public Interfaces:
* ------------------
* keys() : returns current key set for db
* contains() : Checks whther the db contains the key
* removeRecord() : removes a key/value pair from db
* removeChild() : removes specified child of specified key
* saveValue() : saves the dbelement to a key
* addChild() : adds a child to specified key
* size() : returns size of the db
* operator[]() : extracts value from db with key
* showKeys() : displays keys of database
* showHeader() : displaya header
* showElem() : displays the value part
* showDb() : displays db
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

#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include "../DateTime/DateTime.h"

namespace NoSqlDb
{
	/////////////////////////////////////////////////////////////////////
	// PayLoad class
	// -  provides the typename for the payload
	class PayLoad
	{
	public:
		using FilePath = std::string;
		using Categories = std::vector<std::string>;

		std::string& filePath() { return filePath_; }
		std::string filePath() const { return filePath_; }
		void filePath(const std::string& filePath) { filePath_ = filePath; }

		Categories& categories() { return categories_; }
		Categories categories() const { return categories_; }
		void categories(const Categories& categories) { categories_ = categories; }

	private:
		FilePath filePath_;
		Categories categories_;
	};

	/////////////////////////////////////////////////////////////////////
	// DbElement class
	// - provides the value part of a NoSql key-value database

  template<typename T>
  class DbElement
  {
  public:
    using Key = std::string;
    using Children = std::vector<Key>;

    // methods to get and set DbElement fields

    std::string& name() { return name_; }
    std::string name() const { return name_; }
    void name(const std::string& name) { name_ = name; }

    std::string& descrip() { return descrip_; }
    std::string descrip() const { return descrip_; }
    void descrip(const std::string& name) { descrip_ = name; }
    
    DateTime& dateTime() { return dateTime_; }
    DateTime dateTime() const { return dateTime_; }
    void dateTime(const DateTime& dateTime) { dateTime_ = dateTime; }

    Children& children() { return children_; }
    Children children() const { return children_; }
    void children(const Children& children) { children_ = children; }

	T& payLoad() { return payLoad_; }
	T payLoad() const { return payLoad_; }
    void payLoad(const T& payLoad) { payLoad_ = payLoad; }

  private:
    std::string name_;
    std::string descrip_;
    DateTime dateTime_;
    Children children_;
	T payLoad_;
  };

  
  /////////////////////////////////////////////////////////////////////
  // DbCore class
  // - provides core NoSql db operations
  // - does not provide editing, querying, or persistance operations

  template <typename T>
  class DbCore
  {
  public:
	using Key = std::string;
    using Keys = std::vector<Key>;
    using Children = Keys;
    using DbStore = std::unordered_map<Key,DbElement<T>>;
    using iterator = typename DbStore::iterator;

    // methods to access database elements

    Keys keys();
    bool contains(const Key& key);
	bool removeRecord(const Key& key);
	bool removeChild(const Key& key, const Key& child);
	bool saveValue(const Key& key, const DbElement<T>& dbElem);
	bool addChild(const Key& key, const Key& child);
    size_t size();
    DbElement<T>& operator[](const Key& key);
    DbElement<T> operator[](const Key& key) const;
    typename iterator begin() { return dbStore_.begin(); }
    typename iterator end() { return dbStore_.end(); }

    // methods to get and set the private database hash-map storage

    DbStore& dbStore() { return dbStore_; }
    DbStore dbStore() const { return dbStore_; }
    void dbStore(const DbStore& dbStore) { dbStore_ = dbStore; }
  
  private:
    DbStore dbStore_;
  };
  /////////////////////////////////////////////////////////////////////
  // DbCore<T> methods

  //delete a key from the database and iterate through all keys if this key is in any key's children then delete that children

  template<typename T>
  bool DbCore<T>::removeRecord(const Key& key)
  {
	  if (dbStore_.find(key) == dbStore_.end())
		  return false;
	  size_t numErased = dbStore_.erase(key);
	  if (numErased == 1)
	  {
		  // remove key from all other element's children collections
		  for (Key k : keys())
		  {
			  Keys& children = dbStore_[k].children();  // note Keys& - we don't want copy of children
			  Keys::iterator iter;
			  for (iter = children.begin(); iter != children.end(); ++iter)
			  {
				  if ((*iter) == key)
				  {
					  children.erase(iter);
					  break;
				  }
			  }
		  }
		  return true;
	  }
	  return false;
  }

  //deletes child from a key specified
  template<typename T>
  bool DbCore<T>::removeChild(const Key& key,const Key& child)
  {
	  if (dbStore_.find(key) == dbStore_.end())
		  return false;
	  for (Key k : keys())
	  {
		  if(k==key){
			  Keys& children = dbStore_[k].children();  // note Keys& - we don't want copy of children
			  Keys::iterator iter;
			  for (iter = children.begin(); iter != children.end(); ++iter)
			  {
				  if ((*iter) == child)
				  {
					  children.erase(iter);
					  break;
				  }
			  }
		  }
	  }

	  return true;
  }

  //saves dbelement to a key in database 
  template<typename T>
  bool DbCore<T>::saveValue(const Key& key, const DbElement<T>& dbElem)
  {
	  if (dbStore_.find(key) == dbStore_.end())
		  return false;
	  dbStore_[key] = dbElem;
	  return true;


  }


//adds child to a key specified
  template<typename T>
  bool DbCore<T>::addChild(const Key& key, const Key& child)
  {
	  if (dbStore_.find(key) == dbStore_.end())
		  return false;
	  if (dbStore_.find(child) == dbStore_.end())
		  return false;
	  DbElement<T>& demoElem = dbStore_[key];
	  demoElem.children().push_back(child);
	  return true;
	  
	 
  }

  //----< does db contain this key? >----------------------------------

  template<typename T>
  bool DbCore<T>::contains(const Key& key)
  {
    iterator iter = dbStore_.find(key);
    return iter != dbStore_.end();
  }
  //----< returns current key set for db >-----------------------------

  template<typename T>
  typename DbCore<T>::Keys DbCore<T>::keys()
  {
    DbCore<T>::Keys dbKeys;
    DbStore& dbs = dbStore();
    size_t size = dbs.size();
    dbKeys.reserve(size);
    for (auto item : dbs)
    {
      dbKeys.push_back(item.first);
    }
    return dbKeys;
  }
  //----< return number of db elements >-------------------------------

  template<typename T>
  size_t DbCore<T>::size()
  {
    return dbStore_.size();
  }
  //----< extracts value from db with key >----------------------------
  /*
  *  - indexes non-const db objects
  */
  template<typename T>
  DbElement<T>& DbCore<T>::operator[](const Key& key)
  {
    if (!contains(key))
    {
      dbStore_[key] = DbElement<T>();
    }
    return dbStore_[key];
  }
  //----< extracts value from db with key >----------------------------
  /*
  *  - indexes const db objects
  */
  template<typename T>
  DbElement<T> DbCore<T>::operator[](const Key& key) const
  {
    if (!contains(key))
    {
      dbStore_[key] = DbElement<T>();
    }
    return dbStore_[key];
  }
  
  /////////////////////////////////////////////////////////////////////
  // display functions

  //----< display database key set >-----------------------------------

  template<typename T>
  void showKeys(DbCore<T>& db, std::ostream& out = std::cout)
  {
    out << "\n  ";
    for (auto key : db.keys())
    {
      out << key << " ";
    }
  }
  //----< show record header items >-----------------------------------

  inline void showHeader(std::ostream& out = std::cout)
  {
    out << "\n  ";
    out << std::setw(26) << std::left << "DateTime";
    out << std::setw(10) << std::left << "Name";
    out << std::setw(25) << std::left << "Description";
    out << std::setw(25) << std::left << "Payload FilePath";
	out << std::setw(25) << std::left << "Payload categories";
    out << "\n  ";
    out << std::setw(26) << std::left << "------------------------";
    out << std::setw(10) << std::left << "--------";
    out << std::setw(25) << std::left << "-----------------------";
    out << std::setw(25) << std::left << "-----------------------";
	out << std::setw(25) << std::left << "-----------------------";
  }
  //----< display DbElements >-----------------------------------------

  template<typename T>
  void showElem(const DbElement<T>& el, std::ostream& out = std::cout)
  {
    out << "\n  ";
    out << std::setw(26) << std::left << std::string(el.dateTime());
    out << std::setw(10) << std::left << el.name();
    out << std::setw(25) << std::left << el.descrip();
	PayLoad payload = el.payLoad();
	out << std::setw(25) << std::left << payload.filePath();
	
	PayLoad::Categories categories = payload.categories();
	if (categories.size() > 0)
	{
		for (auto category : categories)
		{
			out << " " << category;
		}
	}
    typename DbElement<T>::Children children = el.children();
    if (children.size() > 0)
    {
      out << "\n    child keys: ";
      for (auto key : children)
      {
        out << " " << key;
      }
    }
  }

 
  //----< display all records in database >----------------------------

  template<typename T>
  void showDb(const DbCore<T>& db, std::ostream& out = std::cout)
  {
    showHeader(out);
    typename DbCore<T>::DbStore dbs = db.dbStore();
    for (auto item : dbs)
    {
      showElem(item.second, out);
    }
  }
}
