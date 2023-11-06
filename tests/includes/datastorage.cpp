/*
Tests valid only with DEBUG defined!
*/

#include <gtest/gtest.h>
#include "../../src/includes/datastorage.hpp"
#include "../leaks.hpp"

namespace dataStorage {
	void testEData2(dataStorage::EData& ed) {
		int* integer = ed.getData<int>();
		*integer = (*integer) + 1;
	}

	TEST(EData, general) {
		MemoryLeakDetector leakDetector;
		auto ed = dataStorage::EData(true);
		int* value = new int(50);
		ASSERT_EQ(*value, 50);
		ed.setData(value);
		int* got_value = ed.getData<int>();
		ASSERT_EQ(*got_value, 50);
		testEData2(ed);
		ASSERT_EQ(*got_value, 51);

	}

	TEST(EData, repeat_delete) {
		MemoryLeakDetector leakDetector;
		auto ed = dataStorage::EData(true);
		float* value = new float(50.0);
		ASSERT_FLOAT_EQ(*value, 50.0);
		std::cout << "\nSeting data";
		ed.setData<float>(value);
		std::cout << "\nDeleting first time";
		ed.deleteData<float>();
		std::cout << "\nDeleting second time";
		ASSERT_THROW(ed.deleteData<float>(), dataStorage::WrongType);
	}

	TEST(EData, change_data_lambda) {
		MemoryLeakDetector leakDetector;
		auto ed = dataStorage::EData(true);
		float* value = new float(50.0);
		ed.setData(value);
		ASSERT_FLOAT_EQ(*ed.getData<float>(), 50.);
		std::function<void(float&)> func = [](float& value) mutable -> void { value *= 2.; };
		ed.changeData(func);
		ASSERT_FLOAT_EQ(*ed.getData<float>(), 100.);
	}
}
