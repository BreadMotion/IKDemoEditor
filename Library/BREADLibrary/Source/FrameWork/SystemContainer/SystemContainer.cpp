#include "pch.h"
#include "FrameWork/SystemContainer/SystemContainer.h"


namespace Bread
{
	namespace FrameWork
	{
		// 生成
		std::unique_ptr<SystemContainer> SystemContainer::Create()
		{
			return std::make_unique<SystemContainer>();
		}

		// コンストラクタ
		void SystemContainer::Construct()
		{
			for (auto system : systems)
			{
				system->Construct();
			}
		}

		// 初期化
		void SystemContainer::Initialize()
		{
			for (auto system : systems)
			{
				system->Initialize();
			}
		}

		// 終了化
		void SystemContainer::Finalize()
		{
			for (auto system : systems)
			{
				system->Finalize();
			}
		}

		// 更新
		void SystemContainer::Update()
		{
			for (auto system : systems)
			{
				system->Update();
			}
		}

		// 描画
		void SystemContainer::Draw()
		{
			for (auto system : systems)
			{
				system->Draw();
			}
		}
	}
}