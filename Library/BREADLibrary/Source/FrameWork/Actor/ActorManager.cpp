#include "FrameWork/Actor/ActorManager.h"
#include "FrameWork/Component/Transform.h"
#include "FrameWork/Component/SpatialIndex.h"

//Actorの更新順番を指定できるようにする

namespace Bread
{
	namespace FrameWork
	{
		//初期化
		void ActorManager::Initialize()
		{
		}

		//事前更新
		void ActorManager::PreUpdate()
		{
			for (auto& act : actors)
			{
				act->PreUpdate();
			}
		}

		//更新
		void ActorManager::Update()
		{
			for (auto& act : actors)
			{
				act->Update();
			}
		}

		//事後更新
		void ActorManager::NextUpdate()
		{
			for (auto& act : actors)
			{
				act->NextUpdate();
			}
		}

		//描画
		void ActorManager::Draw()
		{
			for (auto& act : actors)
			{
				act->Draw();
			}
		}

		void ActorManager::Clear()
		{
			for (auto& act : actors)
			{
				act->Destroy();
			}
		}

		void ActorManager::Execute()
		{
		}

		void ActorManager::Exit()
		{
		}

		// アクターを追加
		void ActorManager::AddActors(std::shared_ptr<Actor> actor)
		{
			actor->AddComponent<Transform>();
			actor->AddComponent<SpatialIndexComponent>();
			actors.emplace_back(actor);
		}

		// アクターを削除
		void ActorManager::RemoveActor(std::shared_ptr<Actor> actor)
		{
			actor->Finalize();
			actor.reset();
		}

		//同じIDかチェック＆修正する
		void ActorManager::RenameSameID()
		{
			for (auto& act1 : actors)
			{
				std::string actID = act1->GetID();
				int sameActorNum = 0;
				for (auto& act2 : actors)
				{
					std::string ID = act2->GetID();
					if (act1 != act2)
					{
						if (actID == ID)
						{
							act2->SetID(ID + std::to_string(++sameActorNum));
						}
					}
				}
			}
		}

		void ActorManager::SetParentAndChild(std::shared_ptr<Actor> parent, std::shared_ptr<Actor> child)
		{
			child->SetParentActor(parent);
			parent->SetChildActor(child);
		}

	}//namespace FrameWork
}//namespace Bread