#include "IKTargetComponent.h"
#include "FrameWork/Component/SpatialIndex.h"
#include "FrameWork/Actor/ActorManager.h"
#include "FrameWork/Object/TerrainManager.h"

#include "../Source/Graphics/Device/Win/DirectX11/DeviceDX11.h"
#include "../Source/Graphics/Context/Win/DirectX11/ContextDX11.h"

#include "FND/Instance.h"

#include "../Source/Graphics/GraphicsDevice/Win/DirectX11/GraphicsDeviceDX11.h"

using Bread::FND::Instance;
using Bread::FND::SharedInstance;
using Bread::FND::MapInstance;  //TerrainManager

using namespace Bread::Math;

namespace Bread
{
	namespace FrameWork
	{
		void IKTargetComponent::Initialize()
		{
			Graphics::DeviceDX11* dxDevice{ dynamic_cast<Graphics::DeviceDX11*> (SharedInstance<Graphics::GraphicsDeviceDX11>::instance->GetDevice()) };
			ID3D11Device* device{ dxDevice->GetD3DDevice() };
			std::shared_ptr<Actor> owner{ GetOwner() };
			transform = owner->GetComponent<Transform>();
			rayCast   = owner->AddComponent<RayCastCom>();
			primitive = owner->AddComponent<GeometricPrimitive>(device, GeometricPrimitive::GeometricPrimitiveType::SPHERE);

			ComponentConstruction();
		}

		void IKTargetComponent::PreUpdate()
		{

		}

		void IKTargetComponent::Update()
		{

		}

		void IKTargetComponent::NextUpdate()
		{
			//レイキャスト vsStage
			if (rayCast->GetUseFlag())
			{
				rayCast->SetStartPosition   (rayStart);
				rayCast->SetEndPosition     (rayEnd  );
				rayCast->SetDistance        (length  );
				rayCast->IntersectRayVsModel(MapInstance<TerrainManager>::instance["TerrainModelManager"]);      //レイキャスト判定

				if (rayCast->GetHItFlag())
				{
					transform->SetTranslate(rayCast->hitResult.start);
				}
				else
				{
					transform->SetTranslate(rayCast->hitResult.start);
				}
			}
		}

		void IKTargetComponent::Draw()
		{

		}

		void IKTargetComponent::ComponentConstruction()
		{
			TransformConstruction();
			RayCastConstruction  ();
			PrimitiveConstruction();
		}

		void IKTargetComponent::TransformConstruction()
		{
			Vector3 newTargetPos{ GetLocation(GetOwner()->GetComponent<Transform>()->GetWorldTransform()) };
			newTargetPos.y -= 0.1f;
			newTargetPos.x -= 0.1f;
			transform->SetTranslate(newTargetPos);
		}

		void IKTargetComponent::RayCastConstruction()
		{
		}

		void IKTargetComponent::PrimitiveConstruction()
		{
		}
	}
}