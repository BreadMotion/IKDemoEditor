#include "pch.h"
#include "FrameWork/Actor/Actor.h"
#include "FrameWork/Component/Component.h"


namespace Bread
{
	namespace FrameWork
	{
		// 生成
		std::shared_ptr<Actor> Actor::Create()
		{
			return std::make_shared<Actor>();
		}

		// 初期化
		void Actor::Initialize()
		{
			for (auto child : children)
			{
				child->Initialize();
			}

			for (auto component : components)
			{
				component->Initialize();
			}
		}

		// 終了化
		void Actor::Finalize()
		{
			for (auto child : children)
			{
				child->Finalize();
			}

			for (auto component : components)
			{
				component->Finalize();
			}
		}

		//事前更新
		void Actor::PreUpdate(const f32& dt)
		{
			for (auto child : children)
			{
				child->PreUpdate(dt);
			}

			RenameSameID();

			for (auto component : components)
			{
				component->PreUpdate(dt);
			}
		}

		// 更新
		void Actor::Update(const f32& dt)
		{
			for (auto child : children)
			{
				child->Update(dt);
			}

			for (auto component : components)
			{
				component->Update(dt);
			}
		}

		//事後更新
		void Actor::nextUpdate(const f32& dt)
		{
			for (auto child : children)
			{
				child->nextUpdate(dt);
			}

			RenameSameID();

			for (auto component : components)
			{
				component->NextUpdate(dt);
			}
		}

		// 描画
		void Actor::Draw(const f32& dt)
		{
			for (auto child : children)
			{
				child->Draw(dt);
			}

			for (auto component : components)
			{
				component->Draw(dt);
			}
		}

		// 親アクターの設定
		void Actor::SetParentActor(std::shared_ptr<Actor> actor)
		{
			parent = actor;
		}

		// アクターを追加
		void Actor::AddChildActors(std::shared_ptr<Actor> actor)
		{
			actor->SetParentActor(shared_from_this());
			actor->Initialize();

			children.emplace_back(actor);
		}

		// アクターを削除
		void Actor::RemoveChildActor(std::shared_ptr<Actor> actor)
		{
			actor->SetParentActor(nullptr);
			actor->Finalize();

			auto&& it = std::find(children.begin(), children.end(), actor);
			if (it != children.end())
			{
				children.erase(it);
			}
		}

		// コンポーネントを保存
		void Actor::SaveComponent(std::shared_ptr<Component> component)
		{
			component->SetOwner(shared_from_this());
			components.emplace_back(component);
		}

		// コンポーネントを削除
		void Actor::RemoveComponent(std::shared_ptr<Component> component)
		{
			component->SetOwner(nullptr);
			component->Destruct();

			auto&& it = std::find(components.begin(), components.end(), component);
			if (it != components.end())
			{
				components.erase(it);
			}
		}

		//IDの設定
		void Actor::SetID(const std::string& id)
		{
			ID = id;
		}

		//IDの取得
		const std::string& Actor::GetID()
		{
			return ID;
		}

		//同じIDかチェック＆修正する
		void Actor::RenameSameID()
		{
			for (auto& component1 : components)
			{
				std::string compareID       = component1->GetID();
				int sameComponentNum = 0;
				for (auto& component2 : components)
				{
					std::string ID = component2->GetID();
					if (component1 != component2)
					{
						if (compareID == ID)
						{
							component2->SetID(ID + std::to_string(++sameComponentNum));
						}
					}
				}
			}
		}
	}
}