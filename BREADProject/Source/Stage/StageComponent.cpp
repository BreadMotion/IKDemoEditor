#include "Graphics/RenderManager.h"

#include "StageComponent.h"
#include "Types.h"

#include "FND/Instance.h"
#include "StageCollisionComponent.h"

#include "FrameWork/Actor/ActorManager.h"
#include "FrameWork/Object/TerrainManager.h"

#include "Math/BreadMath.h"

#define USE_TESTSTAGE
//#define USE_MOUNTAIN
//#define USE_EXTRASTAGE
#define CHILD_COLLISION_ACTOR

using Bread::FND::MapInstance;

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

			MapInstance<TerrainManager>::instance["TerrainModelManager"].FirstRegisterPolygon(GetOwner(), model);

#ifdef CHILD_COLLISION_ACTOR
			auto childActor{ owner->AddChildActor<Actor>() };
			childActor->SetID("stageCollision");
			childActor->AddComponent<StageCollisionComponent>();
#endif // CHILD_COLLISION_ACTOR
		}

		void StageComponent::PreUpdate()
		{
		}

		void StageComponent::Update()
		{
			auto t = transform;
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

#ifdef  USE_TESTSTAGE
			model->Load("..\\Data\\Assets\\Model\\Stage\\floor.fbx");
#endif //  USE_TESTSTAGE

#ifdef USE_MOUNTAIN
			model->Load("..\\Data\\Assets\\Model\\SUNLITStage\\uploads_files_820010_Mountain.fbx");
#endif //USE_MOUNTAIN

#ifdef USE_EXTRASTAGE
			model->Load("..\\Data\\Assets\\Model\\ExampleStage\\ExampleStage.fbx");
#endif // USE_EXTRASTAGE

			if (Graphics::IModelResource* resource = model->GetModelResource())
			{
				model->BuildFaces();
			}
		}

		void StageComponent::TransformConstruction()
		{
			transform = GetOwner()->GetComponent<Transform>();
			transform->SetID("stageTransform");

#ifdef USE_MOUNTAIN
			{
				//uploads_files_820010_Mountain.fbx　をLoadしている場合、このリソースは左手座標系のため初期値では縦に表示されてしまう
				//加えてスケールも小さいため調整
				Vector3    euler{ ToRadian(-90.0f),0.0f,0.0f                                     };
				Quaternion q    { ConvertToQuaternionFromRollPitchYaw(euler.x, euler.y, euler.z) };
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