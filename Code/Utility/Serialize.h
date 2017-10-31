#pragma once

#define CEREAL_SAVE_FUNCTION_NAME TsFrameWorkSerialize
#define CEREAL_Load_FUNCTION_NAME TsFrameWorkDeSerialize
#define CEREAL_XML_STRING_VALUE "TsFramework"

#include <cereal/cereal.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/types/vector.hpp>