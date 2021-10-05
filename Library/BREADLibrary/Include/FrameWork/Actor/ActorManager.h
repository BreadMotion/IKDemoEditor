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
			//初期化
			void Initialize();

			//事前更新
			void PreUpdate();

			//更新
			void Update();

			//事後更新
			void NextUpdate();

			//描画
			void Draw();

		public://IThreadWorker
			void Execute() override;
			void Exit() override;
		};
	}//namespace FrameWork
}//namespace Bread