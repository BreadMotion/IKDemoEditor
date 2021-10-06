#pragma once
#include <memory>
#include <vector>
#include "FND/Base.h"
#include "FrameWork/Component/Component.h"

//FlyWeight�̎����\��
//�����̗D��x�����߂鏈�������K�v������

namespace Bread
{
	namespace FrameWork
	{
		enum class ComponentUpdatePriority
		{
			LOWEST = 0,
			BELOW,
			NORMAL,
			IDLE,
			ABOVE,
			HIGHEST
		};

		class [[]] ComponentManager : public FND::Base
		{
		private:
			std::vector<std::shared_ptr<Component>> components;

		public:
			ComponentManager() = default;
			~ComponentManager() {}

		public:
			// ������
			void Initialize();

			// �I����
			void Finalize();

			//���O�X�V
			void PreUpdate();

			// �X�V
			void Update();

			//����X�V
			void NextUpdate();

			// �`��
			void Draw();

		public:
			//�d�������R���|�[�l���g�̎������l�[��
			void RenameSameID();

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
			template <class T, class... Args>
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