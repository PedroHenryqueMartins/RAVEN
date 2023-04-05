#pragma once

namespace RAVEN
{
	class GameObject;

	using CustomMake = std::function<bool(const char*, const rapidjson::Value&, GameObject&)>;

	namespace GameObjectFactory
	{
		void SetCustomMake(CustomMake customMake);
		void Make(std::filesystem::path templateFile, GameObject& gameObject);
	}
}