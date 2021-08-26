#include "pch.h"
#include <map>
#include <string>
#include <sstream>
#include <algorithm>
#include "TextureDX11.h"
#include "Device/Win/DirectX11/DeviceDX11.h"
#include "Buffer/Win/DirectX11/BufferDX11.h"
#include "FND/Assert.h"
#include "FND/Logger.h"
#include "FND/Util.h"
#include "FND/STD.h"
#include "OS/Path.h"
#include "WICTextureLoader/WICTextureLoader.h"
#include "DirectXTex/DirectXTex.h"

namespace Bread
{
	template<UINT length>
	inline void SetDebugObjectName(ID3D11DeviceChild* resource, const char(&name)[length])
	{
#if defined(_DEBUG) || defined(PROFILE)
		resource->SetPrivateData(WKPDID_D3DDebugObjectName, length - 1, name);
#else
		UNREFERENCED_PARAMETER(resource);
		UNREFERENCED_PARAMETER(name);
#endif
	}
}

namespace Bread
{
	namespace Graphics
	{
		//****************************************************************************
		// DirectX11版テクスチャ操作オブジェクト
		//****************************************************************************
		// 生成
		std::unique_ptr<ITexture> ITexture::Create()
		{
			return std::make_unique<TextureDX11>();
		}

		// 初期化
		bool TextureDX11::Initialize(ID3D11Texture2D* texture, ID3D11ShaderResourceView* d3dShaderResourceView)
		{
			texture2d = texture;
			shaderResourceView = d3dShaderResourceView;

			return true;
		}

		// 初期化
		bool TextureDX11::Initialize(IDevice* device, const TextureDesc& desc)
		{
			ID3D11Device* d3dDevice = static_cast<DeviceDX11*>(device)->GetD3DDevice();

			DXGI_FORMAT dxgiFormat = GetDXGIFormat(desc.format);
			D3D11_RESOURCE_DIMENSION d3dResourceDimension = GetD3DResourceDimension(desc.dimension);

			UINT width = desc.width;
			UINT height = desc.height;
			UINT depth = desc.depth;
			UINT arraySize = desc.arraySize;
			UINT mipCount = desc.mipLevels;
			D3D11_USAGE usage = D3D11_USAGE_DEFAULT;
			D3D11_BIND_FLAG bindFlag = D3D11_BIND_SHADER_RESOURCE;
			UINT cupAccessFlag = 0;
			UINT miscFlag = 0;
			bool forceSrgb = false;
			bool isCubemap = desc.dimension == TextureDimension::Cube;
			const uint8_t* bitData = reinterpret_cast<const uint8_t*>(desc.sysMem);
			UINT bitSize = desc.sysMemSize;
			UINT maxSize = 8192;

			// ミップマップを自動生成するかチェック
			bool autogen = false;

			HRESULT hr;
			if (mipCount == 1)
			{
				// 自動生成ミップマップで形式がサポートされているかどうかを確認
				UINT formatSupport = 0;
				hr = d3dDevice->CheckFormatSupport(dxgiFormat, &formatSupport);
				if (SUCCEEDED(hr) && (formatSupport & D3D11_FORMAT_SUPPORT_MIP_AUTOGEN))
				{
					// フィーチャーレベル９以下はボリュームテクスチャに対応していない
					if ((d3dResourceDimension != D3D11_RESOURCE_DIMENSION_TEXTURE3D)
						|| (d3dDevice->GetFeatureLevel() >= D3D_FEATURE_LEVEL_10_0))
					{
						autogen = true;
					}
				}
			}

			// ミップマップの自動生成
			if (autogen)
			{
				hr = CreateD3DResources(d3dDevice, d3dResourceDimension, width, height, depth, 0, arraySize,
					dxgiFormat, usage,
					bindFlag | D3D11_BIND_RENDER_TARGET,
					cupAccessFlag,
					miscFlag | D3D11_RESOURCE_MISC_GENERATE_MIPS, forceSrgb,
					isCubemap, nullptr, &resource, &shaderResourceView);

				if (SUCCEEDED(hr))
				{
					UINT numBytes = 0;
					UINT rowBytes = 0;
					GetSurfaceInfo(width, height, dxgiFormat, &numBytes, &rowBytes, nullptr);

					if (numBytes > bitSize)
					{
						shaderResourceView->Release();
						shaderResourceView = nullptr;
						resource->Release();
						return false;
					}

					D3D11_SHADER_RESOURCE_VIEW_DESC d3dSrvDesc;
					shaderResourceView->GetDesc(&d3dSrvDesc);

					UINT mipLevels = 1;

					switch (d3dSrvDesc.ViewDimension)
					{
					case D3D_SRV_DIMENSION_TEXTURE1D:       mipLevels = d3dSrvDesc.Texture1D.MipLevels; break;
					case D3D_SRV_DIMENSION_TEXTURE1DARRAY:  mipLevels = d3dSrvDesc.Texture1DArray.MipLevels; break;
					case D3D_SRV_DIMENSION_TEXTURE2D:       mipLevels = d3dSrvDesc.Texture2D.MipLevels; break;
					case D3D_SRV_DIMENSION_TEXTURE2DARRAY:  mipLevels = d3dSrvDesc.Texture2DArray.MipLevels; break;
					case D3D_SRV_DIMENSION_TEXTURECUBE:     mipLevels = d3dSrvDesc.TextureCube.MipLevels; break;
					case D3D_SRV_DIMENSION_TEXTURECUBEARRAY:mipLevels = d3dSrvDesc.TextureCubeArray.MipLevels; break;
					case D3D_SRV_DIMENSION_TEXTURE3D:       mipLevels = d3dSrvDesc.Texture3D.MipLevels; break;
					default:
						shaderResourceView->Release();
						shaderResourceView = nullptr;
						resource->Release();
						return false;
					}

					ID3D11DeviceContext* d3dContext = nullptr;
					d3dDevice->GetImmediateContext(&d3dContext);

					if (arraySize > 1)
					{
						const uint8_t* srcBits = bitData;
						const uint8_t* endBits = bitData + bitSize;
						for (UINT item = 0; item < arraySize; ++item)
						{
							if ((srcBits + numBytes) > endBits)
							{
								shaderResourceView->Release();
								shaderResourceView = nullptr;
								resource->Release();
								d3dContext->Release();
								return false;
							}

							UINT res = D3D11CalcSubresource(0, item, mipLevels);
							d3dContext->UpdateSubresource(resource, res, nullptr, srcBits, static_cast<UINT>(rowBytes), static_cast<UINT>(numBytes));
							srcBits += numBytes;
						}
					}
					else
					{
						d3dContext->UpdateSubresource(resource, 0, nullptr, bitData, static_cast<UINT>(rowBytes), static_cast<UINT>(numBytes));
					}

					d3dContext->GenerateMips(shaderResourceView);
					d3dContext->Release();
				}
			}
			else
			{
				// テクスチャ生成
				std::unique_ptr<D3D11_SUBRESOURCE_DATA[]> d3dSubresourceDatas = std::make_unique<D3D11_SUBRESOURCE_DATA[]>(mipCount * arraySize);

				UINT skipMip = 0;
				UINT twidth = 0;
				UINT theight = 0;
				UINT tdepth = 0;
				hr = FillInitData(width, height, depth, mipCount, arraySize, dxgiFormat, maxSize, bitSize, bitData,
					twidth, theight, tdepth, skipMip, d3dSubresourceDatas.get());

				if (SUCCEEDED(hr))
				{
					hr = CreateD3DResources(d3dDevice, d3dResourceDimension, twidth, theight, tdepth, mipCount - skipMip, arraySize,
						dxgiFormat, usage, bindFlag, cupAccessFlag, miscFlag, forceSrgb,
						isCubemap, d3dSubresourceDatas.get(), &resource, &shaderResourceView);

					if (FAILED(hr) && !maxSize && (mipCount > 1))
					{
						// Retry with a maxsize determined by feature level
						switch (d3dDevice->GetFeatureLevel())
						{
						case D3D_FEATURE_LEVEL_9_1:
						case D3D_FEATURE_LEVEL_9_2:
							if (isCubemap)
							{
								maxSize = 512 /*D3D_FL9_1_REQ_TEXTURECUBE_DIMENSION*/;
							}
							else
							{
								maxSize = (d3dResourceDimension == D3D11_RESOURCE_DIMENSION_TEXTURE3D)
									? 256 /*D3D_FL9_1_REQ_TEXTURE3D_U_V_OR_W_DIMENSION*/
									: 2048 /*D3D_FL9_1_REQ_TEXTURE2D_U_OR_V_DIMENSION*/;
							}
							break;

						case D3D_FEATURE_LEVEL_9_3:
							maxSize = (d3dResourceDimension == D3D11_RESOURCE_DIMENSION_TEXTURE3D)
								? 256 /*D3D_FL9_1_REQ_TEXTURE3D_U_V_OR_W_DIMENSION*/
								: 4096 /*D3D_FL9_3_REQ_TEXTURE2D_U_OR_V_DIMENSION*/;
							break;

						default: // D3D_FEATURE_LEVEL_10_0 & D3D_FEATURE_LEVEL_10_1
							maxSize = (d3dResourceDimension == D3D11_RESOURCE_DIMENSION_TEXTURE3D)
								? 2048 /*D3D10_REQ_TEXTURE3D_U_V_OR_W_DIMENSION*/
								: 8192 /*D3D10_REQ_TEXTURE2D_U_OR_V_DIMENSION*/;
							break;
						}

						hr = FillInitData(width, height, depth, mipCount, arraySize, dxgiFormat, maxSize, bitSize, bitData,
							twidth, theight, tdepth, skipMip, d3dSubresourceDatas.get());
						if (SUCCEEDED(hr))
						{
							hr = CreateD3DResources(d3dDevice, d3dResourceDimension, twidth, theight, tdepth, mipCount - skipMip, arraySize,
								dxgiFormat, usage, bindFlag, cupAccessFlag, miscFlag, forceSrgb,
								isCubemap, d3dSubresourceDatas.get(), &resource, &shaderResourceView);
						}
					}
				}
			}

			return true;
		}

		// 初期化
		bool TextureDX11::Initialize(IDevice* device, const ShaderResouceDesc& desc, IBuffer* buffer)
		{
			ID3D11Device* d3dDevice = static_cast<DeviceDX11*>(device)->GetD3DDevice();
			ID3D11Buffer* d3dBuffer = static_cast<BufferDX11*>(buffer)->GetD3DBuffer();

			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
			Bread::FND::MemSet(&srvDesc, 0, sizeof(srvDesc));
			{
				srvDesc.Format = static_cast<DXGI_FORMAT>(desc.format);
				srvDesc.ViewDimension = static_cast<D3D11_SRV_DIMENSION>(desc.viewDimension);
				{
					srvDesc.Buffer.FirstElement = desc.buffer.firstElement;
					srvDesc.Buffer.ElementOffset = desc.buffer.elementOffset;
					srvDesc.Buffer.NumElements = desc.buffer.numElements;
					srvDesc.Buffer.ElementWidth = desc.buffer.elementWidth;

					srvDesc.BufferEx.FirstElement = desc.bufferEx.firstElement;
					srvDesc.BufferEx.NumElements = desc.bufferEx.numElements;
					srvDesc.BufferEx.Flags = desc.bufferEx.flags;
				}
			}

			d3dDevice->CreateShaderResourceView(d3dBuffer, &srvDesc, &shaderResourceView);

			return true;
		}

		bool TextureDX11::Initialize(IDevice* device, const UnorderedAccessViewDesc& desc, IBuffer* buffer)
		{
			ID3D11Device* d3dDevice = static_cast<DeviceDX11*>(device)->GetD3DDevice();
			ID3D11Buffer* d3dBuffer = static_cast<BufferDX11*>(buffer)->GetD3DBuffer();

			D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
			Bread::FND::MemSet(&uavDesc, 0, sizeof(uavDesc));
			{
				uavDesc.Format = static_cast<DXGI_FORMAT>(desc.format);
				uavDesc.ViewDimension = static_cast<D3D11_UAV_DIMENSION>(desc.viewDimension);
				{
					uavDesc.Buffer.FirstElement = desc.buffer.firstElement;
					uavDesc.Buffer.NumElements = desc.buffer.numElements;
					uavDesc.Buffer.Flags = desc.buffer.flags;
				}
			}

			d3dDevice->CreateUnorderedAccessView(d3dBuffer, &uavDesc, &unorderedAccessView);

			return true;
		}

		// 初期化
		bool TextureDX11::Initialize(IDevice* device, const char* filename, MaterialType materialType, const Math::Color& color)
		{
			bool check = false;

			std::function<void(const Math::Color&, DWORD&)> convert = [&](const Math::Color& colour, DWORD& RGBA)
			{
				DWORD R = static_cast<BYTE>(colour.r * 255);
				DWORD G = static_cast<BYTE>(colour.g * 255);
				DWORD B = static_cast<BYTE>(colour.b * 255);
				DWORD A = static_cast<BYTE>(colour.a * 255);
				RGBA = R | (G << 8) | (B << 16) | (A << 24);
			};

			ID3D11Device* d3dDevice = static_cast<DeviceDX11*>(device)->GetD3DDevice();

			switch (materialType)
			{
			case Bread::Graphics::MaterialType::Diffuse:
			case Bread::Graphics::MaterialType::Ambient:
			case Bread::Graphics::MaterialType::Specular:
				if (filename[0] == NULL)
				{
					DWORD RGBA = 0;
					convert(color, RGBA);
					check = CreateDummyTexture(d3dDevice, &shaderResourceView, RGBA, 1, true, true);
				}
				else
				{
					std::unique_ptr<OS::IFileStream> file;
					file = OS::IFileStream::Create();
					file->Initialize(nullptr);

					const char* fullPass = OS::Path::GetFullPath(filename);

					if (!file->Exists(fullPass)) break;

					check = LoadTextureFromFile(d3dDevice, fullPass, &shaderResourceView);
				}
				break;
			case Bread::Graphics::MaterialType::NormalMap:
			case Bread::Graphics::MaterialType::Bump:
				if (filename[0] == NULL)
				{
					DWORD RGBA = 0;
					convert(color, RGBA);
					check = CreateDummyTexture(d3dDevice, &shaderResourceView, 0xFFFF7F7F, 1, false, true);
				}
				else
				{
					std::unique_ptr<OS::IFileStream> file;
					file = OS::IFileStream::Create();
					file->Initialize(nullptr);

					const char* fullPass = OS::Path::GetFullPath(filename);

					if (!file->Exists(fullPass)) break;

					check = LoadTextureFromFile(d3dDevice, fullPass, &shaderResourceView);
				}
				break;
			case Bread::Graphics::MaterialType::Emissive:
				if (filename[0] == NULL)
				{
					DWORD RGBA = 0;
					convert(color, RGBA);
					check = CreateDummyTexture(d3dDevice, &shaderResourceView, 0x00000000, 1, true, true);
				}
				else
				{
					std::unique_ptr<OS::IFileStream> file;
					file = OS::IFileStream::Create();
					file->Initialize(nullptr);

					const char* fullPass = OS::Path::GetFullPath(filename);

					if (!file->Exists(fullPass)) break;

					check = LoadTextureFromFile(d3dDevice, fullPass, &shaderResourceView);
				}
				break;
			default: break;
			}

			return check;
		}

		// 終了化
		void TextureDX11::Finalize()
		{
			FND::SafeRelease(resource);
			FND::SafeRelease(shaderResourceView);
		}

		// テクスチャ設定の取得
		void TextureDX11::GetTextureDesc(TextureDesc* desc)
		{
			D3D11_TEXTURE2D_DESC texture2dDesc = {};
			if (texture2d)
			{
				texture2d->GetDesc(&texture2dDesc);
			}
			else
			{
				HRESULT hr = S_OK;
				shaderResourceView->GetResource(&resource);
				hr = resource->QueryInterface<ID3D11Texture2D>(&texture2d);
				if (FAILED(hr))
				{
					BREAD_LOG_GRP_ERROR("QueryInterface() : Failed!!\n");
					return;
				}
				texture2d->GetDesc(&texture2dDesc);
			}

			desc->width = texture2dDesc.Width;
			desc->height = texture2dDesc.Height;
			desc->arraySize = texture2dDesc.ArraySize;
			desc->mipLevels = texture2dDesc.MipLevels;
		}

		// テクスチャ設定の取得
		void TextureDX11::GetShaderResourceViewDesc(TextureDescDx* desc)
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
			shaderResourceView->GetDesc(&shaderResourceViewDesc);

			desc->format = GetFormat(shaderResourceViewDesc.Format);
			desc->dimension = GetShaderResourceDimension(shaderResourceViewDesc.ViewDimension);
		}

		// テクスチャの読み込み
		bool TextureDX11::LoadTextureFromFile
		(
			ID3D11Device* device,
			const char* filename,
			ID3D11ShaderResourceView** shaderResourceView
		)
		{
			HRESULT hr = S_OK;
			ID3D11Resource* resource;

			static std::map<std::string, ID3D11ShaderResourceView*> cache;

			auto it = cache.find(filename);
			if (it != cache.end())
			{
				*shaderResourceView = it->second;
				(*shaderResourceView)->AddRef();
				(*shaderResourceView)->GetResource(&resource);
			}
			else
			{
				/*hr = DirectX::CreateWICTextureFromFile(device, fileName, resource.GetAddressOf(), shaderResourceView);
				assert( !hr && "DirectX::CreateWICTextureFromFile	Error" );*/

				char ext[32];
				_splitpath_s(filename, NULL, NULL, NULL, NULL, NULL, NULL, ext, sizeof(ext));

				wchar_t _fileName[256];

				MultiByteToWideChar(CP_ACP, 0, filename, static_cast<int>(strlen(filename) + 1), _fileName, 256);

				DirectX::TexMetadata metaData;
				DirectX::ScratchImage image;
				if (strcmp(ext, ".png") == 0 || strcmp(ext, ".bmp") == 0)
				{
					hr = DirectX::LoadFromWICFile(_fileName, 0, &metaData, image);
					if (FAILED(hr))
					{
						BREAD_LOG_GRP_ERROR("DirectX::LoadFromWICFile() : Failed!!\n");
						return false;
					}
				}
				else if (strcmp(ext, ".tga") == 0)
				{
					hr = DirectX::LoadFromTGAFile(_fileName, &metaData, image);
					if (FAILED(hr))
					{
						BREAD_LOG_GRP_ERROR("DirectX::LoadFromTGAFile() : Failed!!\n");
						return false;
					}
				}
				else if (strcmp(ext, ".dds") == 0)
				{
					hr = DirectX::LoadFromDDSFile(_fileName, 0, &metaData, image);
					if (FAILED(hr))
					{
						BREAD_LOG_GRP_ERROR("DirectX::LoadFromDDSFile() : Failed!!\n");
						return false;
					}
				}

				hr = DirectX::CreateShaderResourceViewEx
				(
					device,
					image.GetImages(),
					image.GetImageCount(),
					metaData,
					D3D11_USAGE_DEFAULT,
					D3D11_BIND_SHADER_RESOURCE,
					0,
					D3D11_RESOURCE_MISC_TEXTURECUBE,
					true,
					shaderResourceView
				);
				if (FAILED(hr))
				{
					BREAD_LOG_GRP_ERROR("DirectX::CreateShaderResourceViewEx() : Failed!!\n");
					return false;
				}

				cache.insert(std::make_pair(filename, *shaderResourceView));
			}

			return true;
		}

		// ダミーテクスチャの生成
		bool TextureDX11::CreateDummyTexture
		(
			ID3D11Device* device,
			ID3D11ShaderResourceView** shaderResourceView,
			unsigned int value/*0xAABBGGRR*/,
			unsigned int dimensions,
			bool forceSrgb,
			bool enableCaching
		)
		{
			std::stringstream filename;
			filename << "dummyTexture" << "." << std::hex << std::uppercase << value << "." << dimensions << "." << forceSrgb;

			wchar_t filenameW[256];
			MultiByteToWideChar(CP_ACP, 0, filename.str().c_str(), static_cast<int>(filename.str().length() + 1), filenameW, 256);

			static std::map<std::wstring, ID3D11ShaderResourceView*> cachedTextures;

			std::map<std::wstring, ID3D11ShaderResourceView*>::iterator it = cachedTextures.find(filenameW);
			if (it != cachedTextures.end())
			{
				*shaderResourceView = it->second;
				(*shaderResourceView)->AddRef();
			}
			else
			{
				HRESULT hr = S_OK;

				D3D11_TEXTURE2D_DESC texture2d_desc = {};
				texture2d_desc.Width = dimensions;
				texture2d_desc.Height = dimensions;
				texture2d_desc.MipLevels = 1;
				texture2d_desc.ArraySize = 1;
				texture2d_desc.Format = forceSrgb ? DXGI_FORMAT_R8G8B8A8_UNORM_SRGB : DXGI_FORMAT_R8G8B8A8_UNORM;
				texture2d_desc.SampleDesc.Count = 1;
				texture2d_desc.SampleDesc.Quality = 0;
				texture2d_desc.Usage = D3D11_USAGE_DEFAULT;
				texture2d_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
				texture2d_desc.CPUAccessFlags = 0;
				texture2d_desc.MiscFlags = 0;

				unsigned int* sys_mem = new unsigned int[dimensions * dimensions];
				for (unsigned int i = 0; i < dimensions * dimensions; i++)
				{
					sys_mem[i] = value;
				}
				D3D11_SUBRESOURCE_DATA subresource_data = {};
				subresource_data.pSysMem = sys_mem;
				subresource_data.SysMemPitch = sizeof(unsigned int) * dimensions;
				subresource_data.SysMemSlicePitch = 0;

				ID3D11Texture2D* texture2d;
				hr = device->CreateTexture2D(&texture2d_desc, &subresource_data, &texture2d);
				if (FAILED(hr))
				{
					BREAD_LOG_GRP_ERROR("DirectX::CreateShaderResourceView() : Failed!!\n");
					return false;
				}

				D3D11_SHADER_RESOURCE_VIEW_DESC shader_resource_view_desc = {};
				shader_resource_view_desc.Format = texture2d_desc.Format;
				shader_resource_view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				shader_resource_view_desc.Texture2D.MipLevels = 1;

				hr = device->CreateShaderResourceView(texture2d, &shader_resource_view_desc, shaderResourceView);
				//_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
				if (FAILED(hr))
				{
					BREAD_LOG_GRP_ERROR("DirectX::CreateShaderResourceView() : Failed!!\n");
					return false;
				}

				if (enableCaching)
				{
					cachedTextures.insert(std::make_pair(filenameW, *shaderResourceView));
				}

				texture2d->Release();
				delete[] sys_mem;
			}

			return true;
		}

		// DXGIフォーマット取得
		DXGI_FORMAT TextureDX11::GetDXGIFormat(TextureFormat format)
		{
			switch (format)
			{
			case TextureFormat::R8G8B8A8:		return DXGI_FORMAT_R8G8B8A8_UNORM;
			case TextureFormat::B8G8R8A8:		return DXGI_FORMAT_B8G8R8A8_UNORM;
			case TextureFormat::R24G8:			return DXGI_FORMAT_R24G8_TYPELESS;
			case TextureFormat::R32:			return DXGI_FORMAT_R32_TYPELESS;
			case TextureFormat::B4G4R4A4:		return DXGI_FORMAT_B4G4R4A4_UNORM;
			case TextureFormat::B5G5R5A1:		return DXGI_FORMAT_B5G5R5A1_UNORM;
			case TextureFormat::B5G6R5:			return DXGI_FORMAT_B5G6R5_UNORM;
			case TextureFormat::R16:			return DXGI_FORMAT_R16_TYPELESS;
			case TextureFormat::A8:				return DXGI_FORMAT_A8_UNORM;
			case TextureFormat::R8:				return DXGI_FORMAT_R8_UNORM;
			case TextureFormat::BC1:			return DXGI_FORMAT_BC1_UNORM;
			case TextureFormat::BC2:			return DXGI_FORMAT_BC2_UNORM;
			case TextureFormat::BC3:			return DXGI_FORMAT_BC3_UNORM;
			}
			return DXGI_FORMAT_UNKNOWN;
		}

		// D3Dリソース次元取得
		D3D11_RESOURCE_DIMENSION TextureDX11::GetD3DResourceDimension(TextureDimension dimension)
		{
			switch (dimension)
			{
			case TextureDimension::Tex1D:		return D3D11_RESOURCE_DIMENSION_TEXTURE1D;
			case TextureDimension::Tex2D:		return D3D11_RESOURCE_DIMENSION_TEXTURE2D;
			case TextureDimension::Tex3D:		return D3D11_RESOURCE_DIMENSION_TEXTURE3D;
			case TextureDimension::Cube:		return D3D11_RESOURCE_DIMENSION_TEXTURE2D;
			}

			return D3D11_RESOURCE_DIMENSION_UNKNOWN;
		}

		// フォーマット取得
		TextureFormat TextureDX11::GetFormat(DXGI_FORMAT format)
		{
			switch (format)
			{
			case DXGI_FORMAT_R8G8B8A8_UNORM:	return TextureFormat::R8G8B8A8;
			case DXGI_FORMAT_B8G8R8A8_UNORM:	return TextureFormat::B8G8R8A8;
			case DXGI_FORMAT_R24G8_TYPELESS:	return TextureFormat::R24G8;
			case DXGI_FORMAT_R32_TYPELESS:		return TextureFormat::R32;
			case DXGI_FORMAT_B4G4R4A4_UNORM:	return TextureFormat::B4G4R4A4;
			case DXGI_FORMAT_B5G5R5A1_UNORM:	return TextureFormat::B5G5R5A1;
			case DXGI_FORMAT_B5G6R5_UNORM:		return TextureFormat::B5G6R5;
			case DXGI_FORMAT_R16_TYPELESS:		return TextureFormat::R16;
			case DXGI_FORMAT_A8_UNORM:			return TextureFormat::A8;
			case DXGI_FORMAT_R8_UNORM:			return TextureFormat::R8;
			case DXGI_FORMAT_BC1_UNORM:			return TextureFormat::BC1;
			case DXGI_FORMAT_BC2_UNORM:			return TextureFormat::BC2;
			case DXGI_FORMAT_BC3_UNORM:			return TextureFormat::BC3;
			}
			return TextureFormat::UNKNOWN;
		}

		// リソース次元取得
		TextureDimension TextureDX11::GetResourceDimension(D3D11_SRV_DIMENSION dimension)
		{
			switch (dimension)
			{
			case D3D11_RESOURCE_DIMENSION_TEXTURE1D:	return TextureDimension::Tex1D;
			case D3D11_RESOURCE_DIMENSION_TEXTURE2D:	return TextureDimension::Tex2D;
			case D3D11_RESOURCE_DIMENSION_TEXTURE3D:	return TextureDimension::Tex3D;
			}

			return TextureDimension::Cube;
		}

		// シェーダーリソース次元取得
		TextureDimensionDx TextureDX11::GetShaderResourceDimension(D3D11_SRV_DIMENSION dimension)
		{
			switch (dimension)
			{
			case D3D_SRV_DIMENSION_UNKNOWN:				return TextureDimensionDx::SRV_DIMENSION_UNKNOWN;
			case D3D_SRV_DIMENSION_BUFFER:				return TextureDimensionDx::SRV_DIMENSION_BUFFER;
			case D3D_SRV_DIMENSION_TEXTURE1D:			return TextureDimensionDx::SRV_DIMENSION_TEXTURE1D;
			case D3D_SRV_DIMENSION_TEXTURE1DARRAY:		return TextureDimensionDx::SRV_DIMENSION_TEXTURE1DARRAY;
			case D3D_SRV_DIMENSION_TEXTURE2D:			return TextureDimensionDx::SRV_DIMENSION_TEXTURE2D;
			case D3D_SRV_DIMENSION_TEXTURE2DARRAY:		return TextureDimensionDx::SRV_DIMENSION_TEXTURE2DARRAY;
			case D3D_SRV_DIMENSION_TEXTURE2DMS:			return TextureDimensionDx::SRV_DIMENSION_TEXTURE2DMS;
			case D3D_SRV_DIMENSION_TEXTURE2DMSARRAY:	return TextureDimensionDx::SRV_DIMENSION_TEXTURE2DMSARRAY;
			case D3D_SRV_DIMENSION_TEXTURE3D:			return TextureDimensionDx::SRV_DIMENSION_TEXTURE3D;
			case D3D_SRV_DIMENSION_TEXTURECUBE:			return TextureDimensionDx::SRV_DIMENSION_TEXTURECUBE;
			case D3D_SRV_DIMENSION_TEXTURECUBEARRAY:	return TextureDimensionDx::SRV_DIMENSION_TEXTURECUBEARRAY;
			case D3D_SRV_DIMENSION_BUFFEREX:			return TextureDimensionDx::SRV_DIMENSION_BUFFEREX;
			}

			return TextureDimensionDx::SRV_DIMENSION_UNKNOWN;
		}

		// サーフェイス情報取得
		UINT TextureDX11::BitsPerPixel(DXGI_FORMAT dxgiFormat)
		{
			switch (dxgiFormat)
			{
			case DXGI_FORMAT_R32G32B32A32_TYPELESS:
			case DXGI_FORMAT_R32G32B32A32_FLOAT:
			case DXGI_FORMAT_R32G32B32A32_UINT:
			case DXGI_FORMAT_R32G32B32A32_SINT:
				return 128;

			case DXGI_FORMAT_R32G32B32_TYPELESS:
			case DXGI_FORMAT_R32G32B32_FLOAT:
			case DXGI_FORMAT_R32G32B32_UINT:
			case DXGI_FORMAT_R32G32B32_SINT:
				return 96;

			case DXGI_FORMAT_R16G16B16A16_TYPELESS:
			case DXGI_FORMAT_R16G16B16A16_FLOAT:
			case DXGI_FORMAT_R16G16B16A16_UNORM:
			case DXGI_FORMAT_R16G16B16A16_UINT:
			case DXGI_FORMAT_R16G16B16A16_SNORM:
			case DXGI_FORMAT_R16G16B16A16_SINT:
			case DXGI_FORMAT_R32G32_TYPELESS:
			case DXGI_FORMAT_R32G32_FLOAT:
			case DXGI_FORMAT_R32G32_UINT:
			case DXGI_FORMAT_R32G32_SINT:
			case DXGI_FORMAT_R32G8X24_TYPELESS:
			case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
			case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
			case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
			case DXGI_FORMAT_Y416:
			case DXGI_FORMAT_Y210:
			case DXGI_FORMAT_Y216:
				return 64;

			case DXGI_FORMAT_R10G10B10A2_TYPELESS:
			case DXGI_FORMAT_R10G10B10A2_UNORM:
			case DXGI_FORMAT_R10G10B10A2_UINT:
			case DXGI_FORMAT_R11G11B10_FLOAT:
			case DXGI_FORMAT_R8G8B8A8_TYPELESS:
			case DXGI_FORMAT_R8G8B8A8_UNORM:
			case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
			case DXGI_FORMAT_R8G8B8A8_UINT:
			case DXGI_FORMAT_R8G8B8A8_SNORM:
			case DXGI_FORMAT_R8G8B8A8_SINT:
			case DXGI_FORMAT_R16G16_TYPELESS:
			case DXGI_FORMAT_R16G16_FLOAT:
			case DXGI_FORMAT_R16G16_UNORM:
			case DXGI_FORMAT_R16G16_UINT:
			case DXGI_FORMAT_R16G16_SNORM:
			case DXGI_FORMAT_R16G16_SINT:
			case DXGI_FORMAT_R32_TYPELESS:
			case DXGI_FORMAT_D32_FLOAT:
			case DXGI_FORMAT_R32_FLOAT:
			case DXGI_FORMAT_R32_UINT:
			case DXGI_FORMAT_R32_SINT:
			case DXGI_FORMAT_R24G8_TYPELESS:
			case DXGI_FORMAT_D24_UNORM_S8_UINT:
			case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
			case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
			case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
			case DXGI_FORMAT_R8G8_B8G8_UNORM:
			case DXGI_FORMAT_G8R8_G8B8_UNORM:
			case DXGI_FORMAT_B8G8R8A8_UNORM:
			case DXGI_FORMAT_B8G8R8X8_UNORM:
			case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
			case DXGI_FORMAT_B8G8R8A8_TYPELESS:
			case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
			case DXGI_FORMAT_B8G8R8X8_TYPELESS:
			case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
			case DXGI_FORMAT_AYUV:
			case DXGI_FORMAT_Y410:
			case DXGI_FORMAT_YUY2:
				return 32;

			case DXGI_FORMAT_P010:
			case DXGI_FORMAT_P016:
				return 24;

			case DXGI_FORMAT_R8G8_TYPELESS:
			case DXGI_FORMAT_R8G8_UNORM:
			case DXGI_FORMAT_R8G8_UINT:
			case DXGI_FORMAT_R8G8_SNORM:
			case DXGI_FORMAT_R8G8_SINT:
			case DXGI_FORMAT_R16_TYPELESS:
			case DXGI_FORMAT_R16_FLOAT:
			case DXGI_FORMAT_D16_UNORM:
			case DXGI_FORMAT_R16_UNORM:
			case DXGI_FORMAT_R16_UINT:
			case DXGI_FORMAT_R16_SNORM:
			case DXGI_FORMAT_R16_SINT:
			case DXGI_FORMAT_B5G6R5_UNORM:
			case DXGI_FORMAT_B5G5R5A1_UNORM:
			case DXGI_FORMAT_A8P8:
			case DXGI_FORMAT_B4G4R4A4_UNORM:
				return 16;

			case DXGI_FORMAT_NV12:
			case DXGI_FORMAT_420_OPAQUE:
			case DXGI_FORMAT_NV11:
				return 12;

			case DXGI_FORMAT_R8_TYPELESS:
			case DXGI_FORMAT_R8_UNORM:
			case DXGI_FORMAT_R8_UINT:
			case DXGI_FORMAT_R8_SNORM:
			case DXGI_FORMAT_R8_SINT:
			case DXGI_FORMAT_A8_UNORM:
			case DXGI_FORMAT_AI44:
			case DXGI_FORMAT_IA44:
			case DXGI_FORMAT_P8:
				return 8;

			case DXGI_FORMAT_R1_UNORM:
				return 1;

			case DXGI_FORMAT_BC1_TYPELESS:
			case DXGI_FORMAT_BC1_UNORM:
			case DXGI_FORMAT_BC1_UNORM_SRGB:
			case DXGI_FORMAT_BC4_TYPELESS:
			case DXGI_FORMAT_BC4_UNORM:
			case DXGI_FORMAT_BC4_SNORM:
				return 4;

			case DXGI_FORMAT_BC2_TYPELESS:
			case DXGI_FORMAT_BC2_UNORM:
			case DXGI_FORMAT_BC2_UNORM_SRGB:
			case DXGI_FORMAT_BC3_TYPELESS:
			case DXGI_FORMAT_BC3_UNORM:
			case DXGI_FORMAT_BC3_UNORM_SRGB:
			case DXGI_FORMAT_BC5_TYPELESS:
			case DXGI_FORMAT_BC5_UNORM:
			case DXGI_FORMAT_BC5_SNORM:
			case DXGI_FORMAT_BC6H_TYPELESS:
			case DXGI_FORMAT_BC6H_UF16:
			case DXGI_FORMAT_BC6H_SF16:
			case DXGI_FORMAT_BC7_TYPELESS:
			case DXGI_FORMAT_BC7_UNORM:
			case DXGI_FORMAT_BC7_UNORM_SRGB:
				return 8;

			default:
				return 0;
			}
		}

		// サーフェイス情報取得
		void TextureDX11::GetSurfaceInfo(UINT width, UINT height, DXGI_FORMAT dxgiFormat, UINT* outNumBytes, UINT* outRowBytes, UINT* outNumRows)
		{
			UINT num_bytes = 0;
			UINT row_bytes = 0;
			UINT num_rows = 0;

			bool bc = false;
			bool packed = false;
			bool planar = false;
			UINT bpe = 0;
			switch (dxgiFormat)
			{
			case DXGI_FORMAT_BC1_TYPELESS:
			case DXGI_FORMAT_BC1_UNORM:
			case DXGI_FORMAT_BC1_UNORM_SRGB:
			case DXGI_FORMAT_BC4_TYPELESS:
			case DXGI_FORMAT_BC4_UNORM:
			case DXGI_FORMAT_BC4_SNORM:
				bc = true;
				bpe = 8;
				break;

			case DXGI_FORMAT_BC2_TYPELESS:
			case DXGI_FORMAT_BC2_UNORM:
			case DXGI_FORMAT_BC2_UNORM_SRGB:
			case DXGI_FORMAT_BC3_TYPELESS:
			case DXGI_FORMAT_BC3_UNORM:
			case DXGI_FORMAT_BC3_UNORM_SRGB:
			case DXGI_FORMAT_BC5_TYPELESS:
			case DXGI_FORMAT_BC5_UNORM:
			case DXGI_FORMAT_BC5_SNORM:
			case DXGI_FORMAT_BC6H_TYPELESS:
			case DXGI_FORMAT_BC6H_UF16:
			case DXGI_FORMAT_BC6H_SF16:
			case DXGI_FORMAT_BC7_TYPELESS:
			case DXGI_FORMAT_BC7_UNORM:
			case DXGI_FORMAT_BC7_UNORM_SRGB:
				bc = true;
				bpe = 16;
				break;

			case DXGI_FORMAT_R8G8_B8G8_UNORM:
			case DXGI_FORMAT_G8R8_G8B8_UNORM:
			case DXGI_FORMAT_YUY2:
				packed = true;
				bpe = 4;
				break;

			case DXGI_FORMAT_Y210:
			case DXGI_FORMAT_Y216:
				packed = true;
				bpe = 8;
				break;

			case DXGI_FORMAT_NV12:
			case DXGI_FORMAT_420_OPAQUE:
				planar = true;
				bpe = 2;
				break;

			case DXGI_FORMAT_P010:
			case DXGI_FORMAT_P016:
				planar = true;
				bpe = 4;
				break;
			}

			if (bc)
			{
				UINT num_blocks_wide = 0;
				if (width > 0)
				{
					num_blocks_wide = std::max<UINT>(1, (width + 3) / 4);
				}
				UINT num_blocks_high = 0;
				if (height > 0)
				{
					num_blocks_high = std::max<UINT>(1, (height + 3) / 4);
				}
				row_bytes = num_blocks_wide * bpe;
				num_rows = num_blocks_high;
				num_bytes = row_bytes * num_blocks_high;
			}
			else if (packed)
			{
				row_bytes = ((width + 1) >> 1) * bpe;
				num_rows = height;
				num_bytes = row_bytes * height;
			}
			else if (dxgiFormat == DXGI_FORMAT_NV11)
			{
				row_bytes = ((width + 3) >> 2) * 4;
				num_rows = height * 2; // Direct3D makes this simplifying assumption, although it is larger than the 4:1:1 data
				num_bytes = row_bytes * num_rows;
			}
			else if (planar)
			{
				row_bytes = ((width + 1) >> 1) * bpe;
				num_bytes = (row_bytes * height) + ((row_bytes * height + 1) >> 1);
				num_rows = height + ((height + 1) >> 1);
			}
			else
			{
				UINT bpp = BitsPerPixel(dxgiFormat);
				row_bytes = (width * bpp + 7) / 8; // round up to nearest byte
				num_rows = height;
				num_bytes = row_bytes * height;
			}

			if (outNumBytes)
			{
				*outNumBytes = num_bytes;
			}
			if (outRowBytes)
			{
				*outRowBytes = row_bytes;
			}
			if (outNumRows)
			{
				*outNumRows = num_rows;
			}
		}

		// SRGBフォーマットへ変換
		DXGI_FORMAT TextureDX11::MakeSRGB(DXGI_FORMAT dxgiFormat)
		{
			switch (dxgiFormat)
			{
			case DXGI_FORMAT_R8G8B8A8_UNORM:
				return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

			case DXGI_FORMAT_BC1_UNORM:
				return DXGI_FORMAT_BC1_UNORM_SRGB;

			case DXGI_FORMAT_BC2_UNORM:
				return DXGI_FORMAT_BC2_UNORM_SRGB;

			case DXGI_FORMAT_BC3_UNORM:
				return DXGI_FORMAT_BC3_UNORM_SRGB;

			case DXGI_FORMAT_B8G8R8A8_UNORM:
				return DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;

			case DXGI_FORMAT_B8G8R8X8_UNORM:
				return DXGI_FORMAT_B8G8R8X8_UNORM_SRGB;

			case DXGI_FORMAT_BC7_UNORM:
				return DXGI_FORMAT_BC7_UNORM_SRGB;

			default:
				return dxgiFormat;
			}
		}

		// 初期データを設定する
		HRESULT TextureDX11::FillInitData(
			UINT width,
			UINT height,
			UINT depth,
			UINT mipCount,
			UINT arraySize,
			DXGI_FORMAT dxgiFormat,
			UINT maxsize,
			UINT bitSize,
			const uint8_t* bitData,
			UINT& twidth,
			UINT& theight,
			UINT& tdepth,
			UINT& skipMip,
			D3D11_SUBRESOURCE_DATA* subresourceDatas)
		{
			if (!bitData || !subresourceDatas)
			{
				return E_POINTER;
			}

			skipMip = 0;
			twidth = 0;
			theight = 0;
			tdepth = 0;

			u32 numBytes = 0;
			u32 rowBytes = 0;
			const uint8_t* srcBits = bitData;
			const uint8_t* endBits = bitData + bitSize;

			u32 index = 0;
			for (u32 j = 0; j < arraySize; j++)
			{
				u32 w = width;
				u32 h = height;
				u32 d = depth;
				for (u32 i = 0; i < mipCount; i++)
				{
					GetSurfaceInfo(w,
						h,
						dxgiFormat,
						&numBytes,
						&rowBytes,
						nullptr
					);

					if ((mipCount <= 1) || !maxsize || (w <= maxsize && h <= maxsize && d <= maxsize))
					{
						if (!twidth)
						{
							twidth = w;
							theight = h;
							tdepth = d;
						}

						PHOENIX_ASSERT(index < mipCount * arraySize);
						subresourceDatas[index].pSysMem = (const void*)srcBits;
						subresourceDatas[index].SysMemPitch = static_cast<UINT>(rowBytes);
						subresourceDatas[index].SysMemSlicePitch = static_cast<UINT>(numBytes);
						++index;
					}
					else if (!j)
					{
						// Count number of skipped mipmaps (first item only)
						++skipMip;
					}

					if (srcBits + (numBytes * d) > endBits)
					{
						return E_FAIL;
					}

					srcBits += numBytes * d;

					w = w >> 1;
					h = h >> 1;
					d = d >> 1;
					if (w == 0)
					{
						w = 1;
					}
					if (h == 0)
					{
						h = 1;
					}
					if (d == 0)
					{
						d = 1;
					}
				}
			}

			return (index > 0) ? S_OK : E_FAIL;
		}

		// D3Dリソース作成
		HRESULT TextureDX11::CreateD3DResources(
			ID3D11Device* device,
			D3D11_RESOURCE_DIMENSION resourceDimention,
			UINT width,
			UINT height,
			UINT depth,
			UINT mipCount,
			UINT arraySize,
			DXGI_FORMAT dxgiFormat,
			D3D11_USAGE d3dUsage,
			UINT bindFlags,
			UINT cpuAccessFlags,
			UINT miscFlags,
			bool forceSrgb,
			bool isCubeMap,
			D3D11_SUBRESOURCE_DATA* subresourceDatas,
			ID3D11Resource** resource,
			ID3D11ShaderResourceView** shaderResourceView)
		{
			if (!device)
				return E_POINTER;

			HRESULT hr = E_FAIL;

			if (forceSrgb)
			{
				dxgiFormat = MakeSRGB(dxgiFormat);
			}

			switch (resourceDimention)
			{
			case D3D11_RESOURCE_DIMENSION_TEXTURE1D:
			{
				D3D11_TEXTURE1D_DESC d3dTexture1dDesc;
				d3dTexture1dDesc.Width = static_cast<UINT>(width);
				d3dTexture1dDesc.MipLevels = static_cast<UINT>(mipCount);
				d3dTexture1dDesc.ArraySize = static_cast<UINT>(arraySize);
				d3dTexture1dDesc.Format = dxgiFormat;
				d3dTexture1dDesc.Usage = d3dUsage;
				d3dTexture1dDesc.BindFlags = bindFlags;
				d3dTexture1dDesc.CPUAccessFlags = cpuAccessFlags;
				d3dTexture1dDesc.MiscFlags = miscFlags & ~D3D11_RESOURCE_MISC_TEXTURECUBE;

				ID3D11Texture1D* d3dTexture1d = nullptr;
				hr = device->CreateTexture1D(&d3dTexture1dDesc, subresourceDatas, &d3dTexture1d);
				if (SUCCEEDED(hr) && d3dTexture1d != 0)
				{
					if (shaderResourceView != 0)
					{
						D3D11_SHADER_RESOURCE_VIEW_DESC d3dSrvDesc;
						memset(&d3dSrvDesc, 0, sizeof(d3dSrvDesc));
						d3dSrvDesc.Format = dxgiFormat;

						if (arraySize > 1)
						{
							d3dSrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1DARRAY;
							d3dSrvDesc.Texture1DArray.MipLevels = (!mipCount) ? -1 : d3dTexture1dDesc.MipLevels;
							d3dSrvDesc.Texture1DArray.ArraySize = static_cast<UINT>(arraySize);
						}
						else
						{
							d3dSrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
							d3dSrvDesc.Texture1D.MipLevels = (!mipCount) ? -1 : d3dTexture1dDesc.MipLevels;
						}

						hr = device->CreateShaderResourceView(d3dTexture1d, &d3dSrvDesc, shaderResourceView);
						if (FAILED(hr))
						{
							BREAD_LOG_GRP_ERROR("ID3D11Device::CreateTexture1D() : Failed!!\n");
							d3dTexture1d->Release();
							return hr;
						}
					}

					if (resource != 0)
					{
						*resource = d3dTexture1d;
					}
					else
					{
						SetDebugObjectName(d3dTexture1d, "TextureDX11");
						d3dTexture1d->Release();
					}
				}
			}
			break;

			case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
			{
				D3D11_TEXTURE2D_DESC d3dTexture2dDesc;
				d3dTexture2dDesc.Width = static_cast<UINT>(width);
				d3dTexture2dDesc.Height = static_cast<UINT>(height);
				d3dTexture2dDesc.MipLevels = static_cast<UINT>(mipCount);
				d3dTexture2dDesc.ArraySize = static_cast<UINT>(arraySize);
				d3dTexture2dDesc.Format = dxgiFormat;
				d3dTexture2dDesc.SampleDesc.Count = 1;
				d3dTexture2dDesc.SampleDesc.Quality = 0;
				d3dTexture2dDesc.Usage = d3dUsage;
				d3dTexture2dDesc.BindFlags = bindFlags;
				d3dTexture2dDesc.CPUAccessFlags = cpuAccessFlags;
				if (isCubeMap)
				{
					d3dTexture2dDesc.MiscFlags = miscFlags | D3D11_RESOURCE_MISC_TEXTURECUBE;
				}
				else
				{
					d3dTexture2dDesc.MiscFlags = miscFlags & ~D3D11_RESOURCE_MISC_TEXTURECUBE;
				}

				ID3D11Texture2D* d3dTexture2d = nullptr;
				hr = device->CreateTexture2D(&d3dTexture2dDesc, subresourceDatas, &d3dTexture2d);
				if (SUCCEEDED(hr) && d3dTexture2d != 0)
				{
					if (shaderResourceView != 0)
					{
						D3D11_SHADER_RESOURCE_VIEW_DESC d3dSrvDesc;
						memset(&d3dSrvDesc, 0, sizeof(d3dSrvDesc));
						d3dSrvDesc.Format = dxgiFormat;

						if (isCubeMap)
						{
							if (arraySize > 6)
							{
								d3dSrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBEARRAY;
								d3dSrvDesc.TextureCubeArray.MipLevels = (!mipCount) ? -1 : d3dTexture2dDesc.MipLevels;

								// Earlier we set array_size to (NumCubes * 6)
								d3dSrvDesc.TextureCubeArray.NumCubes = static_cast<UINT>(arraySize / 6);
							}
							else
							{
								d3dSrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
								d3dSrvDesc.TextureCube.MipLevels = (!mipCount) ? -1 : d3dTexture2dDesc.MipLevels;
							}
						}
						else if (arraySize > 1)
						{
							d3dSrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
							d3dSrvDesc.Texture2DArray.MipLevels = (!mipCount) ? -1 : d3dTexture2dDesc.MipLevels;
							d3dSrvDesc.Texture2DArray.ArraySize = static_cast<UINT>(arraySize);
						}
						else
						{
							d3dSrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
							d3dSrvDesc.Texture2D.MipLevels = (!mipCount) ? -1 : d3dTexture2dDesc.MipLevels;
						}

						hr = device->CreateShaderResourceView(d3dTexture2d, &d3dSrvDesc, shaderResourceView);
						if (FAILED(hr))
						{
							BREAD_LOG_GRP_ERROR("ID3D11Device::CreateShaderResourceView() : Failed!!\n");
							d3dTexture2d->Release();
							return hr;
						}
					}

					if (resource != 0)
					{
						*resource = d3dTexture2d;
					}
					else
					{
						SetDebugObjectName(d3dTexture2d, "TextureDX11");
						d3dTexture2d->Release();
					}
				}
			}
			break;

			case D3D11_RESOURCE_DIMENSION_TEXTURE3D:
			{
				D3D11_TEXTURE3D_DESC d3dTexture3dDesc;
				d3dTexture3dDesc.Width = static_cast<UINT>(width);
				d3dTexture3dDesc.Height = static_cast<UINT>(height);
				d3dTexture3dDesc.Depth = static_cast<UINT>(depth);
				d3dTexture3dDesc.MipLevels = static_cast<UINT>(mipCount);
				d3dTexture3dDesc.Format = dxgiFormat;
				d3dTexture3dDesc.Usage = d3dUsage;
				d3dTexture3dDesc.BindFlags = bindFlags;
				d3dTexture3dDesc.CPUAccessFlags = cpuAccessFlags;
				d3dTexture3dDesc.MiscFlags = miscFlags & ~D3D11_RESOURCE_MISC_TEXTURECUBE;

				ID3D11Texture3D* d3dTexture3d = nullptr;
				hr = device->CreateTexture3D(&d3dTexture3dDesc, subresourceDatas, &d3dTexture3d);
				if (SUCCEEDED(hr) && d3dTexture3d != 0)
				{
					if (shaderResourceView != 0)
					{
						D3D11_SHADER_RESOURCE_VIEW_DESC d3dSrvDesc;
						memset(&d3dSrvDesc, 0, sizeof(d3dSrvDesc));
						d3dSrvDesc.Format = dxgiFormat;

						d3dSrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
						d3dSrvDesc.Texture3D.MipLevels = (!mipCount) ? -1 : d3dTexture3dDesc.MipLevels;

						hr = device->CreateShaderResourceView(d3dTexture3d, &d3dSrvDesc, shaderResourceView);
						if (FAILED(hr))
						{
							BREAD_LOG_GRP_ERROR("ID3D11Device::CreateShaderResourceView() : Failed!!\n");
							d3dTexture3d->Release();
							return hr;
						}
					}

					if (resource != 0)
					{
						*resource = d3dTexture3d;
					}
					else
					{
						SetDebugObjectName(d3dTexture3d, "TextureDX11");
						d3dTexture3d->Release();
					}
				}
			}
			break;
			}

			return hr;
		}
	} // namespace Graphics
} // namespace Bread