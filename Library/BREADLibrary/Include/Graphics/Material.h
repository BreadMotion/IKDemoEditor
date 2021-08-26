#pragma once

#include "Math/Color.h"
#include "Graphics/Texture.h"


namespace Bread
{
	namespace Graphics
	{
		//****************************************************************************
		// マテリアル
		//****************************************************************************
		struct Material
		{
		};

		//****************************************************************************
		// ベーシックマテリアル
		//****************************************************************************
		struct BasicMaterial : public Material
		{
			Math::Color color = Math::Color::White;
		};

		//****************************************************************************
		// スタンダードマテリアル
		//****************************************************************************
		struct StandardMaterial : public Material
		{
			ITexture* diffusemap = nullptr;
			ITexture* normalmap = nullptr;
			ITexture* specularmap = nullptr;
			Math::Color emissive = Math::Color::Black;
			Math::Color diffuse = Math::Color::White;
			Math::Color specular = Math::Color::Black;
		};

		//****************************************************************************
		// スカイドームマテリアル
		//****************************************************************************
		struct SkyDomeMaterial : public Material
		{
			ITexture* cubemap = nullptr;
		};

	} // namespace Graphics
} // namespace Bread