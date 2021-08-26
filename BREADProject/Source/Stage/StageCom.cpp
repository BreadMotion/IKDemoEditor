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

			//コンポーネントの追加
			{
				stageModel = AddComponent<ModelObject>(graphicsDevice);
				transform    = AddComponent<Transform>();
			}

			//モデルの初期化
			{
				stageModel->Initialize();
				stageModel->Load("..\\Data\\Assets\\Model\\SUNLITStage\\uploads_files_820010_Mountain.fbx");
			}

			//transformの初期化
			{
				transform->Initialize();

				Vector3       euler = { ToRadian(-90.0f),0.0f,0.0f };
				Quaternion q       = ConvertToQuaternionFromRollPitchYaw(euler.x, euler.y, euler.z);
				transform->SetRotate(q);
				transform->SetScale({ 5.0f,5.0f ,5.0f });
			}

			//モデルのフェイス情報の設定
			{
				if (stageModel->GetModelResource()->IsReady())
				{
					stageModel->BuildFaces();
				}
			}
		}

		void StageActor::Update(const f32& dt)
		{
			using namespace Bread;
			using namespace Bread::FrameWork;
			using namespace Bread::Math;

			for (auto& childAct : GetAllChildActor())
			{
				childAct->Update(dt);
			}

			Matrix             matrix = transform->GetWorldTransform();
			Vector3           location = GetLocation(matrix);

			Matrix worldTransform = objMatrix;
			transform->SetTranslate(GetLocation(worldTransform));
			transform->SetRotate(GetRotation(worldTransform));
			transform->SetScale(GetScale(worldTransform));
			transform->Update(dt);

			stageModel->UpdateTransform(1.0f / 60.0f);
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
