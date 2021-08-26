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
				DirectX::XMFLOAT4X4 wvp; //ワールド・ビュー・プロジェクション合成行列
				DirectX::XMFLOAT4X4 world; //ワールド変換行列
				DirectX::XMFLOAT4     material_color; //材質色
				DirectX::XMFLOAT4     light_direction; //ライト進行方向
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
			Microsoft::WRL::ComPtr <ID3D11Buffer>                    vertexBuffer; //（頂点バッファ）
			Microsoft::WRL::ComPtr <ID3D11Buffer>                    indexBuffer; //（インデックスバッファ）
			Microsoft::WRL::ComPtr <ID3D11Buffer>                    constantBuffer; //（定数バッファ）
			Microsoft::WRL::ComPtr <ID3D11RasterizerState>      wireframeRasterizerState; //（線描画）
			Microsoft::WRL::ComPtr <ID3D11RasterizerState>      solidRasterizerState; //（塗りつぶし描画）
			Microsoft::WRL::ComPtr <ID3D11DepthStencilState> depthStencilState;

			int numIndex = 0;

		public:
			GeometricPrimitive(ID3D11Device* device, int type, bool isCreateBottom = false, DirectX::XMFLOAT3* scale = 0);
			~GeometricPrimitive() {}

		public:
			// コンストラクタ
			void Construct() override {}

			// デストラクタ
			void Destruct() override {}

			// 初期化
			void Initialize()override {}

			// 終了化
			void Finalize()override {}

			// 更新
			void Update()override{}

			// 描画
			void Draw()override {}

			//imgui
			void GUI()override 
			{
				using namespace ImGui;
				std::string guiName = "GeoPrimitive";
				if (ImGui::CollapsingHeader(u8"ジオメトリプリミティブ", ImGuiTreeNodeFlags_NavLeftJumpsBackHere | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Bullet))
				{
					char  name[128] = {};
					FND::StrCpy(name, sizeof(name), GetID().c_str());
					ImGui::Text(u8"名前"); ImGui::SameLine();
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

			//描画
			void Render(
				ID3D11DeviceContext*,            //デバイスコンテキスト
				const DirectX::XMFLOAT4X4&, //ワールド・ビュー・プロジェクション合成行列
				const DirectX::XMFLOAT4X4&, //ワールド変換行列
				const DirectX::XMFLOAT4&,     //ライト進行方向
				const DirectX::XMFLOAT4&,     //材質色
				bool                                          //線・塗りつぶし描画フラグ
			);

			//描画
			void Render(ID3D11DeviceContext*);

		private:
			void GeometricCube(ID3D11Device* device, DirectX::XMFLOAT3 scale, bool isCreateBottom);
			void GeometricCylinder(ID3D11Device* device);
			void GeometricSphere(ID3D11Device* device, u_int slices = 16, u_int stacks = 16);

			void CreateBuffer(ID3D11Device* device, Vertex* v, unsigned int* i, int numV, int numI);
		};
	}
}