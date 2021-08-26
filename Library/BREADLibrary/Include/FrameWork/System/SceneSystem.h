#pragma once

#include <map>
#include "FrameWork/System/System.h"
#include "FrameWork/Actor/Actor.h"


namespace Bread
{
	namespace FrameWork
	{
		class IScene : public Actor
		{
		protected:
			using Super = Actor;

		public:
			IScene() {}
			virtual ~IScene() {}

		public:
			// ������
			void Initialize() override;

			// �I����
			void Finalize() override;
		};

		class SceneSystem : public ISystem
		{
		private:
			std::vector<std::shared_ptr<IScene>> scenes;
			std::shared_ptr<IScene>              currentScene;
			std::shared_ptr<IScene>              nextScene;

		public:
			SceneSystem() {}
			~SceneSystem() override {}

		public:
			// ����
			static std::shared_ptr<SceneSystem> Create()
			{
				return std::make_shared<SceneSystem>();
			}

			// �R���X�g���N�^
			void Construct()  override;

			// �f�X�g���N�^
			void Destruct()   override;

			// ������
			void Initialize() override;

			// �I����
			void Finalize()   override;

			// �X�V
			void Update()     override;

			// �`��
			void Draw()       override;

		public:
			// �V�[�����폜
			void RemoveScene(std::shared_ptr<IScene> scene);

		public:
			// �V�[��������
			template<class T>
			std::shared_ptr<T> GetScene()
			{
				for (auto scene : scenes)
				{
					std::shared_ptr<T> scene = std::dynamic_pointer_cast<std::shared_ptr<T>>(scene);
					if (scene != nullptr)
					{
						return scene;
					}
				}
				return nullptr;
			}

			// �V�[����ǉ�
			template<class T>
			std::shared_ptr<T> AddScene()
			{
				std::shared_ptr<T> scene = std::make_shared<T>();
				
				scene->Construct();
				scenes.emplace_back(scene);

				return scene;
			}

			//	�V�[���̐؂�ւ�
			template<class T>
			void ChangeScene()
			{
				std::shared_ptr<T> scene = GetScene<T>();
				if (scene != nullptr)
				{
					nextScene = scene;
				}
			}
		};
	}
}