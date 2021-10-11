#include "IKTargetCom.h"
#include "Math/BreadMath.h"
#include "../Source/Graphics/Device/Win/DirectX11/DeviceDX11.h"
#include "../Source/Graphics/Context/Win/DirectX11/ContextDX11.h"

#include "FND/Instance.h"

#include "../Source/Graphics/GraphicsDevice/Win/DirectX11/GraphicsDeviceDX11.h"

using Bread::FND::Instance;
using Bread::FND::SharedInstance;

namespace Bread
{
	namespace FrameWork
	{
		std::shared_ptr<Actor> IKTargetActor::Create(Graphics::Camera* cam)
		{
			return std::make_shared<IKTargetActor>(cam);
		}

		void IKTargetActor::Initialize()
		{
			using namespace Math;

			Graphics::DeviceDX11* dxDevice = dynamic_cast<Graphics::DeviceDX11*> (SharedInstance<Graphics::GraphicsDeviceDX11>::instance->GetDevice());
			ID3D11Device* device = dxDevice->GetD3DDevice();
			std::shared_ptr<RayCastCom> wpRayCast;
			if (std::shared_ptr<ModelObject> wpTerrain = terrain.lock())
			{
				rayCast = wpRayCast = AddComponent<RayCastCom>(wpTerrain.get());

			}
			std::shared_ptr<Transform> wpTransform;
			transform = wpTransform = AddComponent<Transform>();
			primitive = AddComponent<GeometricPrimitive>(device, GeometricPrimitive::GeometricPrimitiveType::SPHERE);

			//transform�̏�����
			if(wpTransform)
			{
				wpTransform->Initialize();

				std::shared_ptr<Actor> parentAct = parent.lock();
				if (!parentAct) return;

				Vector3 newTargetPos = GetLocation(parentAct->GetComponent<Transform>()->GetWorldTransform());
				newTargetPos.y -= 0.1f;
				newTargetPos.x -= 0.1f;
				wpTransform->SetTranslate(newTargetPos);
			}

			//rayCastCom�̏�����
			if(wpRayCast)
			{
				wpRayCast->Initialize();
				wpRayCast->SetTargetFaceIndex(*targetFaceIndex);
			}
		}

		void IKTargetActor::PreUpdate()
		{
			for (auto& childAct : GetAllChildActor())
			{
				childAct->PreUpdate();
			}
		}

		void IKTargetActor::Update()
		{
			using namespace Bread::Math;

			for (auto& childAct : GetAllChildActor())
			{
				childAct->Update();
			}
		}

		void IKTargetActor::NextUpdate()
		{
			using namespace Bread::Math;
			for (auto& childAct : GetAllChildActor())
			{
				childAct->NextUpdate();
			}

			std::shared_ptr<Transform>  wpTransform = transform.lock();
			std::shared_ptr<RayCastCom> wpRayCast   = rayCast.lock();
			if (!wpTransform && !wpRayCast && !objMatrix) return;


			Matrix worldTransform = objMatrix;
			wpTransform->SetTranslate(GetLocation(worldTransform));
			wpTransform->SetRotate(GetRotation(worldTransform));
			wpTransform->SetScale(GetScale(worldTransform));
			wpTransform->Update();

			//���C�L���X�g vsStage
			if (wpRayCast->GetUseFlag())
			{
				wpRayCast->SetStartPosition(rayStart);
				wpRayCast->SetEndPosition(rayEnd);
				wpRayCast->SetDistance(length);
				wpRayCast->IntersectRayVsModel();      //���C�L���X�g����

				if (wpRayCast->GetHItFlag())
				{
					wpTransform->SetTranslate(wpRayCast->hitResult.position);
					wpTransform->Update();
				}
			}
		}

		void IKTargetActor::Draw()
		{
			using namespace Math;

			for (auto& childAct : GetAllChildActor())
			{
				childAct->Draw();
			}
			std::shared_ptr<Transform> wpTransform = transform.lock();
			if (!wpTransform)return;

			Vector3 scale       = wpTransform->GetScale();
			Vector3 translate   = wpTransform->GetTranslate();
			Vector3 euler = ConvertToRollPitchYawFromQuaternion(wpTransform->GetRotate());

			// ���[���h�s����쐬
			Matrix W;
			{
				Matrix S, R, T;
				S = MatrixScaling(scale.x, scale.y, scale.z);
				R = MatrixRotationRollPitchYaw(euler.x, euler.y, euler.z);
				T = MatrixTranslation(translate.x, translate.y, translate.z);

				W = S * R * T;
			}

			Graphics::DeviceDX11* device = static_cast<Graphics::DeviceDX11*>(SharedInstance<Graphics::GraphicsDeviceDX11>::instance->GetDevice());

			//primitive->Render
			//(
			//	device->GetD3DContext(),
			//	ConvertToFloat4x4FromVector4x4(W * cameraAct->GetView() * cameraAct->GetProjection()),
			//	ConvertToFloat4x4FromVector4x4(W),
			//	DirectX::XMFLOAT4(1, 1, 1, 1),
			//	DirectX::XMFLOAT4(0.0f, 0.6f, 0.0f, 1.0f),
			//	false
			//);
		}
	}
}