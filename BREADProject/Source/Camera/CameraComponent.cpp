#include "CameraComponent.h"
#include "Math/BreadMath.h"

using namespace Bread::Math;

namespace Bread
{
	namespace FrameWork
	{
		void CameraComponent::Initialize()
		{
			camera = GetOwner()->AddComponent<Graphics::Camera>();

			camera->SetEye({ 0.0f ,3000.0f ,1000.0f });
			camera->SetRotateX(-0.225f);
			camera->SetRotateY(0.0f);
			camera->SetTargetPos({ 0.0f ,2600.0f ,0.0f }, Vector3::Zero);
			camera->SetLookAt({ 0.0f ,3000.0f ,1000.0f }, { 10.0f ,2600.0f ,0.0f }, Vector3{ 0.0f,1.0f,0.0f });
		}

		//事前更新
		void CameraComponent::PreUpdate()
		{
		}

		//更新
		void CameraComponent::Update()
		{
		}

		//事後更新
		void CameraComponent::NextUpdate()
		{
		}
	}
}