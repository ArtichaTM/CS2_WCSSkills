#include <fstream>
#include "../includes/exceptions.hpp"
#include "paths.hpp"
#ifndef CMAKE
#include "../metamod_main.hpp"
#endif

using std::fstream;
using std::filesystem::path;


Paths* Paths::_instance = nullptr;

Paths::Paths(path _main_dir) :
	main_dir(_main_dir),
	data_dir(main_dir / path("data")),
	
	json_folder(data_dir / path("json")),
	sql_folder(data_dir / path("sql")),
	database(data_dir / path("database.db")),
	
	skills(json_folder / path("skills.json")),
	se(json_folder / path("se.json")),
	traits(json_folder / path("traits.json")),
	
	create_tables_sql(sql_folder / path("create_tables.sql"))
{
	fstream f_skills(skills.c_str());
	fstream f_se(se.c_str());
	fstream f_traits(traits.c_str());
	fstream f_create_tables_sql(create_tables_sql.c_str());

	bool good_skills = f_skills.good();
	bool good_se = f_se.good();
	bool good_traits = f_traits.good();
	bool good_create_tables_sql = f_create_tables_sql.good();

	f_skills.close();
	f_se.close();
	f_traits.close();
	f_create_tables_sql.close();

	if (!good_skills) {
		throw WrongDataLocation("WDL1");
	}
	else if (!good_se) {
		throw WrongDataLocation("WDL2");
	}
	else if (!good_traits) {
		throw WrongDataLocation("WDL3");
	}
	else if (!good_create_tables_sql) {
		throw WrongDataLocation("WDL4");
	}
}

void Paths::init() {
	if (_instance) {
		throw PathsRecreating();
	}
#ifdef CMAKE
	_instance = new Paths(path(CMAKE_DATA_DIR));
#else
	_instance = new Paths(
		path(g_WCSSkills.getISmmAPI()->GetBaseDir()) / 
		path("addons") / 
		path("WCSSkills")
	);
#endif
}

void Paths::close() {
	if (!_instance) {
		throw PathsReclosing();
	}
	delete _instance;
}

Paths* Paths::getInstance() {
	if (!_instance) {
		throw PathsIsNotInitialized();
	}
	return _instance;
}

Paths::~Paths() {}
