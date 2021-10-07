#pragma once

#include <memory>
#include <string>
#include "OS/Main.h"
#include "OS/Display.h"
#include "Graphics/GraphicsDevice.h"
#include "FND/Instance.h"


namespace Bread
{
	namespace FrameWork
	{
		//****************************************************************************
		// メインループ
		//****************************************************************************
		class Main : public OS::IMain
		{
		public:
			Main() {}
			~Main() {}

		public:
			// 初期化
			bool __fastcall Initialize(uintPtr instance) override;

			// 終了化
			void Finalize() override;

			// 終了化
			void Run() override;

		protected:
			// 更新
			virtual void __fastcall Update() = 0;

			// 描画
			virtual void __fastcall Render() = 0;
		};
	} // namespace FrameWork
} // namespace Bread