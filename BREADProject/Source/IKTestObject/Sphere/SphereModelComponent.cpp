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

			//ModelResource‚ÌFace‚ª\’z‚Å‚«‚é‚Ì‚ğ‘Ò‹@‚·‚é
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
			model->Load("..\\Data\\Assets\\Model\\Earth\\Earth.fbx");
		}

		void SphereModelComponent::TransformConstruction()
		{
			transform = GetOwner()->GetComponent<Transform>();
			transform->SetID("sphereTransform");
			transform->SetScale(Vector3::OneAll);
			transform->SetTranslate(Vector3::Zero);
			transform->SetRotate(Quaternion::Zero);
		}

		void SphereModelComponent::GUI()
		{
			std::string guiName = "SphereModelComponent : " + GetID();
			if (ImGui::CollapsingHeader(u8"‹…‘Ì", ImGuiTreeNodeFlags_NavLeftJumpsBackHere | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Bullet))
			{
			}
		}
	}
}