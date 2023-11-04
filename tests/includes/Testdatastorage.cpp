#include <gtest/gtest.h>
#include "../../src/includes/datastorage.hpp"


void testEData2(dataStorage::EData& ed) {
	int* integer = ed.getData<int>();
	*integer = (*integer) + 1;
}

TEST(EData, general) {
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
	auto ed = dataStorage::EData(true);
	float* value = new float(50.0);
	ed.setData(value);
	//ed.changeData([&](float& value) {});
}

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}