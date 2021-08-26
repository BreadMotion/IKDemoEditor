#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

#include <windows.h>
#include <tchar.h>
#include <sstream>
#include <memory>

#include <cassert>

#include <d3d11.h>
#include <wrl.h>

#include "FND/Util.h"
#include "FND/STD.h"
#include "./FrameWork/Component/Component.h"

namespace Bread
{
	namespace FrameWork
	{
		class GeometricPrimitive : public Component
		{
		public:
			struct Vertex
			{
				DirectX::XMFLOAT3 pos;
				DirectX::XMFLOAT3 normal;
			};

			struct Cbuffer
			{
				DirectX::XMFLOAT4X4 wvp; //���[���h�E�r���[�E�v���W�F�N�V���������s��
				DirectX::XMFLOAT4X4 world; //���[���h�ϊ��s��
				DirectX::XMFLOAT4     material_color; //�ގ��F
				DirectX::XMFLOAT4     light_direction; //���C�g�i�s����
			};

			enum GeometricPrimitiveType
			{
				CUBE,
				SPHERE,
				CYLINDER,
			} type;

		public:
			Microsoft::WRL::ComPtr <ID3D11VertexShader>        vertexShader;
			Microsoft::WRL::ComPtr <ID3D11PixelShader>          pixelShader;
			Microsoft::WRL::ComPtr <ID3D11InputLayout>          inputLayout;
			Microsoft::WRL::ComPtr <ID3D11Buffer>                    vertexBuffer; //�i���_�o�b�t�@�j
			Microsoft::WRL::ComPtr <ID3D11Buffer>                    indexBuffer; //�i�C���f�b�N�X�o�b�t�@�j
			Microsoft::WRL::ComPtr <ID3D11Buffer>                    constantBuffer; //�i�萔�o�b�t�@�j
			Microsoft::WRL::ComPtr <ID3D11RasterizerState>      wireframeRasterizerState; //�i���`��j
			Microsoft::WRL::ComPtr <ID3D11RasterizerState>      solidRasterizerState; //�i�h��Ԃ��`��j
			Microsoft::WRL::ComPtr <ID3D11DepthStencilState> depthStencilState;

			int numIndex = 0;

		public:
			GeometricPrimitive(ID3D11Device* device, int type, bool isCreateBottom = false, DirectX::XMFLOAT3* scale = 0);
			~GeometricPrimitive() {}

		public:
			// �R���X�g���N�^
			void Construct() override {}

			// �f�X�g���N�^
			void Destruct() override {}

			// ������
			void Initialize()override {}

			// �I����
			void Finalize()override {}

			// �X�V
			void Update()override{}

			// �`��
			void Draw()override {}

			//imgui
			void GUI()override 
			{
				using namespace ImGui;
				std::string guiName = "GeoPrimitive";
				if (ImGui::CollapsingHeader(u8"�W�I���g���v���~�e�B�u", ImGuiTreeNodeFlags_NavLeftJumpsBackHere | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Bullet))
				{
					char  name[128] = {};
					FND::StrCpy(name, sizeof(name), GetID().c_str());
					ImGui::Text(u8"���O"); ImGui::SameLine();
					ImGui::InputText(("##" + ID).c_str(), name, IM_ARRAYSIZE(name));
					SetID(name);

					ImGui::Separator();

					switch (type)
					{
					case GeometricPrimitiveType::CUBE:
						Text("CUBE");
						break;
					case GeometricPrimitiveType::SPHERE:
						Text("SPHERE");
						break;
					case GeometricPrimitiveType::CYLINDER:
						Text("CYLINDER");
						break;
					default:
						break;
					}
				}
			}

			//�`��
			void Render(
				ID3D11DeviceContext*,            //�f�o�C�X�R���e�L�X�g
				const DirectX::XMFLOAT4X4&, //���[���h�E�r���[�E�v���W�F�N�V���������s��
				const DirectX::XMFLOAT4X4&, //���[���h�ϊ��s��
				const DirectX::XMFLOAT4&,     //���C�g�i�s����
				const DirectX::XMFLOAT4&,     //�ގ��F
				bool                                          //���E�h��Ԃ��`��t���O
			);

			//�`��
			void Render(ID3D11DeviceContext*);

		private:
			void GeometricCube(ID3D11Device* device, DirectX::XMFLOAT3 scale, bool isCreateBottom);
			void GeometricCylinder(ID3D11Device* device);
			void GeometricSphere(ID3D11Device* device, u_int slices = 16, u_int stacks = 16);

			void CreateBuffer(ID3D11Device* device, Vertex* v, unsigned int* i, int numV, int numI);
		};
	}
}