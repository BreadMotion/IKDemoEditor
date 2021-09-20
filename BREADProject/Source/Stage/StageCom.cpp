#include "StageCom.h"
#include "Math/BreadMath.h"

namespace Bread
{
	namespace FrameWork
	{
		std::shared_ptr<Actor> StageActor::Create(std::shared_ptr<Graphics::IGraphicsDevice> graphicsDevice)
		{
			return std::make_shared<StageActor>(graphicsDevice);
		}

		void StageActor::Initialize()
		{
			using namespace Bread;
			using namespace Bread::FrameWork;
			using namespace Bread::Math;

			std::shared_ptr<ModelObject> wpStageModel;
			std::shared_ptr<Transform>   wpTransform;
			const float  firstElapsed = 100.0f;

			std::shared_ptr<Graphics::IGraphicsDevice> wpGraphicsDevice = graphicsDevice.lock();
			if (!wpGraphicsDevice)
			{
				return;
			}
			//コンポーネントの追加
			{
				stageModel = wpStageModel = AddComponent<ModelObject>(wpGraphicsDevice);
				transform  = wpTransform  = AddComponent<Transform>();
			}

			//モデルの初期化
			if(wpStageModel)
			{
				wpStageModel->Initialize();
				//wpStageModel->Load("..\\Data\\Assets\\Model\\Stage\\MapCol.fbx");
				wpStageModel->Load("..\\Data\\Assets\\Model\\Stage\\floor.fbx");
				//wpStageModel->Load("..\\Data\\Assets\\Model\\SUNLITStage\\uploads_files_820010_Mountain.fbx");
			}

			//transformの初期化
			if(wpTransform)
			{
				wpTransform->Initialize();

				//Vector3    euler = { ToRadian(-90.0f),0.0f,0.0f };
				//Quaternion q = ConvertToQuaternionFromRollPitchYaw(euler.x, euler.y, euler.z);
				//wpTransform->SetRotate(q);
				wpTransform->SetScale({ 1.0f,1.0f ,1.0f });
				wpTransform->Update(firstElapsed);
			}

			//モデルのフェイス情報の設定
			if (!wpStageModel)
			{
				return;
			}
			while (1)
			{
				if (Graphics::IModelResource* resource = wpStageModel->GetModelResource())
				{
					if (resource->IsReady())
					{
						wpStageModel->BuildFaces();
						break;
					}
				}
			}
		}

		void __fastcall StageActor::PreUpdate(const f32& dt)
		{
			using namespace Bread::Math;
			for (auto& childAct : GetAllChildActor())
			{
				childAct->Update(dt);
			}
		}

		void __fastcall StageActor::Update(const f32& dt)
		{
			using namespace Bread::Math;
			std::shared_ptr<ModelObject> wpStageModel = stageModel.lock();
			std::shared_ptr<Transform>   wpTransform  = transform.lock();

			for (auto& childAct : GetAllChildActor())
			{
				childAct->Update(dt);
			}

			if (!wpStageModel && !wpTransform && !objMatrix)return;
			{
				Matrix  matrix = wpTransform->GetWorldTransform();
				Vector3 location = GetLocation(matrix);

				Matrix worldTransform = objMatrix;
				wpTransform->SetTranslate(GetLocation(worldTransform));
				wpTransform->SetRotate(GetRotation(worldTransform));
				wpTransform->SetScale(GetScale(worldTransform));
				wpTransform->Update(dt);
			}

			{
				wpStageModel->UpdateTransform(1.0f / 60.0f);
			}
		}

		void __fastcall StageActor::NextUpdate(const f32& dt)
		{
			for (auto& childAct : GetAllChildActor())
			{
				childAct->NextUpdate(dt);
			}
		}

		void __fastcall StageActor::Draw(const f32& dt)
		{
			for (auto& childAct : GetAllChildActor())
			{
				childAct->Draw(dt);
			}
		}
	}
}
