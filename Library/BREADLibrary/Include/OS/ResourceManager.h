#pragma once

#include <map>
#include <string>
#include <memory>
#include <vector>

#include "Graphics/GraphicsDevice.h"
#include "Graphics/Model.h"
#include "Graphics/Animation.h"
#include "OS/CriticalSection.h"
#include "OS/Resource.h"
#include "OS/Stream.h"
#include "OS/Thread.h"
#include "OS/Event.h"


namespace Bread
{
	namespace OS
	{
		//****************************************************************************
		// ���\�[�X�}�l�[�W���[
		//****************************************************************************
		class ResourceManager final : public IResourceManager, public IThreadWorker
		{
		private:
			using ResourceInfoMap = std::map<std::string, std::shared_ptr<Resource>>;

			std::map<std::string, std::shared_ptr<IResourceFactory>> factories;
			std::vector<IResourceListener*>                          listeners;

			std::unique_ptr<ICriticalSection> criticalSection;
			std::unique_ptr<IFileStream>      file;
			std::unique_ptr<IThread>          thread;
			std::unique_ptr<IEvent>           event;

			ResourceInfoMap loaded;
			ResourceInfoMap pending;

			bool exitRequested = false;

		public:
			ResourceManager() {}
			~ResourceManager() override {}

		public:
			// ������
			bool __fastcall Initialize(const char* rootDirectory) override;

			// �I����
			void Finalize() override;

			// �g���q�ɑΉ����郊�\�[�X�t�@�N�g���o�^
			void __fastcall RegisterFactory(const char* ext, std::shared_ptr<IResourceFactory> factory) override;

			// ���\�[�X�C�x���g���X�i�[�o�^
			void __fastcall RegisterListener(IResourceListener* listener) override;

			// ���\�[�X����I�u�W�F�N�g�񓯊��ǂݍ���
			std::shared_ptr<Resource> __fastcall LoadAsync(const char* filename, u32 type = 0) override;

			// ���\�[�X����I�u�W�F�N�g�����ǂݍ���
			std::shared_ptr<Resource> __fastcall LoadImmediate(const char* filename, u32 type) override;

			// �ۗ����̂��ׂẴ��\�[�X���ǂݍ��܂��܂ő҂�
			void WaitOnPending() override;

			// �폜���郊�\�[�X������ꍇ�A�������
			int GarbageCollect() override;

			// �X���b�h���s����
			void Execute() override;

			// �X���b�h�����I��
			void Exit() override;

		public:
			//���\�[�X�̎擾
			std::shared_ptr<Resource> __fastcall GetResource(const char* filename);

		private:
			// �t�@�N�g���擾
			IResourceFactory* __fastcall GetFactory(const char* filename);

			// ���\�[�X�ǂݍ���
			bool __fastcall LoadResource(Resource* resource, const char* filename);
		};
	} // namespace OS
} // namespace Bread