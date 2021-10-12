#include "Graphics/GraphicsDevice.h"
#include "Graphics/Camera.h"
#include "Graphics/RenderManager.h"

#include "StageComponent.h"
#include "Types.h"

#include "FND/Instance.h"

#include "FrameWork/Actor/ActorManager.h"
#include "FrameWork/Object/TerrainManager.h"
#include "FrameWork/Input/InputDevice.h"

#include "Math/BreadMath.h"

using Bread::FND::Instance;

using namespace Bread::Math;

namespace Bread
{
	namespace FrameWork
	{
		void StageComponent::Initialize()
		{
			std::shared_ptr<Actor> owner{ GetOwner() };
			model = owner->AddComponent<ModelObject>();
			ComponentConstruction();

			while (1)
			{
				if (Graphics::IModelResource* resource = model->GetModelResource())
				{
					if (resource->IsReady())
					{
						model->BuildFaces();
						break;
					}
				}
			}

			Instance<TerrainManager>::instance.RegisterPolygon(GetOwner());
		}

		void StageComponent::PreUpdate()
		{

		}

		void StageComponent::Update()
		{
			model->UpdateTransform(MapInstance<f32>::instance["elapsedTime"] / 60.0f);
		}

		void StageComponent::NextUpdate()
		{

		}

		void StageComponent::Draw()
		{

		}

		void StageComponent::ComponentConstruction()
		{
			ModelObjectConstruction();
			TransformConstruction();
		}

		void StageComponent::ModelObjectConstruction()
		{
			model->SetID("stageModel");
			model->GetShaderMethod().SetShaderNema(Graphics::ShaderNameVal::basicShader);
			model->Load("..\\Data\\Assets\\Model\\Stage\\floor.fbx");
			//stageModel->Load("..\\Data\\Assets\\Model\\Stage\\MapCol.fbx");
			//stageModel->Load("..\\Data\\Assets\\Model\\SUNLITStage\\uploads_files_820010_Mountain.fbx");
		}

		void StageComponent::TransformConstruction()
		{
			transform = GetOwner()->GetComponent<Transform>();
			transform->SetID("stageTransform");
			//Vector3    euler = { ToRadian(-90.0f),0.0f,0.0f };
			//Quaternion q = ConvertToQuaternionFromRollPitchYaw(euler.x, euler.y, euler.z);
			//wpTransform->SetRotate(q);
			transform->SetScale({ 1.0f,1.0f ,1.0f });
			transform->Update();
		}
	}
}