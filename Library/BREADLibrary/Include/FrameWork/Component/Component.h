#pragma once
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
		class Component : public std::enable_shared_from_this<Component>
		{
		private:
			std::weak_ptr<Actor> owner;
			std::string          ID;

		public:
			explicit Component() {}
			virtual ~Component() {}

		public:
			// �R���X�g���N�^
			virtual void Construct() {}

			// �f�X�g���N�^
			virtual void Destruct() {}

			// ������
			virtual void Initialize() {}

			// �I����
			virtual void Finalize() {}

			//���O�X�V
			virtual void __fastcall PreUpdate() {}

			// �X�V
			virtual void __fastcall Update() {}

			//����X�V
			virtual void __fastcall NextUpdate() {}

			// �`��
			virtual void __fastcall Draw() {}

			//imgui
			virtual void GUI() {}

			void Destroy()
			{
				delete this;
			}

		public:
			// �I�[�i�[�ݒ�
			void __fastcall SetOwner(std::shared_ptr<Actor> actor) { owner = actor; }

			//ID��ݒ�
			void __fastcall SetID(const std::string& id) { ID = id; }

			//ID���擾����
			const std::string& __fastcall GetID() { return ID; }

			// �I�[�i�[�擾
			std::shared_ptr<Actor> GetOwner() { return owner.lock(); }

//		public://Gui watch window �p
//			//matrix
//			void __fastcall RegisterWatchVal(std::string id, Bread::Math::Matrix* val)
//			{
//				using namespace ImGui;
//
//				if (BeginPopupContextItem(id.c_str()))
//				{
//					std::string buttonString = "Regist Watch : " + id;
//					if (Button(buttonString.c_str()))
//					{
//						if (std::shared_ptr<Actor> act= owner.lock())
//						{
//							act->mAry.insert(std::make_pair((id + "-Matrix").c_str(), val));
//						}
//					}
//					EndPopup();
//				}
//			}
//
//			//Quaternion
//			void __fastcall RegisterWatchVal(std::string id, Bread::Math::Quaternion* val)
//			{
//				using namespace ImGui;
//
//;				if (BeginPopupContextItem(id.c_str()))
//				{
//					std::string buttonString = "Regist Watch : "+ id;
//					if (Button(buttonString.c_str()))
//					{
//						if (std::shared_ptr<Actor> act = owner.lock())
//						{
//							act->qAry.insert(std::make_pair((id + "-Quaternion").c_str(), val));
//						}
//					}
//					EndPopup();
//				}
//			}
//
//			//Color
//			void __fastcall RegisterWatchVal(std::string id, Bread::Math::Color* val)
//			{
//				using namespace ImGui;
//				if (BeginPopupContextItem(id.c_str()))
//				{
//					std::string buttonString = "Regist Watch : " + id;
//					if (Button(buttonString.c_str()))
//					{
//						if (std::shared_ptr<Actor> act = owner.lock())
//						{
//							act->cAry.insert(std::make_pair((id + "-Color").c_str(), val));
//						}
//					}
//					EndPopup();
//				}
//			}
//
//			//Vector2
//			void __fastcall RegisterWatchVal(std::string id, Bread::Math::Vector2* val)
//			{
//				using namespace ImGui;
//				static int iterate = 0;
//				if (BeginPopupContextItem(id.c_str()))
//				{
//					std::string buttonString = "Regist Watch : "+ id;
//					if (Button(buttonString.c_str()))
//					{
//						if (std::shared_ptr<Actor> act = owner.lock())
//						{
//							act->v2Ary.insert(std::make_pair((id + "-Vector2").c_str(), val));
//						}
//					}
//					EndPopup();
//				}
//			}
//
//			//Vector3
//			void __fastcall RegisterWatchVal(std::string id, Bread::Math::Vector3* val)
//			{
//				using namespace ImGui;
//				if (BeginPopupContextItem(id.c_str()))
//				{
//					std::string buttonString = "Regist Watch : "+ id;
//					if (Button(buttonString.c_str()))
//					{
//						if (std::shared_ptr<Actor> act = owner.lock())
//						{
//							act->v3Ary.insert(std::make_pair(( id + "-Vector3").c_str(), val));
//						}
//					}
//					EndPopup();
//				}
//			}
//
//			//Vector4
//			void __fastcall RegisterWatchVal(std::string id, Bread::Math::Vector4* val)
//			{
//				using namespace ImGui;
//				if (BeginPopupContextItem(id.c_str()))
//				{
//					std::string buttonString = "Regist Watch : "+ id;
//					if (Button(buttonString.c_str()))
//					{
//						if (std::shared_ptr<Actor> act = owner.lock())
//						{
//							act->v4Ary.insert(std::make_pair((id + "-Vector4").c_str(), val));
//						}
//					}
//					EndPopup();
//				}
//			}

		};
	}
}