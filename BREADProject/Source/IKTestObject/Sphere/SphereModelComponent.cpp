#include "Types.h"
#include "FND/Instance.h"

#include "Graphics/RenderManager.h"

#include "SphereModelComponent.h"

#include "FrameWork/Actor/ActorManager.h"
#include "Math/BreadMath.h"

using Bread::FND::MapInstance;

using namespace Bread::Math;

namespace Bread
{
	namespace FrameWork
	{
		void SphereModelComponent::Initialize()
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
		}

		void SphereModelComponent::PreUpdate()
		{
		}

		void SphereModelComponent::Update()
		{
			//モデル自身のTransform etc...を更新する
			model->UpdateLocalTransform();
			model->UpdateWorldTransform();
			model->UpdateBoneTransform();
		}

		void SphereModelComponent::NextUpdate()
		{
		}

		void SphereModelComponent::Draw()
		{

		}

		void SphereModelComponent::ComponentConstruction()
		{
			ModelObjectConstruction();
			TransformConstruction();
		}

		void SphereModelComponent::ModelObjectConstruction()
		{
			model->SetID("sphereObject");
			model->GetShaderMethod().SetShaderNema(Graphics::ShaderNameVal::basicShader);
			model->Load("..\\Data\\Assets\\Model\\SkyMap\\sphere.fbx");
		}

		void SphereModelComponent::TransformConstruction()
		{
			transform = GetOwner()->GetComponent<Transform>();
			transform->SetID("stageTransform");
			transform->SetScale(Vector3{ 0.001f,0.001f ,0.001f });
		}
	}
}