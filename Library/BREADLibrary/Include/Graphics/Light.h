#pragma once

#include "FND/Base.h"
#include "Math/AABB.h"
#include "Math/Color.h"
#include "Math/Vector.h"


namespace Bread
{
	namespace Graphics
	{
		static const u32 MaxDirLights = 2;
		static const u32 MaxBoxLights = 2;
		static const u32 MaxPointLights = 4;

		//****************************************************************************
		// フォグ
		//****************************************************************************
		struct ExpFog
		{
			s32 enabled = false;
			f32 density = 0.0f;
			f32 range = 0.0f;
			f32 fpad = 0.0f;
			Math::Color color = Math::Color::White;
		};

		//****************************************************************************
		// ライト基底
		//****************************************************************************
		struct Light
		{
			Math::Color ambient;
			Math::Color diffuse;
			Math::Color specular;
		};

		//****************************************************************************
		// ディレクショナルライト
		//****************************************************************************
		struct DirLight : public Light
		{
			Math::Vector3 direction;
			f32 padding1;
		};

		//****************************************************************************
		// ボックスライト
		//****************************************************************************
		struct BoxLight : public DirLight
		{
			Math::Vector3 min;
			f32 padding2;
			Math::Vector3 max;
			f32 padding3;
			Math::Vector4 attenuation;
		};

		//****************************************************************************
		// ポイントライト
		//****************************************************************************
		struct PointLight : public Light
		{
			Math::Vector4 attenuation;
			Math::Vector4 position; //(xyz, radius)
		};

		//****************************************************************************
		// ライト環境
		//****************************************************************************
		struct LightEnvironment
		{
			DirLight dirLights[MaxDirLights];
			BoxLight boxLights[MaxBoxLights];
			PointLight pointLights[MaxPointLights];
			u32 numDirLights;
			u32 numBoxLights;
			u32 numPointLights;
			u32 padding1;
		};

		//****************************************************************************
		// ライティングステート操作インターフェース
		//****************************************************************************
		class ILightingState : public FND::Base
		{
		public:
			// 全てのライト登録解除
			virtual void ClearLight() = 0;

			// ディレクショナルライト登録
			virtual void RegisterDirLight(DirLight* light) = 0;

			// ディレクショナルライト登録解除
			virtual void UnregisterDirLight(DirLight* light) = 0;

			// メインディレクショナルライト取得
			virtual DirLight* GetProminentDirLight() = 0;

			// デフォルトディレクショナルライト取得
			virtual DirLight* GetDefaultDirLight() = 0;

			// ボックスライト登録
			virtual void RegisterBoxLight(BoxLight* light) = 0;

			// ボックスライト登録解除
			virtual void UnregisterBoxLight(BoxLight* light) = 0;

			// ポイントライト登録
			virtual void RegisterPointLight(PointLight* light) = 0;

			// ポイントライト登録解除
			virtual void UnregisterPointLight(PointLight* light) = 0;

			// ライト環境更新
			virtual void UpdateLightEnvironment(LightEnvironment& env, const Math::AABB& bounds) = 0;
		};

	} // namespace Graphics
} // namespace Bread