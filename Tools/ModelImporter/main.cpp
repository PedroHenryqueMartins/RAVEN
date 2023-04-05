#include <RAVEN/Inc/RAVEN.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <cstdio>

using namespace RAVEN;
using namespace RAVEN::Graphics;
using namespace RAVEN::Math;

using BoneIndexLookUp = std::map<std::string, int>;

struct Arguments
{
	std::filesystem::path inputFileName;
	std::filesystem::path outputFileName;
	float scale = 1.0f;
	bool animOnly = false;
};

std::optional<Arguments> ParseArgs(int argc, char* argv[])
{
	if (argc < 3)
		return std::nullopt;

	Arguments args;
	args.inputFileName = argv[argc - 2];
	args.outputFileName = argv[argc - 1];

	for (int i = 1; i + 2 < argc; ++i)
	{
		if (strcmp(argv[i], "-scale") == 0)
		{
			args.scale = static_cast<float>(atof(argv[i + 1]));
		}
		else if (strcmp(argv[i], "-animOnly") == 0)
		{
			args.animOnly = true;
		}
	}

	return args;
}

void PrintUsage()
{
	printf(
		"== ModelImporter Help ==\n"
		"\n"
		"Usage:\n"
		"    ModelImporter.exe [Options] <InputFile> <OutputFile>\n"
		"\n"
		"Options:\n"
		"    -scale <number>    Scale to apply to the model."
		"    -animOnly		Export animation only." // If animation only, only can saveAnimation
		"\n"
	);
}

Vector2 ToVector2(const aiVector3D& v)
{
	return {
		static_cast<float>(v.x),
		static_cast<float>(v.y),
	};
}

Vector3 ToVector3(const aiVector3D& v)
{
	return {
		static_cast<float>(v.x),
		static_cast<float>(v.y),
		static_cast<float>(v.z)
	};
}

Quaternion ToQuaternion(const aiQuaternion& q)
{
	return {
		static_cast<float>(q.w),
		static_cast<float>(q.x),
		static_cast<float>(q.y),
		static_cast<float>(q.z)
	};
}

Matrix4 ToMatrix4(const aiMatrix4x4& m)
{
	//Take the transpose from assimp
	return {
		static_cast<float>(m.a1),
		static_cast<float>(m.b1),
		static_cast<float>(m.c1),
		static_cast<float>(m.d1),

		static_cast<float>(m.a2),
		static_cast<float>(m.b2),
		static_cast<float>(m.c2),
		static_cast<float>(m.d2),

		static_cast<float>(m.a3),
		static_cast<float>(m.b3),
		static_cast<float>(m.c3),
		static_cast<float>(m.d3),

		static_cast<float>(m.a4),
		static_cast<float>(m.b4),
		static_cast<float>(m.c4),
		static_cast<float>(m.d4)
	};
}

Color ToColor(const aiColor3D& c)
{
	return
	{
		static_cast<float>(c.r),
		static_cast<float>(c.g),
		static_cast<float>(c.b),
		1.0f
	};
}

void ExportEmbeddedTexture(const aiTexture* texture, const Arguments& args, const std::filesystem::path& fileName)
{
	printf("Extracting embedded texture %s\n", fileName.u8string().c_str());

	std::string fullFileName = args.outputFileName.u8string();
	fullFileName = fullFileName.substr(0, fullFileName.rfind('/') + 1);
	fullFileName += fileName.filename().u8string().c_str();

	FILE* file = nullptr;
	auto err = fopen_s(&file, fullFileName.c_str(), "wb");
	if (err != 0 || file == nullptr)
	{
		printf("Error: failed to open file %s for saving.\n", fullFileName.c_str());
		return;
	}

	size_t written = fwrite(texture->pcData, 1, texture->mWidth, file);
	ASSERT(written == texture->mWidth, "Error: Failed to extract embedded texture!");
	fclose(file);
}

std::string FindTexture(const aiScene* scene, const aiMaterial* aiMaterial, aiTextureType textureType, const Arguments& args, const char* suffix, uint32_t materialIndex)
{
	std::filesystem::path textureName;

	const uint32_t textureCount = aiMaterial->GetTextureCount(textureType);
	if (textureCount > 0)
	{
		aiString texturePath;
		if (aiMaterial->GetTexture(textureType, 0, &texturePath) == aiReturn_SUCCESS)
		{
			// If texture path starts with *, then the texture is embedded.
			// https://github.com/assimp/assimp/wiki/Embedded-Textures-References
			if (texturePath.C_Str()[0] == '*')
			{
				std::string fileName = args.inputFileName.u8string();
				fileName.erase(fileName.length() - 4); // remove ".fbx" extension
				fileName += suffix;
				fileName += texturePath.C_Str()[1];

				ASSERT(scene->HasTextures(), "Error: No embedded texture found!");

				int textureIndex = atoi(texturePath.C_Str() + 1);
				ASSERT(textureIndex < (int)scene->mNumTextures, "Error: Invalid texture index!");

				const aiTexture* embeddedTexture = scene->mTextures[textureIndex];
				ASSERT(embeddedTexture->mHeight == 0, "Error: Uncompressed texture found!");

				if (embeddedTexture->CheckFormat("jpg"))
					fileName += ".jpg";
				else if (embeddedTexture->CheckFormat("png"))
					fileName += ".png";
				else
					ASSERT(embeddedTexture->mHeight == 0, "Error: Unrecogized texture format!");

				ExportEmbeddedTexture(embeddedTexture, args, fileName);

				printf("Adding texture %s\n", fileName.c_str());
				textureName = fileName;
			}
			// For FBX files, embedded textures is now accessed using GetEmbeddedTexture
			// https://github.com/assimp/assimp/issues/1830
			else if (auto embeddedTexture = scene->GetEmbeddedTexture(texturePath.C_Str()); embeddedTexture)
			{
				std::filesystem::path embeddedFilePath = texturePath.C_Str();
				std::string fileName = args.inputFileName.u8string();
				fileName.erase(fileName.length() - 4); // remove ".fbx" extension
				fileName += suffix;
				fileName += "_" + std::to_string(materialIndex);
				fileName += embeddedFilePath.extension().u8string();

				ExportEmbeddedTexture(embeddedTexture, args, fileName);

				printf("Adding texture %s\n", fileName.c_str());
				textureName = fileName;
			}
			else
			{
				std::filesystem::path filePath = texturePath.C_Str();
				std::string fileName = filePath.filename().u8string();
				printf("Adding texture %s\n", fileName.c_str());
				textureName = fileName;
			}
		}
	}

	return textureName.filename().u8string().c_str();
}

//	Check if aiBone exists in skeleton, if so just return the index.
//	Otherwise, add 
int TryAddBone(const aiBone* aiBone, Skeleton& skeleton, BoneIndexLookUp& lookup)
{
	std::string name = aiBone->mName.C_Str();
	ASSERT(!name.empty(), "Error: aiBone has no name!");

	auto iter = lookup.find(name);
	if (iter != lookup.end())
		return iter->second; //	If bone is already added, just return its index.

	auto& newBone = skeleton.bones.emplace_back(std::make_unique<Bone>());
	newBone->name = std::move(name);
	newBone->index = static_cast<int>(skeleton.bones.size()) - 1;
	newBone->offsetTransform = ToMatrix4(aiBone->mOffsetMatrix);

	lookup.emplace(newBone->name, newBone->index);
	return newBone->index;
}

Bone* BuildSkeleton(const aiNode& sceneNode, Bone* parent, Skeleton& skeleton, BoneIndexLookUp& boneIndexLookup)
{
	Bone* bone = nullptr;

	std::string boneName = sceneNode.mName.C_Str();
	auto iter = boneIndexLookup.find(boneName);
	if (iter != boneIndexLookup.end())
	{
		// Bone already added to skeleton
		bone = skeleton.bones[iter->second].get();
	}
	else
	{
		// Add a new bone in the skeleton for this (possible need to generate a name for it)
		bone = skeleton.bones.emplace_back(std::make_unique<Bone>()).get();
		bone->index = static_cast<int>(skeleton.bones.size()) - 1;
		bone->offsetTransform = Matrix4::Identity;
		if (boneName.empty())
			bone->name = "NoName" + std::to_string(bone->index);
		else
			bone->name = std::move(boneName);

		// Cache the bone index
		boneIndexLookup.emplace(bone->name, bone->index);
	}

	// Link to your parent
	bone->parent = parent;
	bone->parentIndex = parent ? parent->index : -1;
	bone->toParentTransform = ToMatrix4(sceneNode.mTransformation);

	// Recurse through your children
	bone->children.reserve(sceneNode.mNumChildren);
	for (uint32_t i = 0; i < sceneNode.mNumChildren; ++i)
	{
		Bone* child = BuildSkeleton(*sceneNode.mChildren[i], bone, skeleton, boneIndexLookup);
		bone->children.push_back(child);
		bone->childIndices.push_back(child->index);
	}
	return bone;
}


int main(int argc, char* argv[])
{
	const auto argsOpt = ParseArgs(argc, argv);
	if (!argsOpt.has_value())
	{
		PrintUsage();
		return -1;
	}

	const auto& args = argsOpt.value();

	Assimp::Importer importer;
	const uint32_t flags = aiProcessPreset_TargetRealtime_Quality | aiProcess_ConvertToLeftHanded;
	const aiScene* scene = importer.ReadFile(args.inputFileName.u8string(), flags);
	if (scene == nullptr)
	{
		printf("Error: %s\n", importer.GetErrorString());
		return -1;
	}

	// scene
	// +- mesh[_][_][_][_][_][_]...
	// +- material[_][_][_][_][_]...
	// +- animation[_][_][_][_][_]...
	// +- mRootNode
	//     +-Node
	//       +- Node
	//       +- Node
	//          +- Node ...
	//     +-Node
	//       +- Node ...
	//     +-Node
	//     ...

	Model model;

	BoneIndexLookUp boneIndexLookup;

	if (scene->HasMeshes())
	{
		printf("Reading mesh data...\n");

		const uint32_t numMeshes = scene->mNumMeshes;
		model.meshData.reserve(numMeshes);

		for (uint32_t meshIndex = 0; meshIndex < numMeshes; ++meshIndex)
		{
			const auto aiMesh = scene->mMeshes[meshIndex];

			// Ignore anything that is not a triangle mesh for now (e.g. skipping points and lines)
			if (aiMesh->mPrimitiveTypes != aiPrimitiveType_TRIANGLE)
				continue;

			const uint32_t numVertices = aiMesh->mNumVertices;
			const uint32_t numFaces = aiMesh->mNumFaces;
			const uint32_t numIndices = numFaces * 3;

			auto& outMeshData = model.meshData.emplace_back();

			printf("Reading material index...\n");

			outMeshData.materialIndex = aiMesh->mMaterialIndex;

			printf("Reading vertices...\n");

			auto& outMesh = outMeshData.mesh;
			outMesh.vertices.reserve(numVertices);

			const aiVector3D* positions = aiMesh->mVertices;
			const aiVector3D* normals = aiMesh->mNormals;
			const aiVector3D* tangents = aiMesh->HasTangentsAndBitangents() ? aiMesh->mTangents : nullptr;
			const aiVector3D* texCoords = aiMesh->HasTextureCoords(0) ? aiMesh->mTextureCoords[0] : nullptr;
			for (uint32_t i = 0; i < numVertices; ++i)
			{
				auto& vertex = outMesh.vertices.emplace_back();
				vertex.position = ToVector3(positions[i]) * args.scale;
				vertex.normal = ToVector3(normals[i]);
				vertex.tangent = tangents ? ToVector3(tangents[i]) : Vector3::Zero;
				vertex.texCoord = texCoords ? ToVector2(texCoords[i]) : Vector2::Zero;
			}

			printf("Reading indices...\n");

			outMesh.indices.reserve(numIndices);

			const auto aiFaces = aiMesh->mFaces;
			for (uint32_t i = 0; i < numFaces; ++i)
			{
				const auto& aiFace = aiFaces[i];
				outMesh.indices.push_back(aiFace.mIndices[0]);
				outMesh.indices.push_back(aiFace.mIndices[1]);
				outMesh.indices.push_back(aiFace.mIndices[2]);
			}

			if (aiMesh->HasBones())
			{
				if (!model.skeleton)
					model.skeleton = std::make_unique<Skeleton>();

				//	Track how many weights we have added to each vertex so far
				std::vector<int> numWeightsAdded(outMesh.vertices.size(), 0);

				for (uint32_t meshBoneIndex = 0; meshBoneIndex < aiMesh->mNumBones; ++meshBoneIndex)
				{
					const auto aiBone = aiMesh->mBones[meshBoneIndex];
					const int boneIndex = TryAddBone(aiBone, *(model.skeleton), boneIndexLookup);

					for (uint32_t weightIndex = 0; weightIndex < aiBone->mNumWeights; ++weightIndex)
					{
						const aiVertexWeight& weight = aiBone->mWeights[weightIndex];
						auto& vertex = outMesh.vertices[weight.mVertexId];
						auto& count = numWeightsAdded[weight.mVertexId];

						//	Engine can hold at most up to four weights
						if (count < Vertex::MaxBoneWeights)
						{
							vertex.boneIndices[count] = boneIndex;
							vertex.boneWeights[count] = weight.mWeight;
							++count;
						}
					}
				}
			}
		}
	}

	if (scene->HasMaterials())
	{
		printf("Reading materials...\n");

		const uint32_t numMaterials = scene->mNumMaterials;
		model.materialData.reserve(numMaterials);

		for (uint32_t materialIndex = 0; materialIndex < numMaterials; ++materialIndex)
		{
			const auto aiMaterial = scene->mMaterials[materialIndex];

			aiColor3D ambient, diffuse, emissive, specular;
			ai_real specularPower = 1.0f;

			aiMaterial->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
			aiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
			aiMaterial->Get(AI_MATKEY_COLOR_SPECULAR, specular);
			aiMaterial->Get(AI_MATKEY_SHININESS, specularPower);

			auto& materialData = model.materialData.emplace_back();
			materialData.material.ambient = ToColor(ambient);
			materialData.material.diffuse = ToColor(diffuse);
			materialData.material.specular = ToColor(specular);
			materialData.material.emissive = ToColor(emissive);
			materialData.material.power = static_cast<float>(specularPower);
			materialData.diffuseMapName = FindTexture(scene, aiMaterial, aiTextureType_DIFFUSE, args, "_diffuse", materialIndex);
			materialData.specularMapName = FindTexture(scene, aiMaterial, aiTextureType_SHININESS, args, "_specular", materialIndex);
			materialData.displacementMapName = FindTexture(scene, aiMaterial, aiTextureType_DISPLACEMENT, args, "_bump", materialIndex);
			materialData.normalMapName = FindTexture(scene, aiMaterial, aiTextureType_NORMALS, args, "_normal", materialIndex);
		}
	}

	//	Look for skeleton data.
	if (!boneIndexLookup.empty())
	{
		printf("Building skeleton..\n");
		BuildSkeleton(*scene->mRootNode, nullptr, *(model.skeleton), boneIndexLookup);

		//	Apply scale for the skeleton
		for (auto& bone : model.skeleton->bones)
		{
			bone->offsetTransform._41 *= args.scale;
			bone->offsetTransform._42 *= args.scale;
			bone->offsetTransform._43 *= args.scale;
			bone->toParentTransform._41 *= args.scale;
			bone->toParentTransform._42 *= args.scale;
			bone->toParentTransform._43 *= args.scale;
		}
	}

	if (scene->HasAnimations())
	{
		printf("Reading animations...\n");

		for (uint32_t animIndex = 0; animIndex < scene->mNumAnimations; ++animIndex)
		{
			const auto aiAnimation = scene->mAnimations[animIndex];

			auto& animClip = model.animationClips.emplace_back();

			if (aiAnimation->mName.length > 0)
			{
				animClip.name = aiAnimation->mName.C_Str();
			}
			else
			{
				animClip.name = "Anim" + std::to_string(animIndex);
			}
			animClip.tickDuration = static_cast<float>(aiAnimation->mDuration);
			animClip.ticksPerSecond = static_cast<float>(aiAnimation->mTicksPerSecond);

			printf("Reading bone animations for %s ...\n", animClip.name.c_str());

			animClip.boneAnimations.resize(model.skeleton->bones.size());

			for (uint32_t boneAnimIndex = 0; boneAnimIndex < aiAnimation->mNumChannels; ++boneAnimIndex)
			{
				const auto aiBoneAnim = aiAnimation->mChannels[boneAnimIndex];
				const int boneIndex = boneIndexLookup[aiBoneAnim->mNodeName.C_Str()];
				auto& boneAnimation = animClip.boneAnimations[boneIndex];
				boneAnimation = std::make_unique<Animation>();

				AnimationBuilder builder;
				for (uint32_t keyIndex = 0; keyIndex < aiBoneAnim->mNumPositionKeys; ++keyIndex)
				{
					auto& posKey = aiBoneAnim->mPositionKeys[keyIndex];
					builder.AddPositionKey(ToVector3(posKey.mValue)* args.scale, static_cast<float>(posKey.mTime));
				}

				for (uint32_t keyIndex = 0; keyIndex < aiBoneAnim->mNumRotationKeys; ++keyIndex)
				{
					auto& rotKey = aiBoneAnim->mRotationKeys[keyIndex];
					builder.AddRotationKey(ToQuaternion(rotKey.mValue), static_cast<float>(rotKey.mTime));
				}

				for (uint32_t keyIndex = 0; keyIndex < aiBoneAnim->mNumScalingKeys; ++keyIndex)
				{
					auto& scaleKey = aiBoneAnim->mScalingKeys[keyIndex];
					builder.AddScaleKey(ToVector3(scaleKey.mValue), static_cast<float>(scaleKey.mTime));
				}

				*boneAnimation = builder.Get();
			}
		}
	}

	if (!args.animOnly)
	{
		printf("Saving model..\n");
		ModelIO::SaveModel(args.outputFileName, model);

		printf("Saving materials..\n");
		ModelIO::SaveMaterial(args.outputFileName, model);

		printf_s("Saving skeleton..\n");
		ModelIO::SaveSkeleton(args.outputFileName, model);
	}
	
	printf_s("Saving animations..\n");
	ModelIO::SaveAnimation(args.outputFileName, model);

	printf_s("Done setting up model..\n");

	return 0;
}