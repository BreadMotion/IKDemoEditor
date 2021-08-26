#include "pch.h"
#include "FrameWork\Script\Lua.h"
#include "FND\Logger.h"
#include "OS\Path.h"


namespace Bread
{
	namespace FrameWork
	{
		// 生成
		std::unique_ptr<LuaSystem> LuaSystem::Create()
		{
			return std::make_unique<LuaSystem>();
		}

		// 初期化
		bool LuaSystem::Initialize(Graphics::IGraphicsDevice* graphicsDevice)
		{
			luaState = nullptr;
			luaState = luaL_newstate();

			onFunction = false;
			argNum     = 0;

			return true;
		}

		// 終了化
		void LuaSystem::Finalize()
		{
			lua_close(luaState);
			luaState = nullptr;
		}

		// Luaファイルを開いて読み込み
		bool LuaSystem::LoadScript(const s8* scriptFileName)
		{
			if (luaL_dofile(luaState, scriptFileName))
			{
				BREAD_LOG_GRP_ERROR("luaL_dofile : Failed!!\n");
				return false;
			}

			return true;
		}

		// 関数開始
		void LuaSystem::BeginFunction(const s8* functionName)
		{
			// Luaステート内にある関数を指定
			lua_getglobal(luaState, functionName);

			onFunction = true;
			argNum     = 0;
		}

		// 関数を実行
		bool LuaSystem::CallFunction(s32 returnValue)
		{
			if (!onFunction) return false;

			// Lua関数を実行
			if (lua_pcall(luaState, argNum, returnValue, 0))
			{
				BREAD_LOG_GRP_ERROR("lua_pcall : Failed!!\n");
				return false;
			}

			if (0 < returnValue)
			{
				// 戻り値を取得
				for (s32 i = 1; i < returnValue; ++i)
				{
					int val = (int)lua_tonumber(luaState, i);
				}

				// TODO : 戻り値の型
				// TODO : 戻り値を返す
			}

			return true;
		}

		// 関数終了
		void LuaSystem::EndFunction()
		{
			if (!onFunction) return;

			// スタックを削除
			lua_pop(luaState, argNum);
		}

		// Int型引数の設定
		void LuaSystem::SetIntArgument(s32 arg)
		{
			if (!onFunction) return;

			// 引数を設定
			lua_pushnumber(luaState, arg);

			argNum++;
		}
	}
}
