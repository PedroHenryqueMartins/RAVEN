#pragma once

#include "Animator.h"
#include "BlendState.h"
#include "DepthStencilState.h"
#include "Material.h"
#include "MeshBuffer.h"
#include "Skeleton.h"
#include "TextureManager.h"
#include "Transform.h"


namespace RAVEN::Graphics
{
	struct Model;

	class RenderObject final
	{
		

	public:
		void Terminate();

		Transform transform;
		Material material;

		MeshBuffer meshBuffer;
		TextureID diffuseMapID = 0;
		TextureID specularMapID = 0;
		TextureID displacementMapID = 0;
		TextureID normalMapID = 0;

		BlendState::Mode blendMode = BlendState::Mode::Opaque;

		DepthStencilState::DepthTest depthTest = DepthStencilState::DepthTest::Enable;
		DepthStencilState::DepthWrite depthWrite = DepthStencilState::DepthWrite::Enable;

		const Animator* animator = nullptr;
		std::vector<Animator> mAnimators;
		const Skeleton* skeleton = nullptr;

	};

	using RenderGroup = std::vector<RenderObject>;

	[[nodiscard]] RenderGroup CreateRenderGroup(const Model& model, const Animator* animator = nullptr);
	void CleanUpRenderGroup(RenderGroup& renderGroup);

	template <class Effect>
	void DrawRenderGroup(Effect& effect, const RenderGroup& renderGroup)
	{
		for (auto& renderObject : renderGroup)
			effect.Render(renderObject);
	}
}