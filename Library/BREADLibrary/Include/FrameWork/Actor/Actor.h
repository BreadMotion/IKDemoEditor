#pragma once
#include <memory>
#include <map>
#include <vector>
#include <string>
#include "math/BreadMath.h"
#include "FrameWork/Component/Component.h"

namespace Bread
{
	namespace FrameWork
	{
		class Component;
		class Actor : public std::enable_shared_from_this<Actor>
		{
		protected:
			std::weak_ptr<Actor>                    parent;
			std::vector<std::shared_ptr<Actor>>     children;
			std::vector<std::shared_ptr<Component>> components;
			std::string                             ID;

		public:
			Actor();
			virtual ~Actor() {}

		public:
			// ����
			static std::shared_ptr<Actor> Create();

			// ������
			virtual void Initialize();

			// �I����
			virtual void Finalize();

			//���O�X�V
			virtual void __fastcall PreUpdate();

			// �X�V
			virtual void __fastcall Update();

			//����X�V
			virtual void __fastcall NextUpdate();

			// �`��
			virtual void __fastcall Draw();

		public:
			// �e�A�N�^�[�̐ݒ�
			void __fastcall SetParentActor(std::shared_ptr<Actor> actor);

			// �A�N�^�[��ǉ�
			void __fastcall AddChildActors(std::shared_ptr<Actor> actor);

			// �A�N�^�[���폜
			void __fastcall RemoveChildActor(std::shared_ptr<Actor> actor);

			// �R���|�[�l���g��ǉ�
			void __fastcall SaveComponent(std::shared_ptr<Component> component);

			// �R���|�[�l���g���폜
			void __fastcall RemoveComponent(std::shared_ptr<Component> component);

			//ID�̐ݒ�
			void __fastcall SetID(const std::string& id);

			//ID�̎擾
			const std::string& GetID();

			//����ID���`�F�b�N���C������
			void RenameSameID();

		public:
			//�q�A�N�^�[�̎擾
			template<class T>
			std::shared_ptr<T> GetChildActor()
			{
				for (auto& act : children)
				{
					std::shared_ptr<T> obj = std::dynamic_pointer_cast<T>(act);
					if (obj != nullptr)
					{
						return obj;
					}
				}
				return nullptr;
			}

			//�w�肵��ID�����q�A�N�^�[���擾����
			template <class T>
			std::shared_ptr<T> __fastcall GetChildActorFromID(const std::string& name)
			{
				for (auto& act : children)
				{
					if (act->ID != name) continue;

					std::shared_ptr<T> obj = std::dynamic_pointer_cast<T>(act);
					if (obj != nullptr)
					{
						return obj;
					}
				}
				return nullptr;
			}

			//�S�q�A�N�^�[�̎擾
			std::vector<std::shared_ptr<Actor>>&GetAllChildActor()
			{
				return children;
			}

			//�q�A�N�^�[�̒ǉ�
			template <class T, class... Args>
			std::shared_ptr<T> __fastcall AddChildActor(Args&&... args)
			{
				std::shared_ptr<T> obj = std::make_shared<T>(std::forward<Args>(args)...);
				AddChildActors(obj);

				return obj;
			}

		public:
			//�R���|�[�l���g�̎擾
			template<class T>
			std::shared_ptr<T> GetComponent()
			{
				for (auto& component : components)
				{
					if (std::shared_ptr<T> obj = std::dynamic_pointer_cast<T>(component))
					{
						return obj;
					}
				}
				return nullptr;
			}

			//�w�肵��ID�����A�N�^�[���擾����
			template <class T>
			std::shared_ptr<T> __fastcall GetComponentFromID(const std::string& ID)
			{
				for (auto& component : components)
				{
					if (component->GetID() == ID)
					{
						if (std::shared_ptr<T>obj = std::dynamic_pointer_cast<T>(component))
						{
							return obj;
						}
					}
				}
				return nullptr;
			}

			//�S�R���|�[�l���g�̎擾
			std::vector<std::shared_ptr<Component>> GetAllComponent()
			{
				return components;
			}

			//�R���|�[�l���g�̒ǉ�
			template <class T,class... Args>
			std::shared_ptr<T> __fastcall AddComponent(Args&&... args)
			{
				std::shared_ptr<T> obj = std::make_shared<T>(std::forward<Args>(args)...);
				if (obj)
				{
					SaveComponent(obj);
					return obj;
				}
				return  nullptr;
			}
		};
	}//namespace FrameWork
}//namespace Bread