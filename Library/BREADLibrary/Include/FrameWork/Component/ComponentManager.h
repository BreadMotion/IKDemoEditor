#pragma once
#include <memory>
#include <vector>
#include "FND/Base.h"
#include "FrameWork/Component/Component.h"

//FlyWeightの実装予定
//処理の優先度を決める処理を作る必要がある

namespace Bread
{
	namespace FrameWork
	{
		enum class ComponentUpdatePriority
		{
			LOWEST = 0,
			BELOW,
			NORMAL,
			IDLE,
			ABOVE,
			HIGHEST
		};

		class [[]] ComponentManager : public FND::Base
		{
		private:
			std::vector<std::shared_ptr<Component>> components;

		public:
			ComponentManager() = default;
			~ComponentManager() {}

		public:
			// 初期化
			void Initialize();

			// 終了化
			void Finalize();

			//事前更新
			void PreUpdate();

			// 更新
			void Update();

			//事後更新
			void NextUpdate();

			// 描画
			void Draw();

		public:
			//重複したコンポーネントの自動リネーム
			void RenameSameID();

		public:
			//コンポーネントの取得
			template<class T>
			std::shared_ptr<T> GetComponent()
			{
				for (auto& component : components)
				{
					if (std::shared_ptr<T> obj = std::dynamic_pointer_cast<T>(component))
					{
						return obj;
					}
				}
				return nullptr;
			}

			//指定したIDを持つアクターを取得する
			template <class T>
			std::shared_ptr<T> __fastcall GetComponentFromID(const std::string& ID)
			{
				for (auto& component : components)
				{
					if (component->GetID() == ID)
					{
						if (std::shared_ptr<T>obj = std::dynamic_pointer_cast<T>(component))
						{
							return obj;
						}
					}
				}
				return nullptr;
			}

			//全コンポーネントの取得
			std::vector<std::shared_ptr<Component>> GetAllComponent()
			{
				return components;
			}

			//コンポーネントの追加
			template <class T, class... Args>
			std::shared_ptr<T> __fastcall AddComponent(Args&&... args)
			{
				std::shared_ptr<T> obj = std::make_shared<T>(std::forward<Args>(args)...);
				if (obj)
				{
					SaveComponent(obj);
					return obj;
				}
				return  nullptr;
			}
		};
	}//namespace FrameWork
}//namespace Bread