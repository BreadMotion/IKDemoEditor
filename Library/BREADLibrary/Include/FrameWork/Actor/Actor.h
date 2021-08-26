#pragma once
#include <memory>
#include <map>
#include <vector>
#include <string>
#include "math/BreadMath.h"

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
			Actor() {}
			virtual ~Actor() {}

		public:
			// 生成
			static std::shared_ptr<Actor> Create();

			// 初期化
			virtual void Initialize();

			// 終了化
			virtual void Finalize();

			//事前更新
			virtual void PreUpdate(const f32& dt);

			// 更新
			virtual void Update(const f32& dt);

			//事後更新
			virtual void nextUpdate(const f32& dt);

			// 描画
			virtual void Draw(const f32& dt);

		public:
			// 親アクターの設定
			void SetParentActor(std::shared_ptr<Actor> actor);

			// アクターを追加
			void AddChildActors(std::shared_ptr<Actor> actor);

			// アクターを削除
			void RemoveChildActor(std::shared_ptr<Actor> actor);

			// コンポーネントを追加
			void SaveComponent(std::shared_ptr<Component> component);

			// コンポーネントを削除
			void RemoveComponent(std::shared_ptr<Component> component);

			//IDの設定
			void SetID(const std::string& id);

			//IDの取得
			const std::string& GetID();

			//同じIDかチェック＆修正する
			void RenameSameID();

		public:
			//子アクターの取得
			template<class T>
			T* GetChildActor()
			{
				for (auto& act : children)
				{
					std::shared_ptr<T> obj = std::dynamic_pointer_cast<T>(act);
					if (obj != nullptr)
					{
						return obj.get();
					}
				}
				return nullptr;
			}

			template <class T>
			T* GetChildActorFromID(const std::string& name)
			{
				for (auto& act : children)
				{
					if (act->ID != name) continue;

					std::shared_ptr<T> obj = std::dynamic_pointer_cast<T>(act);
					if (obj != nullptr)
					{
						return obj.get();
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
			T* AddChildActor(Args&&... args)
			{
				std::shared_ptr<T> obj = std::make_shared<T>(std::forward<Args>(args)...);
				AddChildActors(obj);

				return obj.get();
			}

		public:
			//コンポーネントの取得
			template<class T>
			T* GetComponent()
			{
				for (auto& component : components)
				{
					if (std::shared_ptr<T> obj = std::dynamic_pointer_cast<T>(component))
					{
						return obj.get();
					}
				}
				return nullptr;
			}

			template <class T>
			T* GetComponentFromID(const std::string& ID)
			{
				for (auto& component : components)
				{
					if (component->GetID() == ID)
					{
						if (std::shared_ptr<T>obj = std::dynamic_pointer_cast<T>(component))
						{
							return obj.get();
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
			T* AddComponent(Args&&... args)
			{
				std::shared_ptr<T> obj = std::make_shared<T>(std::forward<Args>(args)...);
				SaveComponent(obj);

				return obj.get();
			}

		public://GUI用
			std::map<std::string, Math::Matrix*>         mAry;
			std::map<std::string, Math::Quaternion*> qAry;
			std::map<std::string, Math::Color*>           cAry;
			std::map<std::string, Math::Vector2*>        v2Ary;
			std::map<std::string, Math::Vector3*>        v3Ary;
			std::map<std::string, Math::Vector4*>        v4Ary;
		};
	}
}