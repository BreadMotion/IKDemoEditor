#include "FrameWork/Actor/ActorManager.h"

//Actorの更新順番を指定できるようにする

namespace Bread
{
	namespace FrameWork
	{
		//初期化
		void ActorManager::Initialize()
		{
			actors.clear();
		}

		//事前更新
		void ActorManager::PreUpdate()
		{
			for (auto& act : actors)
			{
				for (auto& child : act->GetAllChildActor())
				{
					child->PreUpdate();
				}
				act->PreUpdate();
			}
		}

		//更新
		void ActorManager::Update()
		{
			for (auto& act : actors)
			{
				for (auto& child : act->GetAllChildActor())
				{
					child->Update();

				}
				act->Update();
			}
		}

		//事後更新
		void ActorManager::NextUpdate()
		{
			for (auto& act : actors)
			{
				for (auto& child : act->GetAllChildActor())
				{
					child->NextUpdate();

				}
				act->NextUpdate();
			}
		}

		//描画
		void ActorManager::Draw()
		{
			for (auto& act : actors)
			{
				for (auto& child : act->GetAllChildActor())
				{
					child->Draw();

				}
				act->Draw();
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
			actor->Initialize();
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
	}//namespace FrameWork
}//namespace Bread