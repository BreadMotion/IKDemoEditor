#pragma once
#include <typeinfo>
#include <memory>
#include <string>
#include "math/BreadMath.h"
#include "FrameWork/Actor/Actor.h"
#include "./../../ExternalLibrary/ImGui/Include/imgui.h"

namespace Bread
{
	namespace FrameWork
	{
		class Actor;
		class Component
		{
		public:
			std::weak_ptr<Actor> owner;
			std::string          ID;

		public:
			explicit Component() {}
			virtual ~Component() {}

		public:
			// コンストラクタ
			virtual void Construct() {}

			// デストラクタ
			virtual void Destruct() {}

			// 初期化
			virtual void Initialize() {}

			// 終了化
			virtual void Finalize() {}

			//事前更新
			virtual void PreUpdate(const f32&) {}

			// 更新
			virtual void Update(const f32&) {}

			//事後更新
			virtual void NextUpdate(const f32&) {}

			// 描画
			virtual void Draw(const f32&) {}

			//imgui
			virtual void GUI() {}

		public:
			// オーナー設定
			void SetOwner(std::shared_ptr<Actor> actor) { owner = actor; }

			//IDを設定
			void SetID(const std::string& id) { ID = id; }

			//IDを取得する
			const std::string& GetID() { return ID; }

			// オーナー取得
			std::shared_ptr<Actor> GetOwner() { return owner.lock(); }

		public://Gui watch window 用
			//matrix
			void RegisterWatchVal(std::string id, Bread::Math::Matrix* val)
			{
				using namespace ImGui;

				if (BeginPopupContextItem(id.c_str()))
				{
					std::string buttonString = "Regist Watch : " + id;
					if (Button(buttonString.c_str()))
					{
						if (std::shared_ptr<Actor> act= owner.lock())
						{
							act->mAry.insert(std::make_pair((id + "-Matrix").c_str(), val));
						}
					}
					EndPopup();
				}
			}

			//Quaternion
			void RegisterWatchVal(std::string id, Bread::Math::Quaternion* val)
			{
				using namespace ImGui;

;				if (BeginPopupContextItem(id.c_str()))
				{
					std::string buttonString = "Regist Watch : "+ id;
					if (Button(buttonString.c_str()))
					{
						if (std::shared_ptr<Actor> act = owner.lock())
						{
							act->qAry.insert(std::make_pair((id + "-Quaternion").c_str(), val));
						}
					}
					EndPopup();
				}
			}

			//Color
			void RegisterWatchVal(std::string id, Bread::Math::Color* val)
			{
				using namespace ImGui;
				if (BeginPopupContextItem(id.c_str()))
				{
					std::string buttonString = "Regist Watch : " + id;
					if (Button(buttonString.c_str()))
					{
						if (std::shared_ptr<Actor> act = owner.lock())
						{
							act->cAry.insert(std::make_pair((id + "-Color").c_str(), val));
						}
					}
					EndPopup();
				}
			}

			//Vector2
			void RegisterWatchVal(std::string id, Bread::Math::Vector2* val)
			{
				using namespace ImGui;
				static int iterate = 0;
				if (BeginPopupContextItem(id.c_str()))
				{
					std::string buttonString = "Regist Watch : "+ id;
					if (Button(buttonString.c_str()))
					{
						if (std::shared_ptr<Actor> act = owner.lock())
						{
							act->v2Ary.insert(std::make_pair((id + "-Vector2").c_str(), val));
						}
					}
					EndPopup();
				}
			}

			//Vector3
			void RegisterWatchVal(std::string id, Bread::Math::Vector3* val)
			{
				using namespace ImGui;
				if (BeginPopupContextItem(id.c_str()))
				{
					std::string buttonString = "Regist Watch : "+ id;
					if (Button(buttonString.c_str()))
					{
						if (std::shared_ptr<Actor> act = owner.lock())
						{
							act->v3Ary.insert(std::make_pair(( id + "-Vector3").c_str(), val));
						}
					}
					EndPopup();
				}
			}

			//Vector4
			void RegisterWatchVal(std::string id, Bread::Math::Vector4* val)
			{
				using namespace ImGui;
				if (BeginPopupContextItem(id.c_str()))
				{
					std::string buttonString = "Regist Watch : "+ id;
					if (Button(buttonString.c_str()))
					{
						if (std::shared_ptr<Actor> act = owner.lock())
						{
							act->v4Ary.insert(std::make_pair((id + "-Vector4").c_str(), val));
						}
					}
					EndPopup();
				}
			}

		};
	}
}