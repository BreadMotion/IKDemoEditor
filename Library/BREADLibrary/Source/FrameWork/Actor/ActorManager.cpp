#include "FrameWork/Actor/ActorManager.h"

//elapsedTime�̎����Ă��������l����
//Actor�̍X�V���Ԃ��w��ł���悤�ɂ���

namespace Bread
{
	namespace FrameWork
	{
		//������
		void ActorManager::Initialize()
		{
			actors.clear();
		}

		//���O�X�V
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

		//�X�V
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

			//����X�V
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

			//�`��
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