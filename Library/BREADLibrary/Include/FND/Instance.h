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
        // �C���X�^���X�Ǘ��N���X
        //****************************************************************************
		template <class T> class Instance : public Base
		{
		public:
			static T instance;
		};
		//���̐錾�������Ƃ��A�S�Ă�Instance�̃R���X�g���N�^���Ă΂��
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
		//���̐錾���������_�ł́A�S�Ă�shared_ptr���錾����邪�A���̂̐����͍s���Ă��Ȃ��̂ŃR���X�g���N�^�͌Ă΂�Ȃ�
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
		//���̐錾���������_�ł́A�S�Ă�UniqueInstance���錾����邪�A���̂̐����͍s���Ă��Ȃ��̂ŃR���X�g���N�^�͌Ă΂�Ȃ�
		template <class T> std::unique_ptr<T> UniqueInstance<T>::instance;

		template <class T> class VectorInstance : public Base
		{
		public:
			static std::vector<T> instance;
		public:
			//�G���[�f������g����
			template <class... Args>
			static T& __fastcall MakeInstance(Args&&... args)
			{
				instance.emplace_back();
				return instance.back();
			}
		};
		//���̐錾���������_�ł́A�S�Ă�VectorInstance���錾����邪�A���̂̐����͍s���Ă��Ȃ��̂ŃR���X�g���N�^�͌Ă΂�Ȃ�
		template <class T> std::vector<T> VectorInstance<T>::instance;


		template <class T> class MapInstance : public Base
		{
		public:
			static std::map<std::string, T> instance;
		public:

			//�G���[�f������g����
			template <class... Args>
			static T& __fastcall MakeInstance(const std::string& str, Args&&... args)
			{
				T val{ std::forward<Args>(args)... };
				instance.insert(std::make_pair(str, val));
				return instance[str];
			}
		};
		//���̐錾���������_�ł́A�S�Ă�MapInstance���錾����邪�A���̂̐����͍s���Ă��Ȃ��̂ŃR���X�g���N�^�͌Ă΂�Ȃ�
		template <class T> std::map<std::string,T> MapInstance<T>::instance;
	}  // namespace FND
}  // namespace Bread