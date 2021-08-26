#pragma once


namespace Bread
{
	namespace FrameWork
	{
		class ISystem
		{
		public:
			ISystem() {}
			virtual ~ISystem() {}

		public:
			// �R���X�g���N�^
			virtual void Construct()  = 0;

			// �f�X�g���N�^
			virtual void Destruct()   = 0;

			// ������
			virtual void Initialize() = 0;

			// �I����
			virtual void Finalize()   = 0;

			// �X�V
			virtual void Update()     = 0;

			// �`��
			virtual void Draw()       = 0;
		};
	}
}