#pragma once
#include <memory>
#include <map>
#include <vector>
#include <string>
#include "math/BreadMath.h"

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
			Actor() {}
			virtual ~Actor() {}

		public:
			// ����
			static std::shared_ptr<Actor> Create();

			// ������
			virtual void Initialize();

			// �I����
			virtual void Finalize();

			//���O�X�V
			virtual void PreUpdate(const f32& dt);

			// �X�V
			virtual void Update(const f32& dt);

			//����X�V
			virtual void nextUpdate(const f32& dt);

			// �`��
			virtual void Draw(const f32& dt);

		public:
			// �e�A�N�^�[�̐ݒ�
			void SetParentActor(std::shared_ptr<Actor> actor);

			// �A�N�^�[��ǉ�
			void AddChildActors(std::shared_ptr<Actor> actor);

			// �A�N�^�[���폜
			void RemoveChildActor(std::shared_ptr<Actor> actor);

			// �R���|�[�l���g��ǉ�
			void SaveComponent(std::shared_ptr<Component> component);

			// �R���|�[�l���g���폜
			void RemoveComponent(std::shared_ptr<Component> component);

			//ID�̐ݒ�
			void SetID(const std::string& id);

			//ID�̎擾
			const std::string& GetID();

			//����ID���`�F�b�N���C������
			void RenameSameID();

		public:
			//�q�A�N�^�[�̎擾
			template<class T>
			T* GetChildActor()
			{
				for (auto& act : children)
				{
					std::shared_ptr<T> obj = std::dynamic_pointer_cast<T>(act);
					if (obj != nullptr)
					{
						return obj.get();
					}
				}
				return nullptr;
			}

			template <class T>
			T* GetChildActorFromID(const std::string& name)
			{
				for (auto& act : children)
				{
					if (act->ID != name) continue;

					std::shared_ptr<T> obj = std::dynamic_pointer_cast<T>(act);
					if (obj != nullptr)
					{
						return obj.get();
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
			T* AddChildActor(Args&&... args)
			{
				std::shared_ptr<T> obj = std::make_shared<T>(std::forward<Args>(args)...);
				AddChildActors(obj);

				return obj.get();
			}

		public:
			//�R���|�[�l���g�̎擾
			template<class T>
			T* GetComponent()
			{
				for (auto& component : components)
				{
					if (std::shared_ptr<T> obj = std::dynamic_pointer_cast<T>(component))
					{
						return obj.get();
					}
				}
				return nullptr;
			}

			template <class T>
			T* GetComponentFromID(const std::string& ID)
			{
				for (auto& component : components)
				{
					if (component->GetID() == ID)
					{
						if (std::shared_ptr<T>obj = std::dynamic_pointer_cast<T>(component))
						{
							return obj.get();
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
			T* AddComponent(Args&&... args)
			{
				std::shared_ptr<T> obj = std::make_shared<T>(std::forward<Args>(args)...);
				SaveComponent(obj);

				return obj.get();
			}

		public://GUI�p
			std::map<std::string, Math::Matrix*>         mAry;
			std::map<std::string, Math::Quaternion*> qAry;
			std::map<std::string, Math::Color*>           cAry;
			std::map<std::string, Math::Vector2*>        v2Ary;
			std::map<std::string, Math::Vector3*>        v3Ary;
			std::map<std::string, Math::Vector4*>        v4Ary;
		};
	}
}