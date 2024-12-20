#pragma once
#include <vector>
#include <map>
#include <memory>
#include <string>
#include "Base.h"

namespace Bread
{
	namespace FND
	{
		//****************************************************************************
        // インスタンス管理クラス
        //****************************************************************************
		template <class T> class Instance : public Base
		{
		public:
			static T instance;
		};
		//↓の宣言をしたとき、全てのInstanceのコンストラクタが呼ばれる
		template <class T> T Instance<T> ::instance;

		template <class T> class SharedInstance : public Base
		{
		public:
			static std::shared_ptr<T> instance;

		public:
			template <class... Args>
			static T* makeInstancePtr(Args&&... args)
			{
				instance = std::make_shared<T>();
				return instance.get();
			}
		};
		//↓の宣言をした時点では、全てのshared_ptrが宣言されるが、実体の生成は行っていないのでコンストラクタは呼ばれない
		template <class T> std::shared_ptr<T> SharedInstance<T>::instance;

		template <class T> class UniqueInstance : public Base
		{
		public:
			static std::unique_ptr<T> instance;

		public:
			template <class... Args>
			static T* MakeInstancePtr(Args&&... args)
			{
				instance = std::make_unique<T>();
				return instance.get();
			}
		};
		//↓の宣言をした時点では、全てのUniqueInstanceが宣言されるが、実体の生成は行っていないのでコンストラクタは呼ばれない
		template <class T> std::unique_ptr<T> UniqueInstance<T>::instance;

		template <class T> class VectorInstance : public Base
		{
		public:
			static std::vector<T> instance;
		public:
			//エラー吐くから使うな
			template <class... Args>
			static T& __fastcall MakeInstance(Args&&... args)
			{
				instance.emplace_back();
				return instance.back();
			}
		};
		//↓の宣言をした時点では、全てのVectorInstanceが宣言されるが、実体の生成は行っていないのでコンストラクタは呼ばれない
		template <class T> std::vector<T> VectorInstance<T>::instance;


		template <class T> class MapInstance : public Base
		{
		public:
			static std::map<std::string, T> instance;
		public:

			//エラー吐くから使うな
			template <class... Args>
			static T& __fastcall MakeInstance(const std::string& str, Args&&... args)
			{
				T val{ std::forward<Args>(args)... };
				instance.insert(std::make_pair(str, val));
				return instance[str];
			}
		};
		//↓の宣言をした時点では、全てのMapInstanceが宣言されるが、実体の生成は行っていないのでコンストラクタは呼ばれない
		template <class T> std::map<std::string,T> MapInstance<T>::instance;
	}  // namespace FND
}  // namespace Bread