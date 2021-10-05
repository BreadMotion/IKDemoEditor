#include "FrameWork/Component/ComponentManager.h"

namespace Bread
{
	namespace FrameWork
	{
		// 初期化
		void ComponentManager::Initialize()
		{
			for (auto component : components)
			{
				component->Initialize();
			}
		}

		// 終了化
		void ComponentManager::Finalize()
		{

			for (auto component : components)
			{
				component->Finalize();
			}
		}

		//事前更新
		void ComponentManager::PreUpdate()
		{
			for (auto component : components)
			{
				component->PreUpdate();
			}
		}

		// 更新
		void ComponentManager::Update()
		{
			for (auto component : components)
			{
				component->Update();
			}
		}

		//事後更新
		void ComponentManager::NextUpdate()
		{
			for (auto component : components)
			{
				component->NextUpdate();
			}
		}

		// 描画
		void ComponentManager::Draw()
		{
			for (auto component : components)
			{
				component->Draw();
			}
		}

		//重複したコンポーネントの自動リネーム
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