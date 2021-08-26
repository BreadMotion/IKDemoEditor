#include "Collision.h"

namespace Bread
{
	namespace FrameWork
	{
		//初期化
		void CollisionCom::Initialize()
		{
			ID = GetOwner()->GetID();

			std::shared_ptr<Actor> parentAct = owner.lock();
			targetT = parentAct->GetComponent<Transform>();
		}

		//更新
		void CollisionCom::Update(const Bread::f32& elapsedTime)
		{
			using namespace Bread::Math;

			if (std::shared_ptr<Actor> parentAct = owner.lock())
			{

			}
		}

		//描画
		void CollisionCom::Draw(const Bread::f32& elapsedTime)
		{
		}
	}
}
