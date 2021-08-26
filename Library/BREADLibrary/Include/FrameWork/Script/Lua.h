#pragma once

#include "../../ExternalLibrary/Lua53_Lib/include/lua.hpp"
#include <memory>
#include "Graphics\GraphicsDevice.h"


namespace Bread
{
	namespace FrameWork
	{
		class LuaSystem
		{
		private:
			lua_State* luaState = nullptr;
			bool onFunction     = false;
			s32 argNum          = 0;

		public:
			LuaSystem() {}
			~LuaSystem() {}

		public:
			// 生成
			static std::unique_ptr<LuaSystem> Create();

			// 初期化
			bool Initialize(Graphics::IGraphicsDevice* graphicsDevice);

			// 終了化
			void Finalize();

			// Luaファイルを開いて読み込み
			bool LoadScript(const s8* scriptFileName);

			// 関数開始
			void BeginFunction(const s8* functionName);

			// 関数を実行
			bool CallFunction(s32 returnValue);

			// 関数終了
			void EndFunction();

			// Int型引数の設定
			void SetIntArgument(s32 arg);

			// グローバル変数の取得
			template<class T>
			T GetGlobalValue(const s8* argName)
			{
				if (!onFunction) return T();

				lua_getglobal(luaState, argName);

				T val;

				int type = lua_type(luaState, 1);
				switch (type)
				{
				case LUA_TNUMBER:
					val = static_cast<T>(lua_tonumber(luaState, 1));
					break;

				default: break;
				}

				lua_pop(luaState, 1);

				return val;
			}
		};
	}
}