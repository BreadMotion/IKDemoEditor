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

			MapInstance<TerrainManager>::instance["CollisionModelManager"].FirstRegisterPolygon(GetOwner(), model);
		}

		void StageCollisionComponent::PreUpdate()
		{
		}

		void StageCollisionComponent::Update()
		{
			auto t = transform;
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

			if (Graphics::IModelResource* resource = model->GetModelResource())
			{
				model->BuildFaces();
			}
		}

		void StageCollisionComponent::TransformConstruction()
		{
			transform = GetOwner()->GetComponent<Transform>();
			transform->SetID("stageCollisionTransform");
		}
	}
}