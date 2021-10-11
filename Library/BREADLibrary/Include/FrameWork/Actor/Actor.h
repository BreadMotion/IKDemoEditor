#pragma once
#include <memory>
#include <map>
#include <vector>
#include <string>
#include "math/BreadMath.h"
#include "FrameWork/Component/Component.h"

namespace Bread
{
	namespace FrameWork
	{
		class Component;
		class Actor : public std::enable_shared_from_this<Actor>
		{
		protected:
			std::weak_ptr<Actor>                    parent;
			std::vector<std::shared_ptr<Actor>>     children;
			std::vector<std::shared_ptr<Component>> components;
			std::string                             ID;

		public:
			Actor();
			virtual ~Actor() {}

		public:
			// 生成
			static std::shared_ptr<Actor> Create();

			// 初期化
			virtual void Initialize();

			// 終了化
			virtual void Finalize();

			//事前更新
			virtual void __fastcall PreUpdate();

			// 更新
			virtual void __fastcall Update();

			//事後更新
			virtual void __fastcall NextUpdate();

			// 描画
			virtual void __fastcall Draw();

		public:
			// 親アクターの設定
			void __fastcall SetParentActor(std::shared_ptr<Actor> actor);

			// アクターを追加
			void __fastcall AddChildActors(std::shared_ptr<Actor> actor);

			// アクターを削除
			void __fastcall RemoveChildActor(std::shared_ptr<Actor> actor);

			// コンポーネントを追加
			void __fastcall SaveComponent(std::shared_ptr<Component> component);

			// コンポーネントを削除
			void __fastcall RemoveComponent(std::shared_ptr<Component> component);

			//IDの設定
			void __fastcall SetID(const std::string& id);

			//IDの取得
			const std::string& GetID();

			//同じIDかチェック＆修正する
			void RenameSameID();

		public:
			//子アクターの取得
			template<class T>
			std::shared_ptr<T> GetChildActor()
			{
				for (auto& act : children)
				{
					std::shared_ptr<T> obj = std::dynamic_pointer_cast<T>(act);
					if (obj != nullptr)
					{
						return obj;
					}
				}
				return nullptr;
			}

			//指定したIDを持つ子アクターを取得する
			template <class T>
			std::shared_ptr<T> __fastcall GetChildActorFromID(const std::string& name)
			{
				for (auto& act : children)
				{
					if (act->ID != name) continue;

					std::shared_ptr<T> obj = std::dynamic_pointer_cast<T>(act);
					if (obj != nullptr)
					{
						return obj;
					}
				}
				return nullptr;
			}

			//全子アクターの取得
			std::vector<std::shared_ptr<Actor>>&GetAllChildActor()
			{
				return children;
			}

			//子アクターの追加
			template <class T, class... Args>
			std::shared_ptr<T> __fastcall AddChildActor(Args&&... args)
			{
				std::shared_ptr<T> obj = std::make_shared<T>(std::forward<Args>(args)...);
				AddChildActors(obj);

				return obj;
			}

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
			template <class T,class... Args>
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