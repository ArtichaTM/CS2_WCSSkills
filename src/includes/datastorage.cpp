#include <string>
#include <cassert>
#include <vector>
#include <iostream>
#include "datastorage.hpp"

using std::to_string;
using std::string;
using std::vector;
using nlohmann::json;
using nlohmann::detail::value_t;

namespace dataStorage {
	EData::~EData() {
		if (deleteOnFree && dtype != DataType::None) this->clearData();
	}

	void EData::clearData() {
		switch (dtype) {
			case None: {
				throw DeletingConstData();
				break;
			}
			case WCSP: {
				throw DeletingConstData();
				break;
			}
			case SEInfo: {
				throw DeletingConstData();
				break;
			}
			case SkillSE: {
				throw DeletingConstData();
				break;
			}
			case SkillInfo: {
				throw DeletingConstData();
				break;
			}
			case Bool: {
				delete (bool *) data;
				break;
			}
			case Short: {
				delete (short *) data;
				break;
			}
			case UnsignedShort: {
				delete (unsigned short *) data;
				break;
			}
			case Int: {
				delete (int *) data;
				break;
			}
			case UnsignedInt: {
				delete (unsigned int *) data;
				break;
			}
			case Long: {
				delete (long *) data;
				break;
			}
			case UnsignedLong: {
				delete (unsigned long *) data;
				break;
			}
			case LongLong: {
				delete (long long *) data;
				break;
			}
			case UnsignedLongLong: {
				delete (unsigned long long *) data;
				break;
			}
			case Float: {
				delete (float *) data;
				break;
			}
			case Double: {
				delete (double *) data;
				break;
			}
			case LongDouble: {
				delete (long double *) data;
				break;
			}
			case String: {
				delete (string *) data;
				break;
			}
			case DataS: {
				delete (dataStorage::DataStorage *) data;
				break;
			}
			case VectorS: {
				delete (dataStorage::VectorStorage *) data;
				break;
			}
		}
		dtype = dataStorage::None;
	}

	EData::EData(bool _deleteOnFree) : deleteOnFree(_deleteOnFree), dtype(dataStorage::DataType::None) {}

	void EData::setData(void* value, DataType targetType) {
		if (this->dtype == DataType::None) {
			this->dtype = targetType;
		} else if (this->dtype != targetType) {
			throw WrongType{(char *) "Awaited type " + to_string(targetType) +
							", EData stores " + to_string(dtype) + '.'};
		} else {
			this->clearData();
		}
		this->data = value;
	}

	template<typename T>
	void EData::setConstData(const T *) {
		throw WrongType("No specialization created for type " + to_string(*typeid(T).name()) + '.');
	}
	
	template<typename T>
	void EData::setData(T *) {
		throw WrongType("No specialization created for type " + to_string(*typeid(T).name()) + '.');
	}

	template<typename T>
	void EData::deleteData() {
		throw WrongType("No specialization created for type " + to_string(*typeid(T).name()) + '.');
	}

	template<>
	void *EData::getData<void>(DataType targetType) const {
		if (this->dtype == DataType::None) {
			return nullptr;
		} else if (this->dtype != targetType) {
			throw WrongType();
		}
		return this->data;
	}


	template<>
	DataStorage* make<DataStorage>(bool deleteOnFree, json &info) {
		assert(info.is_object());
		DataStorage* storage = new DataStorage(info.size());
		for (auto& [key, element]: info.items()) switch (element.type()) {
			case value_t::number_float: {
				storage->emplace(key, makeED<true>(deleteOnFree, element.template get<float>()));
				break;
			}
			case value_t::number_unsigned: {
				storage->emplace(key, makeED<true>(deleteOnFree, element.template get<unsigned int>()));
				break;
			}
			case value_t::number_integer: {
				storage->emplace(key, makeED<true>(deleteOnFree, element.template get<int>()));
				break;
			}
			case value_t::boolean: {
				storage->emplace(key, makeED<true>(deleteOnFree, element.template get<bool>()));
				break;
			}
			case value_t::string: {
				storage->emplace(key, makeED<true>(deleteOnFree, element.template get<string>()));
				break;
			}
			case value_t::array: {
				storage->emplace(key, makeED<true>(deleteOnFree,
					make<VectorStorage>(deleteOnFree, element)
				));
				break;
			}
			case value_t::object: {
				storage->emplace(key, makeED<true>(deleteOnFree,
					make<DataStorage>(deleteOnFree, element)
				));
				break;
			}
			default: {
				throw WrongType();
				break;
			}
		}
		return storage;
	}

	template<>
	VectorStorage* make<VectorStorage>(bool deleteOnFree, json &info) {
		assert(info.array());
		VectorStorage* storage = new VectorStorage(info.size());
		for (auto &element: info) switch (element.type()) {
			case value_t::number_float: {
				storage->push_back(makeED<true>(deleteOnFree, element.template get<float>()));
				break;
			}
			case value_t::number_unsigned: {
				storage->push_back(makeED<true>(deleteOnFree, element.template get<unsigned int>()));
				break;
			}
			case value_t::number_integer: {
				storage->push_back(makeED<true>(deleteOnFree, element.template get<int>()));
				break;
			}
			case value_t::boolean: {
				storage->push_back(makeED<true>(deleteOnFree, element.template get<bool>()));
				break;
			}
			case value_t::string: {
				storage->push_back(makeED<true>(deleteOnFree, element.template get<string>()));
				break;
			}
			case value_t::array: {
				storage->push_back(makeED<true>(deleteOnFree,
										 make<VectorStorage>(deleteOnFree, element)
				));
				break;
			}
			case value_t::object: {
				storage->push_back(makeED<true>(deleteOnFree,
										 make<DataStorage>(deleteOnFree, element)
				));
				break;
			}
			default: {
				throw WrongType();
				break;
			}
		}
		return storage;
	}
}

EDATA_SIMPLE_DEFINE(bool, Bool)
EDATA_SIMPLE_DEFINE(short, Short)
EDATA_SIMPLE_DEFINE(unsigned short, UnsignedShort)
EDATA_SIMPLE_DEFINE(int, Int)
EDATA_SIMPLE_DEFINE(unsigned int, UnsignedInt)
EDATA_SIMPLE_DEFINE(long, Long)
EDATA_SIMPLE_DEFINE(unsigned long, UnsignedLong)
EDATA_SIMPLE_DEFINE(long long, LongLong)
EDATA_SIMPLE_DEFINE(unsigned long long, UnsignedLongLong)
EDATA_SIMPLE_DEFINE(float, Float)
EDATA_SIMPLE_DEFINE(double, Double)
EDATA_SIMPLE_DEFINE(long double, LongDouble)
EDATA_SIMPLE_DEFINE(string, String)
EDATA_SIMPLE_DEFINE(dataStorage::DataStorage, DataS)
EDATA_SIMPLE_DEFINE(dataStorage::VectorStorage, DataS)
