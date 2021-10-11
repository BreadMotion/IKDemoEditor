#include "StageCom.h"
#include "Math/BreadMath.h"
#include "FND/Instance.h"
#include "FrameWork/Object/TerrainManager.h"

using Bread::FND::Instance;

namespace Bread
{
	namespace FrameWork
	{
		std::shared_ptr<Actor> StageActor::Create()
		{
			return std::make_shared<StageActor>();
		}

		void StageActor::Initialize()
		{
			using namespace Bread;
			using namespace Bread::FrameWork;
			using namespace Bread::Math;

			std::shared_ptr<ModelObject> wpStageModel;
			std::shared_ptr<Transform>   wpTransform;
			const float  firstElapsed = 100.0f;

			//�R���|�[�l���g�̒ǉ�
			{
				stageModel = wpStageModel = AddComponent<ModelObject>();
				transform  = wpTransform  = AddComponent<Transform>();
			}

			//���f���̏�����
			if(wpStageModel)
			{
				wpStageModel->Initialize();
				//wpStageModel->Load("..\\Data\\Assets\\Model\\Stage\\MapCol.fbx");
				wpStageModel->Load("..\\Data\\Assets\\Model\\Stage\\floor.fbx");
				//wpStageModel->Load("..\\Data\\Assets\\Model\\SUNLITStage\\uploads_files_820010_Mountain.fbx");
			}

			//transform�̏�����
			if(wpTransform)
			{
				wpTransform->Initialize();

				//Vector3    euler = { ToRadian(-90.0f),0.0f,0.0f };
				//Quaternion q = ConvertToQuaternionFromRollPitchYaw(euler.x, euler.y, euler.z);
				//wpTransform->SetRotate(q);
				wpTransform->SetScale({ 1.0f,1.0f ,1.0f });
				wpTransform->Update();
			}

			//���f���̃t�F�C�X���̐ݒ�
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

		void __fastcall StageActor::PreUpdate()
		{
			using namespace Bread::Math;
			for (auto& childAct : GetAllChildActor())
			{
				childAct->Update();
			}
		}

		void __fastcall StageActor::Update()
		{
			using namespace Bread::Math;
			std::shared_ptr<ModelObject> wpStageModel = stageModel.lock();
			std::shared_ptr<Transform>   wpTransform  = transform.lock();

			for (auto& childAct : GetAllChildActor())
			{
				childAct->Update();
			}

			if (!wpStageModel && !wpTransform && !objMatrix)return;
			{
				Matrix  matrix = wpTransform->GetWorldTransform();
				Vector3 location = GetLocation(matrix);

				Matrix worldTransform = objMatrix;
				wpTransform->SetTranslate(GetLocation(worldTransform));
				wpTransform->SetRotate(GetRotation(worldTransform));
				wpTransform->SetScale(GetScale(worldTransform));
				wpTransform->Update();
			}

			{
				wpStageModel->UpdateTransform(1.0f / 60.0f);
			}
		}

		void __fastcall StageActor::NextUpdate()
		{
			for (auto& childAct : GetAllChildActor())
			{
				childAct->NextUpdate();
			}
		}

		void __fastcall StageActor::Draw()
		{
			for (auto& childAct : GetAllChildActor())
			{
				childAct->Draw();
			}
		}
	}
}
