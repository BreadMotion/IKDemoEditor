#include "LinkingSphere.h"
#include "Types.h"
#include "FND/Instance.h"

#include "Graphics/RenderManager.h"

#include "FrameWork/Actor/Actor.h"
#include "FrameWork/Object/Object.h"

#include "Sphere/SphereModelComponent.h"

#include "FrameWork/Actor/ActorManager.h"
#include "Math/BreadMath.h"

using Bread::FND::MapInstance;

using namespace Bread::Math;

namespace Bread
{
	namespace FrameWork
	{
		void LinkingSphere::Initialize()
		{
			//�R���|�[�l���g�̍\�z
			ComponentConstruction();

#if 1
			struct LocalFunction
			{
				static void GenerateBones(std::shared_ptr<Actor> owner,std::vector<IJoint*>& localJoint)
				{
					static s32 iterate{ 0 };
					//�P�O�����Ă��Ȃ�������
					if (++iterate < 10)
					{
						//�q�A�N�^�[�̐���
						auto childActor{ owner->AddChildActor<Actor>() };
						{//�q�A�N�^�[�̐ݒ�y�уR���|�[�l���g�̒ǉ�
							childActor->SetID(("SphereActor" + std::to_string(iterate)));
							childActor->AddComponent<SphereModelComponent>();
						}

						//���������A�N�^�[�̐e�ɑO��̃W���C���g��ݒ�
						//�����������A�N�^�[��O��̃W���C���g�̎q���ɐݒ�
						auto& childJoint{ childActor->GetComponent<ModelObject>()->GetNodes()->at(0) };
						if (localJoint.size() > 0)
						{
							childJoint.parent = localJoint.back();
							localJoint.back()->child.emplace_back(&childJoint);
						}

						//��̃I�u�W�F�N�g�Ƃ��ď����ł���悤�ɓo�^����
						localJoint.emplace_back(&childJoint);

						//�ċA����
						GenerateBones(childActor, localJoint);
					}
				}
			};
			LocalFunction::GenerateBones(GetOwner(), joints);
#endif
		}

		void LinkingSphere::PreUpdate()
		{
		}

		void LinkingSphere::Update()
		{
		}

		void LinkingSphere::NextUpdate()
		{
		}

		void LinkingSphere::Draw()
		{

		}

		void LinkingSphere::ComponentConstruction()
		{
			TransformConstruction();
		}

		void LinkingSphere::TransformConstruction()
		{
		/*	transform = GetOwner()->GetComponent<Transform>();
			transform->SetID("linkingSphereTransform");
			transform->SetTranslate(Math::Vector3{ 1000.0f,0.0f,0.0f });*/
		}
	}
}