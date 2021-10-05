#pragma once
#include "OS/Thread.h"
#include "FrameWork/Actor/Actor.h"

namespace Bread
{
	namespace FrameWork
	{
		class [[nodiscard]] ActorManager : public OS::IThreadWorker
		{
		private:
			std::map<std::string, std::shared_ptr<Actor>> actors = {};

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
		};
	}//namespace FrameWork
}//namespace Bread