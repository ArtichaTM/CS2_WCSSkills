#include <memory>
#include <fstream>

#include "db.hpp"
#include "../paths/paths.hpp"

#ifndef META_CONPRINTF
#define META_CONPRINTF
#endif

using std::filesystem::path;
using std::make_unique;
using std::unique_ptr;
using std::ifstream;


namespace utilities {
	DB* DB::_instance = nullptr;

	DB::DB() : db(nullptr) {}

	DB::~DB()
	{
		if (NULL != db) sqlite3_close(db);
		sqlite3_shutdown();
	}

	sqlite3_stmt* DB::query(const char* query)
	{
		sqlite3_stmt* statement = nullptr;
		int ret;
		if (SQLITE_OK != (ret = sqlite3_prepare_v2(db, query, -1, &statement, NULL)))
		{
			META_CONPRINTF("DB08");
			return nullptr;
		}

		if (SQLITE_ROW != (ret = sqlite3_step(statement)))
		{
			META_CONPRINTF("DB08");
			return nullptr;
		}
		return statement;
	}
	
	template<>
	int DB::query<false>(char* query, sqlite3_stmt*& statement)
	{
		int ret;
		if (SQLITE_OK != (ret = sqlite3_prepare_v2(db, query, -1, &statement, NULL)))
		{
			META_CONPRINTF("DB00");
			return ret;
		}
		ret = sqlite3_step(statement);
		switch (ret) {
		case SQLITE_ROW:
			break;
		case SQLITE_DONE:
			sqlite3_finalize(statement);
			statement = nullptr;
			break;
		default:
			break;
		}
		return ret;
	}

	template<>
	int DB::query<true>(char* query, sqlite3_stmt*& statement)
	{
		int ret = 0;
		char* new_ptr = nullptr;
		int previous_index = 0;
		int index = 0;
		while (query[index] != '\0') {
			if (query[index] == ';') {
				query[index + 1] = '\0';
				new_ptr = query + previous_index;
				ret = this->query<false>(new_ptr, statement);
				previous_index = index + 2;
				switch (ret) {
				case SQLITE_ROW:
					break;
				case SQLITE_DONE:
					break;
				default:
					return ret;
				}
				index++;
			}
			index++;
		}
		return ret;
	}
	 
	int DB::create_tables()
	{
		unique_ptr buffer = make_unique<char*>(new char[4096]);
		sqlite3_stmt* statement;
		int ret;
		
		do {
			sprintf(*buffer, "SELECT name FROM sqlite_master WHERE type='table';");
			if (SQLITE_OK != (ret = sqlite3_prepare_v2(db, *buffer, -1, &statement, NULL)))
			{
				META_CONPRINTF("DB03");
				break;
			}

			ret = sqlite3_step(statement);
			if (ret == SQLITE_ROW) {
				break;
			}
			else if (ret != SQLITE_DONE) {
				META_CONPRINTF("DB04");
				break;
			}

			// No tables found

			sqlite3_finalize(statement);
			statement = nullptr;

			ifstream infile;
			infile.open(Paths::getInstance()->create_tables_sql.string().c_str());
			infile.read(*buffer, 4096);
			if (infile.eof()) {
				// Got the whole file
			}
			else if (infile.fail()) {
				// Error
				infile.close();
				sprintf(*buffer, "DB05");
				META_CONPRINTF(*buffer);
				throw DBSQLFileError(*buffer);
			}
			else {
				// FIle is larger
				infile.close();
				sprintf(*buffer, "DB06");
				META_CONPRINTF(*buffer);
				throw DBSQLFileError(*buffer);
			}
			infile.close();
			this->query<true>(*buffer, statement);


		} while (0);

		sqlite3_finalize(statement);
		return ret;
	}

	void DB::init()
	{
		if (_instance) {
			throw DBRecreating();
		}

		sqlite3* current_db = nullptr;
		sqlite3_stmt* query = nullptr;
		int ret = 0;
		do // avoid nested if's
		{

			// initialize engine
			if (SQLITE_OK != (ret = sqlite3_initialize()))
			{
				META_CONPRINTF("DB01");
				break;
			}

			// open connection to a DB
			if (SQLITE_OK != (ret = sqlite3_open_v2(Paths::getInstance()->database.string().c_str(),
				&current_db,
				SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
				NULL
			))) {
				META_CONPRINTF("DB02");
				break;
			}

		} while (false);
		if (ret == 0) {
			_instance = new DB();
			_instance->db = current_db;
		}

		// cleanup
		if (NULL != query) sqlite3_finalize(query);
	}

	void DB::close()
	{
		if (!_instance) {
			throw DBReclosing();
		}
		delete _instance;
		_instance = nullptr;
	}

	DB* DB::getInstance()
	{
		if (!_instance) {
			throw DBIsNotInitialized();
		}
		return _instance;
	}
}