#include "pch.h"
#include "Math/Arithmetic.h"
#include "LightingState.h"


namespace Bread
{
	namespace Graphics
	{
		//****************************************************************************
		// ライティングステート操作オブジェクト
		//****************************************************************************
		// コンストラクタ
		LightingState::LightingState()
		{
			defaultDirLight.ambient = defaultDirLight.diffuse = defaultDirLight.specular = Math::Color(1, 1, 1, 1);
			defaultDirLight.direction = Math::Vector3(0.0f, 1.0f, 0.0f);

			noDirLight.ambient = noDirLight.diffuse = noDirLight.specular = Math::Color(0, 0, 0, 0);
			noDirLight.direction = Math::Vector3(0.0f, -1.0f, 0.0f);

			noBoxLight.ambient = noBoxLight.diffuse = noBoxLight.specular = Math::Color(0, 0, 0, 0);
			noBoxLight.min = Math::Vector3(Math::MAX_F32, Math::MAX_F32, Math::MAX_F32);
			noBoxLight.max = Math::Vector3(-Math::MAX_F32, -Math::MAX_F32, -Math::MAX_F32);

			noPointLight.ambient = noPointLight.diffuse = noPointLight.specular = Math::Color(0, 0, 0, 0);
			noPointLight.position = Math::Vector4(0, 0, 0, 0);
		}

		// 全てのライト登録解除
		void LightingState::ClearLight()
		{
			dirLights.clear();
			boxLights.clear();
			pointLights.clear();
		}

		// ディレクショナルライト登録
		void LightingState::RegisterDirLight(DirLight* light)
		{
			dirLights.insert(light);
		}

		// ディレクショナルライト登録解除
		void LightingState::UnregisterDirLight(DirLight* light)
		{
			dirLights.erase(light);
		}

		// メインディレクショナルライト取得
		DirLight* LightingState::GetProminentDirLight()
		{
			DirLight* light = GetDefaultDirLight();
			if (dirLights.size())
			{
				light = *(dirLights.begin());
			}
			return light;
		}

		// デフォルトディレクショナルライト取得
		DirLight* LightingState::GetDefaultDirLight()
		{
			return &defaultDirLight;
		}

		// ボックスライト登録
		void LightingState::RegisterBoxLight(BoxLight* light)
		{
			boxLights.insert(light);
		}

		// ボックスライト登録解除
		void LightingState::UnregisterBoxLight(BoxLight* light)
		{
			boxLights.erase(light);
		}

		// ポイントライト登録
		void LightingState::RegisterPointLight(PointLight* light)
		{
			pointLights.insert(light);
		}

		// ポイントライト登録解除
		void LightingState::UnregisterPointLight(PointLight* light)
		{
			pointLights.erase(light);
		}

		// ライト環境更新
		void LightingState::UpdateLightEnvironment(LightEnvironment& env, const Math::AABB& bounds)
		{
			env.numDirLights = 0;
			env.numBoxLights = 0;
			env.numPointLights = 0;

			// ディレクショナルライト収集
			if (dirLights.size() > 0)
			{
				for (auto it = dirLights.begin(); it != dirLights.end(); ++it)
				{
					env.dirLights[env.numDirLights++] = *(*it);
					if (env.numDirLights >= MaxDirLights)
					{
						break;
					}
				}
			}
			else
			{
				// ない場合はデフォルトライト
				env.dirLights[env.numDirLights++] = defaultDirLight;
			}

			//// ボックスライト収集
			//for (auto it = boxLights.begin(); it != boxLights.end(); ++it)
			//{
			//	BoxLight  light = *(*it);
			//	Math::AABB lightBounds(light.min, light.max);

			//	if (bounds.Intersect(lightBounds))
			//	{
			//		env.boxLights[env.numBoxLights++] = light;
			//		if (env.numBoxLights >= MaxBoxLights)
			//		{
			//			break;
			//		}
			//	}
			//}

			//// ポイントライト収集
			//for (auto it = pointLights.begin(); it != pointLights.end(); ++it)
			//{
			//	PointLight  light = *(*it);

			//	Math::Vector3 pos = Math::Vector3(light.position.x, light.position.y, light.position.z);
			//	f32 radius = light.position.w;
			//	Math::Vector3 ll(pos.x - radius, pos.y - radius, pos.z - radius);
			//	Math::Vector3 ur(pos.x + radius, pos.y + radius, pos.z + radius);
			//	Math::AABB sphereBounds(ll, ur);
			//	if (bounds.Intersect(sphereBounds))
			//	{
			//		env.pointLights[env.numPointLights++] = light;
			//		if (env.numPointLights >= MaxPointLights)
			//		{
			//			break;
			//		}
			//	}
			//}

			// 未使用のライトスロットを空に設定
			for (u32 i = env.numDirLights; i < MaxDirLights; ++i)
			{
				env.dirLights[i] = noDirLight;
			}
			for (u32 i = env.numBoxLights; i < MaxBoxLights; ++i)
			{
				env.boxLights[i] = noBoxLight;
			}
			for (u32 i = env.numPointLights; i < MaxPointLights; ++i)
			{
				env.pointLights[i] = noPointLight;
			}
		}


	}	// namespace grp
}	// namespace ys