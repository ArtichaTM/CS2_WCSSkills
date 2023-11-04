#ifndef WCSSKILLS_DATASTORAGE_HPP
#define WCSSKILLS_DATASTORAGE_HPP

#include <functional>
#include <vector>
#include "exceptions.hpp"
#include "json.hpp"

#ifdef DEBUG
#define EDATA_SIMPLE_SETDATA(_type, _datatype) \
template<> void dataStorage::EData::setData<_type>(_type* v) \
{ \
	this->setData((void*)v, dataStorage::DataType::_datatype); \
}
#else
#define EDATA_SIMPLE_SETDATA(_type, _datatype) \
template<> void dataStorage::EData::setData<_type>(_type* v) \
{ \
	this->setData((void*)v, dataStorage::DataType::_datatype); \
}
#endif

#define EDATA_SIMPLE_GETDATA(_type, _datatype) \
template<> _type* dataStorage::EData::getData<_type>() const \
{   \
	return (_type*)this->getData<void>(dataStorage::DataType::_datatype); \
}

#ifdef DEBUG
	#define EDATA_SIMPLE_DELETEDATA(_type, _datatype) \
	template<> void dataStorage::EData::deleteData<_type>() \
	{ \
		if (dtype != _datatype) { \
			throw WrongType("Trying to delete data from EData passing wrong type");\
		} \
		if (dtype == dataStorage::DataType::None) { \
			throw DeletingEmptyEData("Trying to delete empty EData. May be you repeating .deleteData() method?"); \
		} \
		delete (_type*) data; \
		dtype = dataStorage::DataType::None; \
	}
#else
	#define EDATA_SIMPLE_DELETEDATA(_type, _datatype) \
	template<> void dataStorage::EData::deleteData<_type>() \
	{ \
		delete (_type*) data; \
		dtype = dataStorage::DataType::None; \
	}
#endif

#define EDATA_SIMPLE_DEFINE(_type, _datatype)\
EDATA_SIMPLE_SETDATA(_type, _datatype)\
EDATA_SIMPLE_GETDATA(_type, _datatype)\
EDATA_SIMPLE_DELETEDATA(_type, _datatype)

#define EDATA_SIMPLE_SETDATA_CONST(_type, _datatype) \
template<> void dataStorage::EData::setData<_type>(_type const* v) \
{ this->setData((void*)v, dataStorage::DataType::_datatype); }

#define EDATA_SIMPLE_DEFINE_CONST(_type, _datatype)\
EDATA_SIMPLE_SETDATA_CONST(_type, _datatype)

namespace dataStorage {
	class WrongType : public CustomException { using CustomException::CustomException; };
	class DeletingEmptyEData : public CustomException { using CustomException::CustomException; };
	enum DataType {
		None, Bool,
		Short, UnsignedShort,
		Int, UnsignedInt,
		Long, UnsignedLong,
		LongLong, UnsignedLongLong,
		Float, Double, LongDouble,
		String, DataS, VectorS,
		WCSP,
		SEInfo, SkillSE, SkillInfo
	};
	
	class EData;
	typedef std::unordered_map<std::basic_string<char>, dataStorage::EData> DataStorage;
	typedef std::vector<dataStorage::EData> VectorStorage;
}

namespace dataStorage {
	
	class EData {
	protected:
		void* data = nullptr;
		DataType dtype = DataType::None;
		
		void clearData();
		
		void setData(void*, DataType);
		template<typename T> [[nodiscard]] T* getData(DataType) const;
	public:
		const bool deleteOnFree = false;
		
		explicit EData() = default;
		explicit EData(bool);
		~EData();
		
		template<typename T>               void setData   (T*);
		template<typename T> [[nodiscard]] T*   getData   () const;
		template<typename T>               void setData   (const T*);
		template<typename T>               void deleteData();
		template<typename T> void changeData(std::function<void(T&)> func) { func(*(T*)data); }
	};

	template<typename T>
	EData* makeED(bool _deleteOnFree, T object_data) {
		auto* eData = new EData(_deleteOnFree);
		eData->setData(&object_data);
		return eData;
	}
	
	template<typename T>
	T make(bool deleteOnFree, nlohmann::json&);
	template<> DataStorage   make<DataStorage  >(bool deleteOnFree, nlohmann::json& info);
	template<> VectorStorage make<VectorStorage>(bool deleteOnFree, nlohmann::json& info);
}


#endif //WCSSKILLS_DATASTORAGE_HPP
