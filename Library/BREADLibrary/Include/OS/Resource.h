#pragma once

#include <memory>

#include "types.h"
#include "FND/Base.h"
#include "OS/Stream.h"


namespace Bread
{
	namespace OS
	{
		enum ResourceType
		{
			MODEL,
			ANIMATION,
			ALL,
		};

		//****************************************************************************
		// ���\�[�X������N���X�ł��B
		//****************************************************************************
		class Resource : public FND::Base
		{
		private:
			bool ready = false;

		public:
			// ����������Ԃɐݒ�
			void SetReady() { ready = true; }

			// ����������Ԃ�����
			bool IsReady() const { return ready; }
		};

		//****************************************************************************
		// ���\�[�X����C�x���g���L���b�`����C���^�[�t�F�[�X
		//****************************************************************************
		class IResourceListener : public FND::Base
		{
		public:
			// ���\�[�X���ǂݍ��܂ꂽ�Ƃ��ɌĂ΂��
			virtual void __fastcall OnResourceLoaded(Resource* resource) = 0;

		};

		//****************************************************************************
		// ���\�[�X����I�u�W�F�N�g�����C���^�[�t�F�[�X
		//****************************************************************************
		class IResourceFactory : public FND::Base
		{
		public:
			// ���\�[�X����I�u�W�F�N�g����
			virtual std::unique_ptr<Resource> __fastcall CreateResource(u32 type) = 0;

			// ���\�[�X�ǂݍ���
			virtual bool __fastcall LoadResource(Resource* resource, OS::IFileStream* stream, const char* filename) = 0;

		};

		//****************************************************************************
		// ���\�[�X����I�u�W�F�N�g�Ǘ��C���^�[�t�F�[�X
		//****************************************************************************
		class IResourceManager : public FND::Base
		{
		public:
			// ����
			static std::shared_ptr<IResourceManager> Create();

			// ������
			virtual bool __fastcall Initialize(const char* rootDirectory) = 0;

			// �I����
			virtual void Finalize() = 0;

			// �g���q�ɑΉ����郊�\�[�X�t�@�N�g���o�^
			virtual void __fastcall RegisterFactory(const char* ext, std::shared_ptr<IResourceFactory> factory) = 0;

			// ���\�[�X�C�x���g���X�i�[�o�^
			virtual void __fastcall RegisterListener(IResourceListener* listener) = 0;

			// ���\�[�X����I�u�W�F�N�g�񓯊��ǂݍ���
			virtual std::shared_ptr<Resource> __fastcall LoadAsync(const char* filename, u32 type = 0) = 0;

			// ���\�[�X����I�u�W�F�N�g�����ǂݍ���
			virtual std::shared_ptr<Resource> __fastcall  LoadImmediate(const char* filename, u32 type) = 0;

			// �ۗ����̂��ׂẴ��\�[�X���ǂݍ��܂��܂ő҂�
			virtual void WaitOnPending() = 0;

			// �폜���郊�\�[�X������ꍇ�A�������
			virtual int GarbageCollect() = 0;

			// �񓯊��ǂݍ���
			template<class T>
			std::shared_ptr<T> __fastcall LoadAsync(const char* filename, u32 type = 0)
			{
				return std::dynamic_pointer_cast<T>(LoadAsync(filename, type));
			}

			// �����ǂݍ���
			template<class T>
			std::shared_ptr<T> __fastcall LoadImmediate(const char* filename, u32 type = 0)
			{
				std::shared_ptr<Resource> resource = LoadImmediate(filename, type);
				if (!resource)
				{
					return nullptr;
				}

				return std::dynamic_pointer_cast<T>(resource);
			}
		};

	} // namespace OS
} // namespace Bread