#include "Collision.h"

namespace Bread
{
	namespace FrameWork
	{
		//‰Šú‰»
		void CollisionCom::Initialize()
		{
			using namespace Bread;
			using namespace Bread::FrameWork;
			using namespace Bread::Math;

			std::shared_ptr<Actor> parentAct = owner.lock();
			targetT = parentAct->GetComponent<Transform>();
		}

		//XV
		void CollisionCom::Update(const Bread::f32& elapsedTime)
		{
			using namespace Bread;
			using namespace Bread::FrameWork;
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
