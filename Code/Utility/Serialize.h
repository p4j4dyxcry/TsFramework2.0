#pragma once
#include "../Core/Types.h"

#define CEREAL_SAVE_FUNCTION_NAME TsFrameWorkSerialize
#define CEREAL_LOAD_FUNCTION_NAME TsFrameWorkDeSerialize
#define CEREAL_XML_STRING_VALUE "TsFramework"

#include <External/cereal-1.2.2/include/cereal/cereal.hpp>
#include <External/cereal-1.2.2/include/cereal/archives/xml.hpp>
#include <External/cereal-1.2.2/include/cereal/types/vector.hpp>

#define TS_SERIALIZE_TEMPLATE_SAVE_BINARY(_class,variable)\
template <class Archive, class T> \
typename std::enable_if<cereal::traits::is_output_serializable<cereal::BinaryData<T>, Archive>::value\
    && std::is_arithmetic<T>::value && !std::is_same<T, bool>::value, void>::type\
    CEREAL_SAVE_FUNCTION_NAME(Archive & ar, _class<T> const & variable)

#define TS_SERIALIZE_TEMPLATE_LOAD_BINARY(_class,variable)\
template <class Archive, class T> \
typename std::enable_if<cereal::traits::is_input_serializable<cereal::BinaryData<T>, Archive>::value\
    && std::is_arithmetic<T>::value && !std::is_same<T, bool>::value, void>::type\
    CEREAL_LOAD_FUNCTION_NAME(Archive & ar, _class<T> & variable)

#define TS_SERIALIZE_TEMPLATE_SAVE_FILE(_class,variable)\
template <class Archive, class T>\
typename std::enable_if<!cereal::traits::is_output_serializable<cereal::BinaryData<T>, Archive>::value\
    || !std::is_arithmetic<T>::value, void>::type\
    CEREAL_SAVE_FUNCTION_NAME(Archive & ar, _class<T> const & variable)

#define TS_SERIALIZE_TEMPLATE_LOAD_FILE(_class,variable)\
template <class Archive, class T>\
typename std::enable_if<!cereal::traits::is_input_serializable<cereal::BinaryData<T>, Archive>::value\
    || !std::is_arithmetic<T>::value, void>::type\
    CEREAL_LOAD_FUNCTION_NAME(Archive & ar, _class<T> & variable)


#include "Serialize/Array.hpp"