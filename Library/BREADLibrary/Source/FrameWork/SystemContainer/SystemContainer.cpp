#include "pch.h"
#include "FrameWork/SystemContainer/SystemContainer.h"


namespace Bread
{
	namespace FrameWork
	{
		// ����
		std::unique_ptr<SystemContainer> SystemContainer::Create()
		{
			return std::make_unique<SystemContainer>();
		}

		// �R���X�g���N�^
		void SystemContainer::Construct()
		{
			for (auto system : systems)
			{
				system->Construct();
			}
		}

		// ������
		void SystemContainer::Initialize()
		{
			for (auto system : systems)
			{
				system->Initialize();
			}
		}

		// �I����
		void SystemContainer::Finalize()
		{
			for (auto system : systems)
			{
				system->Finalize();
			}
		}

		// �X�V
		void SystemContainer::Update()
		{
			for (auto system : systems)
			{
				system->Update();
			}
		}

		// �`��
		void SystemContainer::Draw()
		{
			for (auto system : systems)
			{
				system->Draw();
			}
		}
	}
}