#include "FrameWork/Component/ComponentManager.h"

namespace Bread
{
	namespace FrameWork
	{
		// ������
		void ComponentManager::Initialize()
		{
			for (auto component : components)
			{
				component->Initialize();
			}
		}

		// �I����
		void ComponentManager::Finalize()
		{

			for (auto component : components)
			{
				component->Finalize();
			}
		}

		//���O�X�V
		void ComponentManager::PreUpdate()
		{
			for (auto component : components)
			{
				component->PreUpdate();
			}
		}

		// �X�V
		void ComponentManager::Update()
		{
			for (auto component : components)
			{
				component->Update();
			}
		}

		//����X�V
		void ComponentManager::NextUpdate()
		{
			for (auto component : components)
			{
				component->NextUpdate();
			}
		}

		// �`��
		void ComponentManager::Draw()
		{
			for (auto component : components)
			{
				component->Draw();
			}
		}

		//�d�������R���|�[�l���g�̎������l�[��
		void ComponentManager::RenameSameID()
		{
			for (auto& component1 : components)
			{
				std::string compareID = component1->GetID();
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
	}//namespace FrameWork
}//namespace Bread