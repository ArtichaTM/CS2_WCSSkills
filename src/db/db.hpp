#include <filesystem>

#include "sqlite3/sqlite3.h"
#include "../includes/exceptions.hpp"

namespace utilities {
	class DB {
	protected:
		static DB* _instance;
		sqlite3* db;

		explicit DB();
		~DB();
		DB(DB const&) = delete;

		/*
		* Completes one query and returns it's statement
		* This function primarily used for SELECT statements as they return rows
		*/
		sqlite3_stmt* query(const char*);

		/*
		* Completes one or more querys.
		* transation = true : pass char[] of query with endings on ; (can be 1 or more)
		* transation = false: pass ONLY one query that ends with ;
		* 
		* Passed statement should be nullptr
		*/
		template<bool transaction>
		int query(char*, sqlite3_stmt*&);
	public:
		class DBSQLFileError : public CustomException { using CustomException::CustomException; };
		class DBDefaultException : public CustomException { using CustomException::CustomException; };
		class DBIsNotInitialized : public CustomException { using CustomException::CustomException; };
		class DBRecreating : public CustomException { using CustomException::CustomException; };
		class DBReclosing : public CustomException { using CustomException::CustomException; };

		int create_tables();

		static void init();
		static void close();
		static DB* getInstance();
	};
}