#pragma once

#include <memory>
#include <string>
#include "OS/Main.h"
#include "OS/Display.h"
#include "Graphics/GraphicsDevice.h"


namespace Bread
{
	namespace FrameWork
	{
		//****************************************************************************
		// メインループ
		//****************************************************************************
		class Main : public OS::IMain
		{
		protected:
			std::unique_ptr<OS::IDisplay>              display;
			std::shared_ptr<Graphics::IGraphicsDevice> graphicsDevice;
			float elapsedTime = 0.0f;

		public:
			Main() {}
			~Main() {}

		public:
			// 初期化
			bool Initialize(uintPtr instance) override;

			// 終了化
			void Finalize() override;

			// 終了化
			void Run() override;

		protected:
			// 更新
			virtual void Update(Bread::f32 elapsedTime) = 0;

			// 描画
			virtual void Render(Bread::f32 elapsedTime) = 0;

		public:
			OS::IDisplay* getDisplay() { return display.get(); }
			Graphics::IGraphicsDevice* getGraphicsDevice() { return graphicsDevice.get(); }
		};
	} // namespace FrameWork
} // namespace Bread