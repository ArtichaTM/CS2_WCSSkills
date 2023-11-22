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
	skills(data_dir / std::filesystem::path("skills.json")),
	se(data_dir / std::filesystem::path("se.json")),
	traits(data_dir / std::filesystem::path("traits.json")),
	database(data_dir / std::filesystem::path("database.db")),
	sql_folder(data_dir / std::filesystem::path("sql")),
	create_tables_sql(sql_folder / std::filesystem::path("create_tables.sql"))
{
	std::fstream f_skills(skills.c_str());
	std::fstream f_se(skills.c_str());
	std::fstream f_traits(skills.c_str());

	bool good_skills = f_skills.good();
	bool good_se = f_se.good();
	bool good_traits = f_traits.good();

	f_skills.close();
	f_se.close();
	f_traits.close();

	if (!good_skills) {
		throw WrongDataLocation("Can't open skills.json");
	}
	else if (!good_se) {
		throw WrongDataLocation("Can't open se.json");
	}
	else if (!good_traits) {
		throw WrongDataLocation("Can't open traits.json");
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
