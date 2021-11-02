#include "Graphics/RenderManager.h"

#include "StageCollisionComponent.h"
#include "Types.h"

#include "FND/Instance.h"

#include "FrameWork/Actor/ActorManager.h"
#include "FrameWork/Object/TerrainManager.h"

#include "Math/BreadMath.h"

#define USE_TESTSTAGE
//#define USE_MOUNTAIN
//#define USE_EXTRASTAGE

using Bread::FND::MapInstance;

using namespace Bread::Math;

namespace Bread
{
	namespace FrameWork
	{
		void StageCollisionComponent::Initialize()
		{
			std::shared_ptr<Actor> owner{ GetOwner() };
			model = owner->AddComponent<ModelObject>();
			ComponentConstruction();

			//ModelResourceのFaceが構築できるのを待機する
			auto BuildFace = [](std::shared_ptr<ModelObject> model) {
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
			};
			BuildFace(model);

			MapInstance<TerrainManager>::instance["CollisionModelManager"].FirstRegisterPolygon(GetOwner(), model);
		}

		void StageCollisionComponent::PreUpdate()
		{
		}

		void StageCollisionComponent::Update()
		{
			//モデル自身のTransform etc...を更新する
			model->UpdateLocalTransform();
			model->UpdateWorldTransform();
			model->UpdateBoneTransform();
		}

		void StageCollisionComponent::NextUpdate()
		{
		}

		void StageCollisionComponent::Draw()
		{

		}

		void StageCollisionComponent::ComponentConstruction()
		{
			ModelObjectConstruction();
			TransformConstruction();
		}

		void StageCollisionComponent::ModelObjectConstruction()
		{
			model->SetID("stageCollisionModel");
			//model->GetShaderMethod().SetShaderNema(Graphics::ShaderNameVal::basicShader);

#ifdef  USE_TESTSTAGE
			model->Load("..\\Data\\Assets\\Model\\Stage\\MapCol.fbx");
#endif //  USE_TESTSTAGE

#ifdef USE_MOUNTAIN
			model->Load("..\\Data\\Assets\\Model\\SUNLITStage\\uploads_files_820010_Mountain.fbx");
#endif //USE_MOUNTAIN

#ifdef USE_EXTRASTAGE
			model->Load("..\\Data\\Assets\\Model\\ExampleStage\\ExampleStage.fbx");
#endif // USE_EXTRASTAGE
		}

		void StageCollisionComponent::TransformConstruction()
		{
			transform = GetOwner()->GetComponent<Transform>();
			transform->SetID("stageCollisionTransform");

#ifdef USE_MOUNTAIN
			{
				//uploads_files_820010_Mountain.fbx　をLoadしている場合、このリソースは左手座標系のため初期値では縦に表示されてしまう
				//加えてスケールも小さいため調整
				Vector3    euler{ ToRadian(-90.0f),0.0f,0.0f };
				Quaternion q{ ConvertToQuaternionFromRollPitchYaw(euler.x, euler.y, euler.z) };
				transform->SetRotate(q);
				transform->SetScale({ 5.0f,5.0f ,5.0f });
			}
#endif //USE_MOUNTAIN

#ifdef  USE_TESTSTAGE
			transform->SetScale({ 1.5f,1.5f ,1.5f });
#endif //  USE_TESTSTAGE

#ifdef USE_EXTRASTAGE
			transform->SetScale({ 100.0f, 100.0f, 100.0f });
#endif // USE_EXTRASTAGE
		}
	}
}