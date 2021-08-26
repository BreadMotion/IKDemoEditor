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
			// ����
			static std::unique_ptr<SystemContainer> Create();

			// �R���X�g���N�^
			void Construct();

			// ������
			void Initialize();

			// �I����
			void Finalize();

			// �X�V
			void Update();

			// �`��
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