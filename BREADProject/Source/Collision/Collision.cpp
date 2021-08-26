#include "Collision.h"

namespace Bread
{
	namespace FrameWork
	{
		//‰Šú‰»
		void CollisionCom::Initialize()
		{
			ID = GetOwner()->GetID();

			std::shared_ptr<Actor> parentAct = owner.lock();
			targetT = parentAct->GetComponent<Transform>();
		}

		//XV
		void CollisionCom::Update(const Bread::f32& elapsedTime)
		{
			using namespace Bread::Math;

			if (std::shared_ptr<Actor> parentAct = owner.lock())
			{

			}
		}

		//•`‰æ
		void CollisionCom::Draw(const Bread::f32& elapsedTime)
		{
		}
	}
}
