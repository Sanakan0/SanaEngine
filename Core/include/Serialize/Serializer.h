#pragma once

#include "glm/fwd.hpp"
#include <string>



#include <tinyxml2.h>



/* Forward declaration */
namespace OvRendering::Resources
{
	class Model;
	class Texture;
	class Shader;
}

namespace OvAudio::Resources
{
	class Sound;
}

namespace OvCore::Resources
{
	class Material;
}

namespace SCore
{
	class Serializer
	{
	public:

		static void SerializeBoolean(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name, bool p_value);
		static void SerializeString(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name, const std::string& p_value);
		static void SerializeFloat(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name, float p_value);
		static void SerializeDouble(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name, double p_value);
		static void SerializeInt(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name, int p_value);
		static void SerializeUint(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name, unsigned p_value);
		static void SerializeInt64(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name, int64_t p_value);
		static void SerializeVec2(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name, const glm::vec2& p_value);
		static void SerializeVec3(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name, const glm::vec3& p_value);
		static void SerializeVec4(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name, const glm::vec4& p_value);
		static void SerializeQuat(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name, const glm::quat& p_value);
		static void SerializeVec2d(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name, const glm::dvec2& p_value);
		//static void SerializeColor(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name, const OvUI::Types::Color& p_value);

		static void DeserializeBoolean(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name, bool& p_out);
		static void DeserializeString(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name, std::string& p_out);
		static void DeserializeFloat(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name, float& p_out);
		static void DeserializeDouble(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name, double& p_out);
		static void DeserializeInt(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name, int& p_out);
		static void DeserializeUint(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name, unsigned& p_out);
		static void DeserializeInt64(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name, int64_t& p_out);
		static void DeserializeVec2(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name, glm::vec2& p_out);
		static void DeserializeVec3(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name, glm::vec3& p_out);
		static void DeserializeVec4(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name, glm::vec4& p_out);
		static void DeserializeQuat(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name, glm::quat& p_out);
		static void DeserializeVec2d(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name, glm::dvec2& p_out);
		//static void DeserializeColor(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name, OvUI::Types::Color& p_out);

		
		#pragma endregion
	};
}