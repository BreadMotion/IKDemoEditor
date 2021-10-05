#include "FrameWork/Actor/ActorManager.h"

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
				for (auto& child : act->GetAllChildActor())
				{
					child->PreUpdate();
				}
				act->PreUpdate();
			}
		}

		//�X�V
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

		//����X�V
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

		//�`��
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

		// �A�N�^�[��ǉ�
		void ActorManager::AddActors(std::shared_ptr<Actor> actor)
		{
			actor->Initialize();
			actors.emplace_back(actor);
		}

		// �A�N�^�[���폜
		void ActorManager::RemoveActor(std::shared_ptr<Actor> actor)
		{
			actor->Finalize();
			actor.reset();
		}

		//����ID���`�F�b�N���C������
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