#include "pch.h"
#include "FrameWork/Actor/Actor.h"
#include "FrameWork/Component/Component.h"


namespace Bread
{
	namespace FrameWork
	{
		// ����
		std::shared_ptr<Actor> Actor::Create()
		{
			return std::make_shared<Actor>();
		}

		// ������
		void Actor::Initialize()
		{
			for (auto child : children)
			{
				child->Initialize();
			}

			for (auto component : components)
			{
				component->Initialize();
			}
		}

		// �I����
		void Actor::Finalize()
		{
			for (auto child : children)
			{
				child->Finalize();
			}

			for (auto component : components)
			{
				component->Finalize();
			}
		}

		//���O�X�V
		void Actor::PreUpdate(const f32& dt)
		{
			for (auto child : children)
			{
				child->PreUpdate(dt);
			}

			RenameSameID();

			for (auto component : components)
			{
				component->PreUpdate(dt);
			}
		}

		// �X�V
		void Actor::Update(const f32& dt)
		{
			for (auto child : children)
			{
				child->Update(dt);
			}

			for (auto component : components)
			{
				component->Update(dt);
			}
		}

		//����X�V
		void Actor::nextUpdate(const f32& dt)
		{
			for (auto child : children)
			{
				child->nextUpdate(dt);
			}

			RenameSameID();

			for (auto component : components)
			{
				component->NextUpdate(dt);
			}
		}

		// �`��
		void Actor::Draw(const f32& dt)
		{
			for (auto child : children)
			{
				child->Draw(dt);
			}

			for (auto component : components)
			{
				component->Draw(dt);
			}
		}

		// �e�A�N�^�[�̐ݒ�
		void Actor::SetParentActor(std::shared_ptr<Actor> actor)
		{
			parent = actor;
		}

		// �A�N�^�[��ǉ�
		void Actor::AddChildActors(std::shared_ptr<Actor> actor)
		{
			actor->SetParentActor(shared_from_this());
			actor->Initialize();

			children.emplace_back(actor);
		}

		// �A�N�^�[���폜
		void Actor::RemoveChildActor(std::shared_ptr<Actor> actor)
		{
			actor->SetParentActor(nullptr);
			actor->Finalize();

			auto&& it = std::find(children.begin(), children.end(), actor);
			if (it != children.end())
			{
				children.erase(it);
			}
		}

		// �R���|�[�l���g��ۑ�
		void Actor::SaveComponent(std::shared_ptr<Component> component)
		{
			component->SetOwner(shared_from_this());
			components.emplace_back(component);
		}

		// �R���|�[�l���g���폜
		void Actor::RemoveComponent(std::shared_ptr<Component> component)
		{
			component->SetOwner(nullptr);
			component->Destruct();

			auto&& it = std::find(components.begin(), components.end(), component);
			if (it != components.end())
			{
				components.erase(it);
			}
		}

		//ID�̐ݒ�
		void Actor::SetID(const std::string& id)
		{
			ID = id;
		}

		//ID�̎擾
		const std::string& Actor::GetID()
		{
			return ID;
		}

		//����ID���`�F�b�N���C������
		void Actor::RenameSameID()
		{
			for (auto& component1 : components)
			{
				std::string compareID       = component1->GetID();
				int sameComponentNum = 0;
				for (auto& component2 : components)
				{
					std::string ID = component2->GetID();
					if (component1 != component2)
					{
						if (compareID == ID)
						{
							component2->SetID(ID + std::to_string(++sameComponentNum));
						}
					}
				}
			}
		}
	}
}