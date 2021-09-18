#pragma once

#include <memory>
#include "Types.h"
#include "FND/Base.h"


namespace Bread
{
	namespace OS
	{
		//****************************************************************************
		// �X�g���[���V�[�N
		//****************************************************************************
		enum class StreamSeek
		{
			Top,		// �t�@�C���̐擪
			Current,	// �t�@�C���̌��݈ʒu
			End			// �t�@�C���̏I�[
		};

		//****************************************************************************
		// �t�@�C���A�N�Z�X
		//****************************************************************************
		enum class FileAccess
		{
			Read,	// �ǂݍ���
			Write,	// ��������
			Append	// �ǉ���������
		};

		//****************************************************************************
		// �X�g���[������C���^�[�t�F�[�X
		//****************************************************************************
		class IStream : public FND::Base
		{
		public:
			// �N���[�Y
			virtual void Close() = 0;

			// �ǂݍ���
			virtual s32 __fastcall Read(void* buffer, s32 size) = 0;

			// ��������
			virtual s32 __fastcall  Write(const void* buffer, s32 size) = 0;

			// �|�C���^�ʒu�ݒ�
			virtual bool __fastcall Seek(s32 offset, StreamSeek origin) = 0;

			// �|�C���^�ʒu�擾
			virtual s32 Tell() = 0;

			// �T�C�Y�擾
			virtual s32 GetSize() = 0;
		};

		//****************************************************************************
		// �t�@�C���X�g���[������C���^�[�t�F�[�X
		//****************************************************************************
		class IFileStream : public IStream
		{
		public:
			// �t�@�C������
			static std::unique_ptr<IFileStream> Create();

			// ������
			virtual bool __fastcall Initialize(const char* basePath) = 0;

			// �I����
			virtual void Finalize() = 0;

			// ���݊m�F
			virtual bool __fastcall Exists(const char* path) = 0;
			virtual bool __fastcall ExistsW(const wchar_t* path) = 0;

			// �I�[�v��
			virtual bool __fastcall Open(const char* path, FileAccess access) = 0;

			// �t�@�C���̍폜
			virtual bool __fastcall Remove(const char* path) = 0;
		};

		//****************************************************************************
		// �������X�g���[������C���^�[�t�F�[�X
		//****************************************************************************
		class IMemoryStream : public IStream
		{
		public:
			// �t�@�C������
			static std::unique_ptr<IMemoryStream> Create();

			// ������
			virtual bool Initialize() = 0;

			// �I����
			virtual void Finalize() = 0;

			// �I�[�v��
			virtual bool __fastcall Open(void* buffer, s32 size) = 0;
		};
	} // namespace OS
} // namespace Bread