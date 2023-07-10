#include "Serialize/Serializer.h"
namespace SCore {

class ISerializable{
public:


		virtual void Serialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) = 0;

		virtual void Deserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) = 0;


		virtual ~ISerializable() = default;

};


}