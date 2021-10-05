#include "FrameWork/Actor/ActorManager.h"

//elapsedTimeの持ってきかたを考える
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
				for (auto& child : act.second->GetAllChildActor())
				{
					child->PreUpdate();
				}
				act.second->PreUpdate();
			}
		}

		//更新
		void ActorManager::Update()
		{
			for (auto& act : actors)
			{
				for (auto& child : act.second->GetAllChildActor())
				{
					child->Update();

				}
				act.second->Update();
			}
		}

			//事後更新
		void ActorManager::NextUpdate()
		{
			for (auto& act : actors)
			{
				for (auto& child : act.second->GetAllChildActor())
				{
					child->NextUpdate();

				}
				act.second->NextUpdate();
			}
		}

			//描画
		void ActorManager::Draw()
		{
			for (auto& act : actors)
			{
				for (auto& child : act.second->GetAllChildActor())
				{
					child->Draw();

				}
				act.second->Draw();
			}
		}

		void ActorManager::Execute()
		{
		}

		void ActorManager::Exit()
		{
		}
	}//namespace FrameWork
}//namespace Bread