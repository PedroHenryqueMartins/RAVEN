#include "Precompiled.h"
#include "ModelIO.h"

#include "Animation.h"
#include "Model.h"

using namespace RAVEN;
using namespace RAVEN::Graphics;

namespace
{
	void WriteMatrix(FILE* file, const Math::Matrix4& m)
	{
		fprintf(file, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n",
			m._11, m._12, m._13, m._14,
			m._21, m._22, m._23, m._24,
			m._31, m._32, m._33, m._34,
			m._41, m._42, m._43, m._44);
	}

	void ReadMatrix(FILE* file, Math::Matrix4& m)
	{
		fscanf_s(file, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n",
			&m._11, &m._12, &m._13, &m._14,
			&m._21, &m._22, &m._23, &m._24,
			&m._31, &m._32, &m._33, &m._34,
			&m._41, &m._42, &m._43, &m._44);
	}
}

void AnimationIO::Write(FILE* file, const Animation& animation)
{
	fprintf(file, "PositionKeys: %d\n", static_cast<uint32_t>(animation.mPositionKeys.size()));
	if (!animation.mPositionKeys.empty())
	{
		for (auto& posKey : animation.mPositionKeys)
			fprintf(file, "%f %f %f %f\n", posKey.time, posKey.key.x, posKey.key.y, posKey.key.z);
	}
	fprintf(file, "RotationKeys: %d\n", static_cast<uint32_t>(animation.mRotationKeys.size()));
	if (!animation.mRotationKeys.empty())
	{
		for (auto& rotKey : animation.mRotationKeys)
			fprintf(file, "%f %f %f %f %f\n", rotKey.time, rotKey.key.w, rotKey.key.x, rotKey.key.y, rotKey.key.z);
	}
	fprintf(file, "ScaleKeys: %d\n", static_cast<uint32_t>(animation.mScaleKeys.size()));
	if (!animation.mScaleKeys.empty())
	{
		for (auto& scaleKey : animation.mScaleKeys)
			fprintf(file, "%f %f %f %f\n", scaleKey.time, scaleKey.key.x, scaleKey.key.y, scaleKey.key.z);
	}
}

void AnimationIO::Read(FILE* file, Animation& animation)
{
	uint32_t positionKeys = 0;
	fscanf_s(file, "PositionKeys: %d\n", &positionKeys);
	if (positionKeys > 0)
	{
		animation.mPositionKeys.resize(positionKeys);
		for (auto& posKey : animation.mPositionKeys)
			fscanf_s(file, "%f %f %f %f\n", &posKey.time, &posKey.key.x, &posKey.key.y, &posKey.key.z);
	}

	uint32_t rotationKeys = 0;
	fscanf_s(file, "RotationKeys: %d\n", &rotationKeys);
	if (rotationKeys > 0)
	{
		animation.mRotationKeys.resize(rotationKeys);
		for (auto& rotKey : animation.mRotationKeys)
			fscanf_s(file, "%f %f %f %f %f\n", &rotKey.time, &rotKey.key.w, &rotKey.key.x, &rotKey.key.y, &rotKey.key.z);
	}

	uint32_t scaleKeys = 0;
	fscanf_s(file, "ScaleKeys: %d\n", &scaleKeys);
	if (scaleKeys > 0)
	{
		animation.mScaleKeys.resize(scaleKeys);
		for (auto& scaleKey : animation.mScaleKeys)
			fscanf_s(file, "%f %f %f %f\n", &scaleKey.time, &scaleKey.key.x, &scaleKey.key.y, &scaleKey.key.z);
	}
}


void ModelIO::SaveModel(std::filesystem::path filePath, const Model& model)
{
	if (model.meshData.empty())
		return;

	FILE* file = nullptr;
	fopen_s(&file, filePath.u8string().c_str(), "w");
	if (file == nullptr)
		return;

	const uint32_t meshCount = static_cast<uint32_t>(model.meshData.size());
	fprintf_s(file, "MeshCount: %u\n", meshCount);
	for (uint32_t i = 0; i < meshCount; ++i)
	{
		const auto& meshData = model.meshData[i];
		fprintf_s(file, "MaterialIndex: %d\n", meshData.materialIndex);

		const auto& mesh = meshData.mesh;
		const uint32_t vertexCount = static_cast<uint32_t>(mesh.vertices.size());
		fprintf_s(file, "VertexCount: %u\n", vertexCount);

		for (auto& vertex : mesh.vertices)
		{
			fprintf_s(file, "%f %f %f %f %f %f %f %f %f %f %f %d %d %d %d %f %f %f %f\n",
				vertex.position.x, vertex.position.y, vertex.position.z,
				vertex.normal.x, vertex.normal.y, vertex.normal.z,
				vertex.tangent.x, vertex.tangent.y, vertex.tangent.z,
				vertex.texCoord.x, vertex.texCoord.y,
				vertex.boneIndices[0], vertex.boneIndices[1], vertex.boneIndices[2], vertex.boneIndices[3],
				vertex.boneWeights[0], vertex.boneWeights[1], vertex.boneWeights[2], vertex.boneWeights[3]
			);
		}

		const uint32_t indexCount = static_cast<uint32_t>(mesh.indices.size());
		fprintf_s(file, "IndexCount: %u\n", indexCount);

		for (size_t i = 0; i < indexCount; i += 3)
		{
			fprintf_s(file, "%d %d %d\n",
				mesh.indices[i],
				mesh.indices[i + 1u],
				mesh.indices[i + 2u]);
		}
	}

	fclose(file);
}

void ModelIO::LoadModel(std::filesystem::path filePath, Model& model)
{
	FILE* file = nullptr;
	fopen_s(&file, filePath.u8string().c_str(), "r");
	if (file == nullptr)
		return;

	uint32_t meshCount = 0;
	fscanf_s(file, "MeshCount: %u\n", &meshCount);

	model.meshData.resize(meshCount);
	for (uint32_t i = 0; i < meshCount; ++i)
	{
		auto& meshData = model.meshData[i];
		fscanf_s(file, "MaterialIndex: %d\n", &meshData.materialIndex);

		auto& mesh = meshData.mesh;

		uint32_t vertexCount = 0;
		fscanf_s(file, "VertexCount: %d\n", &vertexCount);
		mesh.vertices.resize(vertexCount);

		for (auto& vertex : mesh.vertices)
		{
			fscanf_s(file, "%f %f %f %f %f %f %f %f %f %f %f %d %d %d %d %f %f %f %f\n",
				&vertex.position.x, &vertex.position.y, &vertex.position.z,
				&vertex.normal.x, &vertex.normal.y, &vertex.normal.z,
				&vertex.tangent.x, &vertex.tangent.y, &vertex.tangent.z,
				&vertex.texCoord.x, &vertex.texCoord.y,
				&vertex.boneIndices[0], &vertex.boneIndices[1], &vertex.boneIndices[2], &vertex.boneIndices[3],
				&vertex.boneWeights[0], &vertex.boneWeights[1], &vertex.boneWeights[2], &vertex.boneWeights[3]
			);
		}

		uint32_t indexCount = 0;
		fscanf_s(file, "IndexCount: %u\n", &indexCount);
		mesh.indices.resize(indexCount);

		for (size_t i = 0; i < indexCount; i += 3)
		{
			fscanf_s(file, "%d %d %d\n",
				&mesh.indices[i],
				&mesh.indices[i + 1u],
				&mesh.indices[i + 2u]);
		}
	}

	fclose(file);
}

void ModelIO::SaveMaterial(std::filesystem::path filePath, const Model& model)
{
	if (model.materialData.empty())
		return;

	filePath.replace_extension("material");

	FILE* file = nullptr;
	fopen_s(&file, filePath.u8string().c_str(), "w");
	if (file == nullptr)
		return;

	const uint32_t materialCount = static_cast<uint32_t>(model.materialData.size());
	fprintf_s(file, "MaterialCount: %u\n", materialCount);
	for (const auto& material : model.materialData)
	{
		auto& mat = material.material;
		fprintf_s(file, "%f %f %f %f \n", mat.ambient.r, mat.ambient.g, mat.ambient.b, mat.ambient.a);
		fprintf_s(file, "%f %f %f %f \n", mat.diffuse.r, mat.diffuse.g, mat.diffuse.b, mat.diffuse.a);
		fprintf_s(file, "%f %f %f %f \n", mat.specular.r, mat.specular.g, mat.specular.b, mat.specular.a);
		fprintf_s(file, "%f %f %f %f \n", mat.emissive.r, mat.emissive.g, mat.emissive.b, mat.emissive.a);
		fprintf_s(file, "%f \n", mat.power);

		fprintf_s(file, "%s\n", material.diffuseMapName.empty() ? "none" : material.diffuseMapName.c_str());
		fprintf_s(file, "%s\n", material.specularMapName.empty() ? "none" : material.specularMapName.c_str());
		fprintf_s(file, "%s\n", material.displacementMapName.empty() ? "none" : material.displacementMapName.c_str());
		fprintf_s(file, "%s\n", material.normalMapName.empty() ? "none" : material.normalMapName.c_str());
	}

	fclose(file);
}

void ModelIO::LoadMaterial(std::filesystem::path filePath, Model& model)
{

	filePath.replace_extension("material");

	FILE* file = nullptr;
	fopen_s(&file, filePath.u8string().c_str(), "r");
	if (file == nullptr)
		return;

	uint32_t materialCount = 0;
	fscanf_s(file, "MaterialCount: %u\n", &materialCount);

	auto TryReadTextureName = [&](auto& fileName)
	{
		char buffer[128]{};
		fscanf_s(file, "%s\n", buffer, (uint32_t)sizeof(buffer));
		if (strcmp(buffer, "none") != 0)
			fileName = filePath.replace_filename(buffer).u8string();
	};

	model.materialData.resize(materialCount);
	for (auto& material : model.materialData)
	{
		auto& mat = material.material;
		fscanf_s(file, "%f %f %f %f \n", &mat.ambient.r, &mat.ambient.g, &mat.ambient.b, &mat.ambient.a);
		fscanf_s(file, "%f %f %f %f \n", &mat.diffuse.r, &mat.diffuse.g, &mat.diffuse.b, &mat.diffuse.a);
		fscanf_s(file, "%f %f %f %f \n", &mat.specular.r, &mat.specular.g, &mat.specular.b, &mat.specular.a);
		fscanf_s(file, "%f %f %f %f \n", &mat.emissive.r, &mat.emissive.g, &mat.emissive.b, &mat.emissive.a);
		fscanf_s(file, "%f \n", &mat.power);
		TryReadTextureName(material.diffuseMapName);
		TryReadTextureName(material.specularMapName);
		TryReadTextureName(material.displacementMapName);
		TryReadTextureName(material.normalMapName);
	}

	fclose(file);
}

void ModelIO::SaveSkeleton(std::filesystem::path filePath, const Model& model)
{
	if (!model.skeleton)
		return;

	filePath.replace_extension("skeleton");

	FILE* file = nullptr;
	fopen_s(&file, filePath.u8string().c_str(), "w");
	if (file == nullptr)
		return;

	const uint32_t boneCount = static_cast<uint32_t>(model.skeleton->bones.size());
	fprintf_s(file, "BoneCount: %u\n", boneCount);
	for (const auto& bone : model.skeleton->bones)
	{
		
		fprintf_s(file, "Name: %s\n", bone->name.c_str());
		fprintf_s(file, "Index: %d\n", bone->index);
		fprintf_s(file, "ParentIndex: %d\n", bone->parentIndex);
		const uint32_t childCount = static_cast<uint32_t>(bone->childIndices.size());
		fprintf_s(file, "ChildCount: %u\n", childCount);
		for (const auto& indices : bone->childIndices)
		{
			fprintf_s(file, "%d ", indices);
		}
		fprintf_s(file, "\n");
		fprintf_s(file, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n", bone->toParentTransform._11, bone->toParentTransform._12, bone->toParentTransform._13, bone->toParentTransform._14, 
			bone->toParentTransform._21, bone->toParentTransform._22, bone->toParentTransform._23, bone->toParentTransform._24, 
			bone->toParentTransform._31, bone->toParentTransform._32, bone->toParentTransform._33, bone->toParentTransform._34, 
			bone->toParentTransform._41, bone->toParentTransform._42, bone->toParentTransform._43, bone->toParentTransform._44);
		fprintf_s(file, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n", bone->offsetTransform._11, bone->offsetTransform._12, bone->offsetTransform._13, bone->offsetTransform._14,
			bone->offsetTransform._21, bone->offsetTransform._22, bone->offsetTransform._23, bone->offsetTransform._24,
			bone->offsetTransform._31, bone->offsetTransform._32, bone->offsetTransform._33, bone->offsetTransform._34,
			bone->offsetTransform._41, bone->offsetTransform._42, bone->offsetTransform._43, bone->offsetTransform._44);
	}
	

	fclose(file);
}

void ModelIO::LoadSkeleton(std::filesystem::path filePath, Model& model)
{
	filePath.replace_extension("skeleton");

	FILE* file = nullptr;
	fopen_s(&file, filePath.u8string().c_str(), "r");
	if (file == nullptr)
		return;

	model.skeleton = std::make_unique<Skeleton>();
	uint32_t boneCount = 0;
	fscanf_s(file, "BoneCount: %u\n", &boneCount);

	for (uint32_t i = 0; i < boneCount; ++i)
	{
		auto bone = model.skeleton->bones.emplace_back(std::make_unique<Bone>()).get();
		char buffer[128]{};
		fscanf_s(file, "Name: %s\n", buffer, (uint32_t)sizeof(buffer));
		bone->name = buffer;
		fscanf_s(file, "Index: %d\n", &bone->index);
		fscanf_s(file, "ParentIndex: %d\n", &bone->parentIndex);
		uint32_t childCount = 0;
		fscanf_s(file, "ChildCount: %u\n", &childCount);
		bone->childIndices.resize(childCount);
		for (int i = 0; i < bone->childIndices.size(); ++i)
		{
			fscanf_s(file, "%d ", &bone->childIndices[i]);
		}
		fscanf_s(file, "\n");
		fscanf_s(file, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n", &bone->toParentTransform._11, &bone->toParentTransform._12, &bone->toParentTransform._13, &bone->toParentTransform._14,
			&bone->toParentTransform._21, &bone->toParentTransform._22, &bone->toParentTransform._23, &bone->toParentTransform._24,
			&bone->toParentTransform._31, &bone->toParentTransform._32, &bone->toParentTransform._33, &bone->toParentTransform._34,
			&bone->toParentTransform._41, &bone->toParentTransform._42, &bone->toParentTransform._43, &bone->toParentTransform._44);
		fscanf_s(file, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n", &bone->offsetTransform._11, &bone->offsetTransform._12, &bone->offsetTransform._13, &bone->offsetTransform._14,
			&bone->offsetTransform._21, &bone->offsetTransform._22, &bone->offsetTransform._23, &bone->offsetTransform._24,
			&bone->offsetTransform._31, &bone->offsetTransform._32, &bone->offsetTransform._33, &bone->offsetTransform._34,
			&bone->offsetTransform._41, &bone->offsetTransform._42, &bone->offsetTransform._43, &bone->offsetTransform._44);
	}

	fclose(file);

	for (int b = 0; b < boneCount; ++b)
	{
		Bone* bone = model.skeleton->bones[b].get();
		if (bone->parentIndex == -1)
		{
			model.skeleton->root = bone;
		}
		else
		{
			bone->parent = model.skeleton->bones[bone->parentIndex].get();
		}


		for (int i = 0; i < bone->childIndices.size(); ++i)
		{
			bone->children.push_back(model.skeleton->bones[bone->childIndices[i]].get());
		}
	}
}

void ModelIO::SaveAnimation(std::filesystem::path filePath, const Model& model)
{
	if (model.animationClips.empty())
		return;

	filePath.replace_extension("animset");

	FILE* file = nullptr;
	fopen_s(&file, filePath.u8string().c_str(), "w");
	if (file == nullptr)
		return;

	const uint32_t clipCount = static_cast<uint32_t>(model.animationClips.size());
	fprintf_s(file, "ClipCount: %u\n", clipCount);
	for (auto& animClip : model.animationClips)
	{
		fprintf_s(file, "Name: %s\n", animClip.name.c_str());
		fprintf_s(file, "Duration: %f\n", animClip.tickDuration);
		fprintf_s(file, "TicksPerSecond: %f\n", animClip.ticksPerSecond);

		const uint32_t animCount = static_cast<uint32_t>(animClip.boneAnimations.size());
		fprintf_s(file, "AnimCount: %u\n", animCount);
		for (auto& animation : animClip.boneAnimations)
		{
			if (animation)
			{
				fprintf_s(file, "[Animation]\n");
				AnimationIO::Write(file, *animation);
			}
			else
			{
				fprintf_s(file, "[Empty]\n");
			}
		}
	}

	fclose(file);
}

void ModelIO::LoadAnimation(std::filesystem::path filePath, Model& model)
{
	filePath.replace_extension("animset");

	FILE* file = nullptr;
	fopen_s(&file, filePath.u8string().c_str(), "r");
	if (file == nullptr)
		return;

	uint32_t clipCount = 0;
	fscanf_s(file, "ClipCount: %u\n", &clipCount);

	model.animationClips.reserve(model.animationClips.size() + clipCount);
	for (uint32_t i = 0; i < clipCount; ++i)
	{
		auto& animClip = model.animationClips.emplace_back();

		char buffer[128];
		fgets(buffer, (uint32_t)sizeof(buffer), file);
		if (strncmp(buffer, "Name: ", 6) == 0)
		{
			animClip.name = buffer + 6;
			animClip.name.pop_back(); // Remove \n;
		}

		fscanf_s(file, "Duration: %f\n", &animClip.tickDuration);
		fscanf_s(file, "TicksPerSecond: %f\n", &animClip.ticksPerSecond);

		uint32_t animCount = 0;
		fscanf_s(file, "AnimCount: %u\n", &animCount);
		if (animCount > 0)
		{
			animClip.boneAnimations.resize(animCount);
			for (auto& animation : animClip.boneAnimations)
			{
				char label[128];
				fscanf_s(file, "%s\n", label, (uint32_t)sizeof(label));
				if (strcmp(label, "[Animation]") == 0)
				{
					animation = std::make_unique<Animation>();
					AnimationIO::Read(file, *animation);
				}
			}
		}
	}

	fclose(file);
}
