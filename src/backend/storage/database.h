/*-------------------------------------------------------------------------
 *
l* database.h
 * file description
 *
 * Copyright(c) 2015, CMU
 *
 *-------------------------------------------------------------------------
 */

#pragma once

#include "tbb/concurrent_unordered_map.h"
#include "backend/common/types.h"
#include "backend/storage/data_table.h"

#include <iostream>

namespace peloton {
namespace storage {

class Database;
// database oid -> database address
typedef tbb::concurrent_unordered_map<oid_t, Database*> database_oid_to_address;
// table name -> table oid
typedef tbb::concurrent_unordered_map<std::string, oid_t> table_name_to_oid;
// table oid -> table address
typedef tbb::concurrent_unordered_map<oid_t, storage::DataTable*> table_oid_to_address;

static database_oid_to_address database_address_locator;

//===--------------------------------------------------------------------===//
// DataBase
//===--------------------------------------------------------------------===//

class Database{

public:
    Database(Database const&) = delete;  

    // Constructor
    Database( oid_t database_oid ) : database_oid(database_oid){}

    ~Database()
    {
      //drop all tables in current db
    };

    //===--------------------------------------------------------------------===//
    // OPERATIONS
    //===--------------------------------------------------------------------===//
    
    static Database* GetDatabaseById(oid_t database_oid){
      Database* db_address;

      try {
        db_address = database_address_locator.at( database_oid );
      }catch (const std::out_of_range& oor) {
        Database* db = new Database( database_oid ); 
        db_address = db;
        database_address_locator.insert( std::pair<oid_t,Database*>( database_oid,
                                                                     db_address ));
      }
      return db_address;
    }

    // TODO :: relation id will be removed when we can get oid from catalog stably
    //         or store relation id in the table
    bool AddTable(storage::DataTable* table){

      std::string table_name = table->GetName();
      oid_t table_oid = table->GetId();

      try {
        table_oid = table_oid_locator.at( table_name );
        return false; // table already exists
      }catch (const std::out_of_range& oor)  {
        table_oid_locator.insert( std::pair<std::string,oid_t>
                                           ( table_name, table_oid ));
      table_address_locator.insert( std::pair<oid_t, storage::DataTable*>
                                             ( table_oid, table ));
      }

      return true;
    }

    inline storage::DataTable* GetTableByName(const std::string table_name) const{
      storage::DataTable* table = nullptr;
      oid_t table_oid;
      try {
        table_oid = table_oid_locator.at( table_name );
      } catch  (const std::out_of_range& oor) {
    	  return table; // return nullptr
      }
      table = table_address_locator.at( table_oid );

      return table;
    }

    inline storage::DataTable* GetTableById(const oid_t table_oid) const{
      storage::DataTable* table = nullptr;
      try {
        table = table_address_locator.at( table_oid );
      } catch  (const std::out_of_range& oor) {
        table = nullptr;
      }
      return table;
    }

    /*
    GetTableCount()
    GetAllTableList()
    RemoveTableByName()
    RemoveTableById()
    DropAllDataTable()
    */

    //===--------------------------------------------------------------------===//
    // UTILITIES
    //===--------------------------------------------------------------------===//

    // Get a string representation of this table
    friend std::ostream& operator<<(std::ostream& os, const Database& database);

protected:

    //===--------------------------------------------------------------------===//
    // MEMBERS
    //===--------------------------------------------------------------------===//

    // database oid
    oid_t database_oid;

    // map from table name to table oid
    table_name_to_oid table_oid_locator;
    // map from table oid to table address
    table_oid_to_address table_address_locator;
};


} // End storage namespace
} // End peloton namespace

