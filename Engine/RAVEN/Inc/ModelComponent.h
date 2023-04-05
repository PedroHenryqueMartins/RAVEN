#pragma once

#include "Component.h"

namespace RAVEN
{
	class ModelComponent final : public Component
	{
	public:
		SET_TYPE_ID(ComponentId::Model);

		void Initialize() override;
		void Terminate() override;

		void SetFileName(const char* fileName) { mFileName = fileName; }
		void AddAnimation(const char* animationFileName)
		{
			mAnimationFileNames.push_back(animationFileName);
		}

		Graphics::ModelID GetModelID() const { return mModelID; }
		const Graphics::Model& GetModel() const
		{
			return *Graphics::ModelManager::Get()->GetModel(mModelID);
		}

	private:
		std::string mFileName;
		std::vector<std::string> mAnimationFileNames;
		Graphics::ModelID mModelID = 0;
		//Math::Quaternion mRotationCorrection = Math::Quaternion::Identity;
	};
}