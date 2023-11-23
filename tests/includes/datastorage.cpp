/*
Tests valid only with DEBUG defined!
*/

#include <string>

#include <gtest/gtest.h>
#include "../../src/includes/datastorage.hpp"
#include "../../src/includes/doublelinkedlist.hpp"
#include "../leaks.hpp"

using namespace dataStorage;

namespace dataStorage {
	void testEData2(EData& ed) {
		int* integer = ed.getData<int>();
		*integer = (*integer) + 1;
	}

	TEST(EData, general) {
		MemoryLeakDetector leakDetector;
		auto ed = EData(true);
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
		auto ed = EData(true);
		float* value = new float(50.0);
		ASSERT_FLOAT_EQ(*value, 50.0);
		ed.setData<float>(value);
		ed.deleteData<float>();
		ASSERT_THROW(ed.deleteData<float>(), WrongType);
	}

	TEST(EData, change_data_lambda) {
		MemoryLeakDetector leakDetector;
		auto ed = EData(true);
		float* value = new float(50.0);
		ed.setData(value);
		ASSERT_FLOAT_EQ(*ed.getData<float>(), 50.);
		std::function<void(float&)> func = [](float& value) mutable -> void { value *= 2.; };
		ed.changeData(func);
		ASSERT_FLOAT_EQ(*ed.getData<float>(), 100.);
	}

	template <typename T>
	void dolili_test() {
		MemoryLeakDetector leakDetector;
		auto ed = EData(false);
		auto* dolili1 = new DoubleLinkedList<T>();
		ed.setData(dolili1);
		auto* dolili2 = ed.getData<DoubleLinkedList<T>>();
		ASSERT_EQ(dolili1, dolili2);
		delete dolili1;
	}

	TEST(EData, dolili_bool_short				) { dolili_test<bool				>(); }
	TEST(EData, dolili_short_short				) { dolili_test<short				>(); }
	TEST(EData, dolili_unsigned_short_short		) { dolili_test<unsigned short		>(); }
	TEST(EData, dolili_int_short				) { dolili_test<int					>(); }
	TEST(EData, dolili_unsigned_int_short		) { dolili_test<unsigned int		>(); }
	TEST(EData, dolili_long_short				) { dolili_test<long				>(); }
	TEST(EData, dolili_unsigned_long_short		) { dolili_test<unsigned long		>(); }
	TEST(EData, dolili_long_long_short			) { dolili_test<long long			>(); }
	TEST(EData, dolili_unsigned_long_longshort	) { dolili_test<unsigned long long	>(); }
	TEST(EData, dolili_float_short				) { dolili_test<float				>(); }
	TEST(EData, dolili_double_short				) { dolili_test<double				>(); }
	TEST(EData, dolili_long_double_short		) { dolili_test<long double			>(); }
	TEST(EData, dolili_string_short_			) { dolili_test<std::string			>(); }
}
