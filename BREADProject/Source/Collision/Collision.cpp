#include "Collision.h"

namespace Bread
{
	namespace FrameWork
	{
		//������
		void CollisionCom::Initialize()
		{
			SetID(GetOwner()->GetID());

			std::shared_ptr<Actor> parentAct = GetOwner();
			targetT = parentAct->GetComponent<Transform>();
		}

		//�X�V
		void CollisionCom::Update()
		{
			using namespace Bread::Math;

			if (std::shared_ptr<Actor> parentAct = GetOwner())
			{

			}
		}

		//�`��
		void CollisionCom::Draw()
		{
		}
	}
}
