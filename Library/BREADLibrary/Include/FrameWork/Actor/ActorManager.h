#pragma once
#include <map>
#include <memory>
#include <string>
#include "FND/Base.h"
#include "OS/Thread.h"
#include "FrameWork/Actor/Actor.h"

namespace Bread
{
	namespace FrameWork
	{
		class [[]] ActorManager : public OS::IThreadWorker, FND::Base
		{
		private:
			std::vector<std::shared_ptr<Actor>> actors = {};
			std::string shaderName{};

		public:
			ActorManager() = default;
			~ActorManager() {}

		public:
			//������
			void Initialize();

			//���O�X�V
			void PreUpdate();

			//�X�V
			void Update();

			//����X�V
			void NextUpdate();

			//�`��
			void Draw();

		public://IThreadWorker
			void Execute() override;
			void Exit() override;

		public:
			// �A�N�^�[��ǉ�
			void __fastcall AddActors(std::shared_ptr<Actor> actor);

			// �A�N�^�[���폜
			void __fastcall RemoveActor(std::shared_ptr<Actor> actor);

			//����ID���`�F�b�N���C������
			void RenameSameID();

		public:
			//�A�N�^�[�̎擾(�_�E���L���X�g)
			template<class T>
			std::shared_ptr<T> GetActor()
			{
				for (auto& act : actors)
				{
					std::shared_ptr<T> obj = std::dynamic_pointer_cast<T>(act);
					if (obj != nullptr)
					{
						return obj;
					}
				}
				return nullptr;
			}

			//�w�肵��ID�����A�N�^�[���擾����
			std::shared_ptr<Actor> __fastcall GetActorFromID(const std::string& name)
			{
				for (auto& act : actors)
				{
					if (act->GetID() != name) continue;

					if (act != nullptr)
					{
						return act;
					}
				}
				return nullptr;
			}

			//�w�肵��ID�����A�N�^�[���擾����(�_�E���L���X�g)
			template<class T>
			std::shared_ptr<Actor> __fastcall GetActorFromID(const std::string& name)
			{
				for (auto& act : actors)
				{
					if (act->GetID() != name) continue;

					std::shared_ptr<T> obj = std::dynamic_pointer_cast<T>(act);
					if (obj != nullptr)
					{
						return obj;
					}
				}
				return nullptr;
			}


			//�S�A�N�^�[�̎擾
			std::vector<std::shared_ptr<Actor>>& GetAllActor()
			{
				return actors;
			}

			//�A�N�^�[�̒ǉ�
			template <class T, class... Args>
			std::shared_ptr<T> __fastcall AddActor(Args&&... args)
			{
				std::shared_ptr<T> obj = std::make_shared<T>(std::forward<Args>(args)...);
				AddActors(obj);

				return obj;
			}
		};
	}//namespace FrameWork
}//namespace Bread