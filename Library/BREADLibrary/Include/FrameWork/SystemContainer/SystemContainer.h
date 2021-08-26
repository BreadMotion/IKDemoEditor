#pragma once

#include "FrameWork/System/System.h"
#include <memory>
#include <list>


namespace Bread
{
	namespace FrameWork
	{
		class SystemContainer
		{
		private:
			std::list<std::shared_ptr<ISystem>> systems;

		public:
			SystemContainer() {}
			~SystemContainer() {}

		public:
			// 生成
			static std::unique_ptr<SystemContainer> Create();

			// コンストラクタ
			void Construct();

			// 初期化
			void Initialize();

			// 終了化
			void Finalize();

			// 更新
			void Update();

			// 描画
			void Draw();

		public:
			template<class T>
			T* GetSystem()
			{
				for (auto system : systems)
				{
					T* buff = dynamic_cast<T*>(system.get());
					if (buff != nullptr) return buff;
				}
				return nullptr;
			}

			template<class T>
			T* AddSystem()
			{
				std::shared_ptr<T> buff = std::make_shared<T>();

				buff->parent = this;
				systems.push_back(buff);

				return buff.get();
			}
		};
	}
}