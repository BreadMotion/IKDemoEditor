#include "Collision.h"

namespace Bread
{
	namespace FrameWork
	{
		//初期化
		void CollisionCom::Initialize()
		{
			SetID(GetOwner()->GetID());

			std::shared_ptr<Actor> parentAct = GetOwner();
			targetT = parentAct->GetComponent<Transform>();
		}

		//更新
		void CollisionCom::Update()
		{
			using namespace Bread::Math;

			if (std::shared_ptr<Actor> parentAct = GetOwner())
			{

			}
		}

		//描画
		void CollisionCom::Draw()
		{
		}
	}
}
