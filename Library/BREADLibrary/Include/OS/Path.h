#pragma once

namespace Bread
{
	namespace OS
	{
		//****************************************************************************
		// �t�@�C���p�X���샆�[�e�B���e�B
		//****************************************************************************
		class Path
		{
		public:
			// �J�����g�f�B���N�g�����擾
			static const char* __fastcall  GetCurrentDirectory(char* buffer = nullptr, int bufferSize = 0);

			// �t���p�X���擾
			static const char* __fastcall GetFullPath(const char* filePath, char* buffer = nullptr, int bufferSize = 0);
			static const wchar_t* __fastcall GetFullPathW(const wchar_t* filePath, wchar_t* buffer = nullptr, int bufferSize = 0);

			// �t���p�X����t�@�C�������擾
			static const char* __fastcall GetFileName(const char* filePath);

			// �t���p�X����t�@�C���g���q���擾
			static const char* __fastcall GetFileExtension(const char* filePath);

			// �t���p�X�̃t�@�C���g���q��ύX
			static const char* __fastcall ChangeFileExtension(const char* filePath, const char* extension, char* buffer = nullptr, int bufferSize = 0);

			// �t���p�X����t�@�C���g���q���`�F�b�N
			static bool __fastcall CheckFileExtension(const char* filePath, const char* extension);

			// �t���p�X����t�@�C�������������f�B���N�g���p�X���擾
			static const char* __fastcall GetDirectoryName(const char* filePath, char* buffer = nullptr, int bufferSize = 0);

			// �t�@�C���p�X���������ɕϊ���'\\'��'/'�ϊ�
			static const char* __fastcall ToLower(const char* filePath, char* buffer = nullptr, int bufferSize = 0);

			// ../���܂ރp�X���œK��
			static const char* __fastcall Clean(const char* filePath, char* buffer = nullptr, int bufferSize = 0);

			// ���΃t�@�C���p�X�𐶐�
			static const char* __fastcall GetRelativePath(const char* basePath, const char* targetPath, char* buffer = nullptr, int bufferSize = 0);

			// ���΃t�@�C���p�X�����΃t�@�C���p�X�𐶐�
			static const char* __fastcall GetAbsolutePath(const char* basePath, const char* relativePath, char* buffer = nullptr, int bufferSize = 0);

			// �t�@�C���p�X����g���q�Ȃ��t�@�C�������擾
			static const char* __fastcall GetFileNameWithoutExtension(const char* filePath, char* buffer = nullptr, int bufferSize = 0);

			// �t�@�C���p�X������
			static const char* __fastcall Combine(const char* basePath, const char* targetPath, char* buffer = nullptr, int bufferSize = 0);

			// ���C���h�J�[�h�}�b�`���O
			// '*' �́A�����������ł��}�b�`���O����
			// '?' �́A�ꕶ�����ł��}�b�`���O����
			// �������C���h�J�[�h�̓X�y�[�X�܂��̓^�u�ŋ�؂邱��
			static bool __fastcall IsMatchWildcard(const char* filePath, const char* wildcard);

			// �p�X�����ꂩ��r
			static bool __fastcall  Equal(const char* filePth1, const char* filePath2);

			// ���s�\�t�@�C�������s����Ă���ꏊ�܂Ńf�B���N�V���� �i��F"C:\\Desktop\\Project\\�j
			// Debug��Release�̃t�@�C���p�X�͊܂܂Ȃ��B�����A�܂ނ̂ł����keep��true�ɂ���Ƃ��̂܂܂̏�ԂŕԂ��B
			static const char* __fastcall GetModuleFilePass(bool keep = false);

			// ���s�\�t�@�C�������s����Ă���ꏊ�܂Ńf�B���N�V���� �i��F"C:\\Desktop\\Project\\�j
			// backCount�̐����t�@�C���K�w��k��B
			static const char* __fastcall GetModuleFilePass(int backCount);
		};

	} // namespace OS
} // namespace Bread