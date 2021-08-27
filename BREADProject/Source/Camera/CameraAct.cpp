#include "CameraAct.h"
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

			camera->SetEye({ 0.0f ,3000.0f ,1000.0f });
			camera->SetRotateX(-0.225f);
			camera->SetRotateY(0.0f);
			camera->SetTargetPos({ 0.0f ,2600.0f ,0.0f }, Vector3::Zero);
			camera->SetLookAt({ 0.0f ,3000.0f ,1000.0f }, { 10.0f ,2600.0f ,0.0f }, Vector3{ 0.0f,1.0f,0.0f });
		}

		//事前更新
		void CameraActor::PreUpdate(const f32& dt)
		{
			for (auto& childAct : GetAllChildActor())
			{
				childAct->PreUpdate(dt);
			}
			camera->PreUpdate(dt);
		}

		//更新
		void CameraActor::Update(const f32& dt)
		{
			for (auto& childAct : GetAllChildActor())
			{
				childAct->Update(dt);
			}
			camera->Update(dt);
		}

		//事後更新
		void CameraActor::NextUpdate(const f32& dt)
		{
			for (auto& childAct : GetAllChildActor())
			{
				childAct->NextUpdate(dt);
			}
			camera->NextUpdate(dt);
		}
	}
}