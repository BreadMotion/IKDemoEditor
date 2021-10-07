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

		public:
			// アクターを追加
			void __fastcall AddActors(std::shared_ptr<Actor> actor);

			// アクターを削除
			void __fastcall RemoveActor(std::shared_ptr<Actor> actor);

			//同じIDかチェック＆修正する
			void RenameSameID();

		public:
			//アクターの取得(ダウンキャスト)
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

			//指定したIDを持つアクターを取得する
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

			//指定したIDを持つアクターを取得する(ダウンキャスト)
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


			//全アクターの取得
			std::vector<std::shared_ptr<Actor>>& GetAllActor()
			{
				return actors;
			}

			//アクターの追加
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