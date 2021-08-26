#include "Collision.h"

namespace Bread
{
	namespace FrameWork
	{
		//初期化
		void CollisionCom::Initialize()
		{
			using namespace Bread;
			using namespace Bread::FrameWork;
			using namespace Bread::Math;

			std::shared_ptr<Actor> parentAct = owner.lock();
			targetT = parentAct->GetComponent<Transform>();
		}

		//更新
		void CollisionCom::Update()
		{
			using namespace Bread;
			using namespace Bread::FrameWork;
			using namespace Bread::Math;

			if (std::shared_ptr<Actor> parentAct = owner.lock())
			{
			
			}
		}
		
		//描画
		void CollisionCom::Draw()
		{
		}
	}
}
