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

			//ModelResourceのFaceが構築できるのを待機する
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

			Instance<TerrainManager>::instance.FirstRegisterPolygon(GetOwner());
		}

		void StageComponent::PreUpdate()
		{
			transform->GetWorldTransform();
		}

		void StageComponent::Update()
		{
			//モデル自身のTransform etc...を更新する
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
			//model->Load("..\\Data\\Assets\\Model\\Stage\\MapCol.fbx");
			//model->Load("..\\Data\\Assets\\Model\\SUNLITStage\\uploads_files_820010_Mountain.fbx");
			//model->Load("..\\Data\\Assets\\Model\\ExampleStage\\ExampleStage.fbx");
		}

		void StageComponent::TransformConstruction()
		{
			transform = GetOwner()->GetComponent<Transform>();
			transform->SetID("stageTransform");
#if 0
			{
				//uploads_files_820010_Mountain.fbx　をLoadしている場合、このリソースは左手座標系のため初期値では縦に表示されてしまう
				//加えてスケールも小さいため調整
				Vector3    euler{ ToRadian(-90.0f),0.0f,0.0f };
				Quaternion q{ ConvertToQuaternionFromRollPitchYaw(euler.x, euler.y, euler.z) };
				transform->SetRotate(q);
				transform->SetScale({ 5.0f,5.0f ,5.0f });
			}
#elif 1
			transform->SetScale({ 1.5f,1.5f ,1.5f });

#else
			transform->SetScale({ 100.0f, 100.0f, 100.0f });
#endif
		}
	}
}