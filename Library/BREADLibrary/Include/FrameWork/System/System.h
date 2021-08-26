#pragma once


namespace Bread
{
	namespace FrameWork
	{
		class ISystem
		{
		public:
			ISystem() {}
			virtual ~ISystem() {}

		public:
			// コンストラクタ
			virtual void Construct()  = 0;

			// デストラクタ
			virtual void Destruct()   = 0;

			// 初期化
			virtual void Initialize() = 0;

			// 終了化
			virtual void Finalize()   = 0;

			// 更新
			virtual void Update()     = 0;

			// 描画
			virtual void Draw()       = 0;
		};
	}
}