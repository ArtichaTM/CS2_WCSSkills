#include <string>
#include <cassert>
#include <vector>
#include <format>
#include "datastorage.hpp"

using std::to_string;
using std::string;
using std::vector;
using nlohmann::json;
using nlohmann::detail::value_t;

namespace dataStorage {
    EData::~EData() {
        if (deleteOnFree) this->clearData();
    }

    void EData::clearData() {
        switch (dtype) {
            case None:
            case WCSP:
            case SEInfo:
            case SkillSE:
            case SkillInfo: {
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
    }

    EData::EData(bool _deleteOnFree) : deleteOnFree(_deleteOnFree) {}

    void EData::setData(void *value, DataType targetType) {
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
    void EData::setData(const T *) {
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

    template<> DataStorage  * make<DataStorage  >(bool deleteOnFree, json &info);
    template<> VectorStorage* make<VectorStorage>(bool deleteOnFree, json &info);

    template<>
    DataStorage* make<DataStorage>(bool deleteOnFree, json &info) {
        assert(info.is_object());
        auto *storage = new DataStorage(info.size());
        for (auto &[key, element]: info.items()) switch (element.type()) {
            case value_t::number_float: {
                storage->emplace(key, (*make(deleteOnFree, new float(element))));
                break;
            }
            case value_t::number_unsigned: {
                storage->emplace(key, (*make(deleteOnFree, new unsigned int(element))));
                break;
            }
            case value_t::number_integer: {
                storage->emplace(key, (*make(deleteOnFree, new int(element))));
                break;
            }
            case value_t::boolean: {
                storage->emplace(key, (*make(deleteOnFree, new bool(element))));
            }
            case value_t::string: {
                storage->emplace(key, (*make(deleteOnFree, new string(element))));
            }
            case value_t::array: {
                storage->emplace(key, (*make(deleteOnFree,
                                             make<VectorStorage>(deleteOnFree, element)
                )));
            }
            case value_t::object: {
                storage->emplace(key, (*make(deleteOnFree,
                                             make<DataStorage>(deleteOnFree, element)
                )));
            }
            default: {
                throw WrongType();
            }
        }
        return storage;
    }

    template<>
    VectorStorage *make<VectorStorage>(bool deleteOnFree, json &info) {
        assert(info.array());
        auto *storage = new VectorStorage(info.size());
        for (auto &element: info) switch (element.type()) {
            case value_t::number_float: {
                storage->push_back(*make(deleteOnFree, new float(element)));
                break;
            }
            case value_t::number_unsigned: {
                storage->push_back(*make(deleteOnFree, new unsigned int(element)));
                break;
            }
            case value_t::number_integer: {
                storage->push_back(*make(deleteOnFree, new int(element)));
                break;
            }
            case value_t::boolean: {
                storage->push_back(*make(deleteOnFree, new bool(element)));
            }
            case value_t::string: {
                storage->push_back(*make(deleteOnFree, new string(element)));
            }
            case value_t::array: {
                storage->push_back(*make(deleteOnFree,
                                         make<VectorStorage>(deleteOnFree, element)
                ));
            }
            case value_t::object: {
                storage->push_back(*make(deleteOnFree,
                                         make<DataStorage>(deleteOnFree, element)
                ));
            }
            default: {
                throw WrongType();
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
EDATA_SIMPLE_DEFINE_CONST(dataStorage::DataStorage, DataS)
EDATA_SIMPLE_DEFINE(dataStorage::VectorStorage, DataS)
EDATA_SIMPLE_DEFINE_CONST(dataStorage::VectorStorage, DataS)
