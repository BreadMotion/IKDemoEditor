#pragma once

#include <memory>

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
		template <class T> T Instance<T> ::instance;

		template <class T> class SharedInstance
		{
		public:
			static std::shared_ptr<T> instance;
		public:
			static T* makeInstancePtr()
			{
				instance = std::make_shared<T>();
				return instance.get();
			}
		};
		template <class T> std::shared_ptr<T> SharedInstance<T>::instance;

		template <class T> class UniqueInstance
		{
		public:
			static std::unique_ptr<T> instance;
		public:
			static T* MakeInstancePtr()
			{
				instance = std::make_unique<T>();
				return instance.get();
			}
		};
		template <class T> std::unique_ptr<T> UniqueInstance<T>::instance;
	}  // namespace FND
}  // namespace Bread