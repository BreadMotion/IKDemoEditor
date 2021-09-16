#include "IKTargetCom.h"
#include "Math/BreadMath.h"
#include "../Source/Graphics/Device/Win/DirectX11/DeviceDX11.h"
#include "../Source/Graphics/Context/Win/DirectX11/ContextDX11.h"
#include "../Player/Player.h"

namespace Bread
{
	namespace FrameWork
	{
		std::shared_ptr<Actor> IKTargetActor::Create(std::shared_ptr<Graphics::IGraphicsDevice> graphicsDevice,Graphics::Camera* cam)
		{
			return std::make_shared<IKTargetActor>(graphicsDevice, cam);
		}

		void IKTargetActor::Initialize()
		{
			using namespace Math;
			std::shared_ptr<Graphics::IGraphicsDevice> wpGraphicsDevice = graphicsDevice.lock();
			if (!wpGraphicsDevice)
			{
				return;
			}

			Graphics::DeviceDX11* dxDevice = dynamic_cast<Graphics::DeviceDX11*> (wpGraphicsDevice->GetDevice());
			ID3D11Device* device = dxDevice->GetD3DDevice();
			if (std::shared_ptr<ModelObject> wpTerrain = terrain.lock())
			{
				rayCast = AddComponent<RayCastCom>(wpGraphicsDevice, wpTerrain.get());

			}
			transform = AddComponent<Transform>();
			primitive = AddComponent<GeometricPrimitive>(device, GeometricPrimitive::GeometricPrimitiveType::SPHERE);

			//transformの初期化
			std::shared_ptr<Transform> wpTransform = transform.lock();
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

			//rayCastComの初期化
			std::shared_ptr<RayCastCom> wpRayCast = rayCast.lock();
			if(wpRayCast)
			{
				wpRayCast->Initialize();
				wpRayCast->SetTargetFaceIndex(*targetFaceIndex);
			}
		}

		void IKTargetActor::PreUpdate(const f32& dt)
		{
			for (auto& childAct : GetAllChildActor())
			{
				childAct->PreUpdate(dt);
			}
		}

		void IKTargetActor::Update(const f32& dt)
		{
			using namespace Bread::Math;

			for (auto& childAct : GetAllChildActor())
			{
				childAct->Update(dt);
			}
		}

		void IKTargetActor::NextUpdate(const f32& dt)
		{
			using namespace Bread::Math;
			for (auto& childAct : GetAllChildActor())
			{
				childAct->NextUpdate(dt);
			}

			std::shared_ptr<Transform>  wpTransform = transform.lock();
			std::shared_ptr<RayCastCom> wpRayCast   = rayCast.lock();
			if (!wpTransform && !wpRayCast && !objMatrix) return;


			Matrix worldTransform = objMatrix;
			wpTransform->SetTranslate(GetLocation(worldTransform));
			wpTransform->SetRotate(GetRotation(worldTransform));
			wpTransform->SetScale(GetScale(worldTransform));
			wpTransform->Update(dt);

			//レイキャスト vsStage
			if (wpRayCast->GetUseFlag())
			{
#if 0
				//std::shared_ptr<Actor> parentAct = parent.lock();
				//if (!parentAct) return;
				//Transform*                         parentT         = parentAct->GetComponent<Transform>();
				//CyclicCoordinateDescent* parentCCDIK = parentAct->GetComponent<CyclicCoordinateDescent>();
				//Matrix        parentM = parentT->GetWorldTransform();

				//Vector3     upVector    = GetRotation(parentM).LocalUp();
				//Vector3   rightVector   = GetRotation(parentM).LocalRight();
				//const f32 inverseVec  = -1.0f;

				//auto   nodes       = IKModel->GetNodes();
				//Matrix hipM        = nodes->at(1).worldTransform   * parentM;
				//Matrix bone        = nodes->at(56).worldTransform * parentM;
				//Matrix bone1      = nodes->at(57).worldTransform * parentM;
				//Matrix bone2      = nodes->at(58).worldTransform * parentM;

				//Vector3 boneVec         = Vector3Subtract(GetLocation(bone2), GetLocation(bone1));
				//f32        length             = Vector3Length(boneVec) + parentCCDIK->order.at(0)->ankleHeight;
				//f32        halfPelvimetry = Vector3Length(GetLocation(hipM) - GetLocation(bone));

				//Vector3 rayVec   = (upVector) * length;
				//Vector3 rayEnd  = GetLocation(parentM) + (rightVector * (halfPelvimetry));
				//Vector3 rayStart = rayEnd + rayVec;
#endif
				wpRayCast->SetStartPosition(rayStart);
				wpRayCast->SetEndPosition(rayEnd);
				wpRayCast->SetDistance(length);
				wpRayCast->IntersectRayVsModel();      //レイキャスト判定

				if (wpRayCast->GetHItFlag())
				{
					wpTransform->SetTranslate(wpRayCast->hitResult.position);
					wpTransform->Update(dt);
				}
		}
		}

		void IKTargetActor::Draw(const f32& dt)
		{
			using namespace Math;
			std::shared_ptr<Graphics::IGraphicsDevice> wpGraphicsDevice = graphicsDevice.lock();
			if (!wpGraphicsDevice)
			{
				return;
			}


			for (auto& childAct : GetAllChildActor())
			{
				childAct->Draw(dt);
			}
			std::shared_ptr<Transform> wpTransform = transform.lock();
			if (!wpTransform)return;

			Vector3 scale       = wpTransform->GetScale();
			Vector3 translate   = wpTransform->GetTranslate();
			Vector3 euler = ConvertToRollPitchYawFromQuaternion(wpTransform->GetRotate());

			// ワールド行列を作成
			Matrix W;
			{
				Matrix S, R, T;
				S = MatrixScaling(scale.x, scale.y, scale.z);
				R = MatrixRotationRollPitchYaw(euler.x, euler.y, euler.z);
				T = MatrixTranslation(translate.x, translate.y, translate.z);

				W = S * R * T;
			}

			Graphics::DeviceDX11* device = static_cast<Graphics::DeviceDX11*>(wpGraphicsDevice->GetDevice());

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