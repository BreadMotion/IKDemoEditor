#pragma once

#include <set>
#include "Graphics/Light.h"


namespace Bread
{
	namespace Graphics
	{
		//****************************************************************************
		// ライティングステート操作オブジェクト
		//****************************************************************************
		class LightingState : public ILightingState
		{
		private:
			DirLight defaultDirLight;
			std::set<DirLight*> dirLights;
			std::set<BoxLight*> boxLights;
			std::set<PointLight*> pointLights;

			DirLight noDirLight;
			BoxLight noBoxLight;
			PointLight noPointLight;

		public:
			LightingState();
			~LightingState() override {}

		public:
			// 全てのライト登録解除
			void ClearLight() override;

			// ディレクショナルライト登録
			void RegisterDirLight(DirLight* light) override;

			// ディレクショナルライト登録解除
			void UnregisterDirLight(DirLight* light) override;

			// メインディレクショナルライト取得
			DirLight* GetProminentDirLight() override;

			// デフォルトディレクショナルライト取得
			DirLight* GetDefaultDirLight() override;

			// ボックスライト登録
			void RegisterBoxLight(BoxLight* light) override;

			// ボックスライト登録解除
			void UnregisterBoxLight(BoxLight* light) override;

			// ポイントライト登録
			void RegisterPointLight(PointLight* light) override;

			// ポイントライト登録解除
			void UnregisterPointLight(PointLight* light) override;

			// ライト環境更新
			void UpdateLightEnvironment(LightEnvironment& env, const Math::AABB& bounds) override;
		};
	} // namespace Graphics
} // namespace Bread