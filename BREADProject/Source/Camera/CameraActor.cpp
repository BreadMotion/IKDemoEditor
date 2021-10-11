#include "CameraActor.h"
#include "Math/BreadMath.h"

namespace Bread
{
	namespace FrameWork
	{
		std::shared_ptr<Actor> CameraActor::Create()
		{
			return std::make_shared<CameraActor>();
		}

		void CameraActor::Initialize()
		{
			using namespace Math;
			camera = AddComponent<Graphics::Camera>();

			std::shared_ptr < Graphics::Camera > wpCamera = camera.lock();
			if (!wpCamera)return;
			{
				wpCamera->SetEye({ 0.0f ,3000.0f ,1000.0f });
				wpCamera->SetRotateX(-0.225f);
				wpCamera->SetRotateY(0.0f);
				wpCamera->SetTargetPos({ 0.0f ,2600.0f ,0.0f }, Vector3::Zero);
				wpCamera->SetLookAt({ 0.0f ,3000.0f ,1000.0f }, { 10.0f ,2600.0f ,0.0f }, Vector3{ 0.0f,1.0f,0.0f });
			}
		}

		//事前更新
		void CameraActor::PreUpdate()
		{
			for (auto& childAct : GetAllChildActor())
			{
				childAct->PreUpdate();
			}

			std::shared_ptr < Graphics::Camera > wpCamera = camera.lock();
			if (!wpCamera)return;
			{
				wpCamera->PreUpdate();
			}
		}

		//更新
		void CameraActor::Update()
		{
			for (auto& childAct : GetAllChildActor())
			{
				childAct->Update();
			}

			std::shared_ptr < Graphics::Camera > wpCamera = camera.lock();
			if (!wpCamera)return;
			{
				wpCamera->Update();
			}
		}

		//事後更新
		void CameraActor::NextUpdate()
		{
			for (auto& childAct : GetAllChildActor())
			{
				childAct->NextUpdate();
			}

			std::shared_ptr < Graphics::Camera > wpCamera = camera.lock();
			if (!wpCamera)return;
			{
				wpCamera->NextUpdate();
			}
		}
	}
}