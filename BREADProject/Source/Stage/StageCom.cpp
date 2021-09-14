#include "StageCom.h"
#include "Math/BreadMath.h"

namespace Bread
{
	namespace FrameWork
	{
		std::shared_ptr<Actor> StageActor::Create(Graphics::IGraphicsDevice* graphicsDevice)
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
			//コンポーネントの追加
			{
				stageModel = wpStageModel = AddComponent<ModelObject>(graphicsDevice);
				transform  = wpTransform  = AddComponent<Transform>();
			}

			//モデルの初期化
			if(wpStageModel)
			{
				wpStageModel->Initialize();
				wpStageModel->Load("..\\Data\\Assets\\Model\\SUNLITStage\\uploads_files_820010_Mountain.fbx");
			}

			//transformの初期化
			if(wpTransform)
			{
				wpTransform->Initialize();

				Vector3    euler = { ToRadian(-90.0f),0.0f,0.0f };
				Quaternion q = ConvertToQuaternionFromRollPitchYaw(euler.x, euler.y, euler.z);
				wpTransform->SetRotate(q);
				wpTransform->SetScale({ 5.0f,5.0f ,5.0f });
				wpTransform->Update(firstElapsed);
			}

			//モデルのフェイス情報の設定
			if (wpStageModel)
			{
				if (wpStageModel->GetModelResource()->IsReady())
				{
					wpStageModel->BuildFaces();
				}
			}
		}

		void StageActor::PreUpdate(const f32& dt)
		{
			using namespace Bread::Math;
			for (auto& childAct : GetAllChildActor())
			{
				childAct->Update(dt);
			}
		}

		void StageActor::Update(const f32& dt)
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

		void StageActor::NextUpdate(const f32& dt)
		{
			for (auto& childAct : GetAllChildActor())
			{
				childAct->NextUpdate(dt);
			}
		}

		void StageActor::Draw(const f32& dt)
		{
			for (auto& childAct : GetAllChildActor())
			{
				childAct->Draw(dt);
			}
		}
	}
}
