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
		void __fastcall Actor::PreUpdate()
		{
			for (auto child : children)
			{
				child->PreUpdate();
			}

			RenameSameID();

			for (auto component : components)
			{
				component->PreUpdate();
			}
		}

		// �X�V
		void __fastcall Actor::Update()
		{
			for (auto child : children)
			{
				child->Update();
			}

			for (auto component : components)
			{
				component->Update();
			}
		}

		//����X�V
		void __fastcall Actor::NextUpdate()
		{
			for (auto child : children)
			{
				child->NextUpdate();
			}

			RenameSameID();

			for (auto component : components)
			{
				component->NextUpdate();
			}
		}

		// �`��
		void __fastcall Actor::Draw()
		{
			for (auto child : children)
			{
				child->Draw();
			}

			for (auto component : components)
			{
				component->Draw();
			}
		}

		// �e�A�N�^�[�̐ݒ�
		void __fastcall Actor::SetParentActor(std::shared_ptr<Actor> actor)
		{
			parent = actor;
		}

		// �A�N�^�[��ǉ�
		void __fastcall Actor::AddChildActors(std::shared_ptr<Actor> actor)
		{
			actor->SetParentActor(shared_from_this());
			actor->Initialize();

			children.emplace_back(actor);
		}

		// �A�N�^�[���폜
		void __fastcall Actor::RemoveChildActor(std::shared_ptr<Actor> actor)
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
		void __fastcall Actor::SaveComponent(std::shared_ptr<Component> component)
		{
			component->SetOwner(shared_from_this());
			components.emplace_back(component);
		}

		// �R���|�[�l���g���폜
		void __fastcall Actor::RemoveComponent(std::shared_ptr<Component> component)
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
		void __fastcall Actor::SetID(const std::string& id)
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