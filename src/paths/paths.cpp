#include <fstream>
#include "../includes/exceptions.hpp"
#include "paths.hpp"
#ifndef CMAKE
#include "../metamod_main.hpp"
#endif

Paths* Paths::_instance = nullptr;

Paths::Paths(std::filesystem::path _main_dir) :
	main_dir(_main_dir),
	data_dir(main_dir / std::filesystem::path("data")),
	
	json_folder(data_dir / std::filesystem::path("json")),
	sql_folder(data_dir / std::filesystem::path("sql")),
	database(data_dir / std::filesystem::path("database.db")),
	
	skills(json_folder / std::filesystem::path("skills.json")),
	se(json_folder / std::filesystem::path("se.json")),
	traits(json_folder / std::filesystem::path("traits.json")),
	
	create_tables_sql(sql_folder / std::filesystem::path("create_tables.sql"))
{
	std::fstream f_skills(skills.c_str());
	std::fstream f_se(skills.c_str());
	std::fstream f_traits(skills.c_str());
	std::fstream f_create_tables_sql(create_tables_sql.c_str());

	bool good_skills = f_skills.good();
	bool good_se = f_se.good();
	bool good_traits = f_traits.good();
	bool good_create_tables_sql = f_create_tables_sql.good();

	f_skills.close();
	f_se.close();
	f_traits.close();

	if (!good_skills) {
		throw WrongDataLocation("Can't open json/skills.json");
	}
	else if (!good_se) {
		throw WrongDataLocation("Can't open json/se.json");
	}
	else if (!good_traits) {
		throw WrongDataLocation("Can't open json/traits.json");
	}
	else if (!good_create_tables_sql) {
		throw WrongDataLocation("Can't open sql/create_tables.sql");
	}
}

void Paths::init() {
	if (_instance) {
		throw PathsRecreating();
	}
#ifdef CMAKE
	_instance = new Paths(std::filesystem::path(CMAKE_DATA_DIR));
#else
	_instance = new Paths(
		std::filesystem::path(g_WCSSkills.getISmmAPI()->GetBaseDir()) / 
		std::filesystem::path("addons") / 
		std::filesystem::path("WCSSkills")
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

Paths::~Paths() {
}
