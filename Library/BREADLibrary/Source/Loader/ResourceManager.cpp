#include "pch.h"
#include <string>
#include "ResourceManager.h"
#include <d3dcompiler.h>

#if defined( DEBUG ) || defined( _DEBUG )
#define _ASSERT_EXPR_A(expr, msg) \
	(void)((!!(expr)) || \
	(1 != _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, "%s", msg)) || \
	(_CrtDbgBreak(), 0))
#else
#define  _ASSERT_EXPR_A(expr, expr_str) ((void)0)
#endif

inline LPWSTR hr_trace(HRESULT hr)
{
	LPWSTR msg;
	FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPWSTR>(&msg), 0, NULL);
	return msg;
}


void ResourceManager::CreateVertexShaderAndInputLayout
  ( ID3D11Device *device,
	const char *csoName,
	ID3D11VertexShader **vertexShader,
	ID3D11InputLayout **inputLayout,
	D3D11_INPUT_ELEMENT_DESC *inputElementDesc,
	UINT numElements )
{

	HRESULT hr = S_OK;


	struct SetOfVertexShaderAndInputLayout
	{
		SetOfVertexShaderAndInputLayout( ID3D11VertexShader  *_vertexShader, ID3D11InputLayout *_inputLayout ) : _vertexShader( _vertexShader ), _inputLayout( _inputLayout ) {}
		Microsoft::WRL::ComPtr<ID3D11VertexShader> _vertexShader;
		Microsoft::WRL::ComPtr<ID3D11InputLayout>  _inputLayout;
	};


	static std::map<std::string, SetOfVertexShaderAndInputLayout> cache;


	auto it = cache.find( csoName );
	if ( it != cache.end() )
	{
		*vertexShader = it->second._vertexShader.Get();
		( *vertexShader )->AddRef();
		*inputLayout = it->second._inputLayout.Get();
		if ((*inputLayout)) ( *inputLayout )->AddRef();
		return;
	}


	//Load CSO (VS)
	FILE* fp = 0;
	fopen_s( &fp, csoName, "rb" );

	
	fseek( fp, 0, SEEK_END );
	long csoSize = ftell( fp );
	fseek( fp, 0, SEEK_SET );

	
	std::unique_ptr<unsigned char[]> csoData = std::make_unique<unsigned char[]>( csoSize );
	fread( csoData.get(), csoSize, 1, fp );
	fclose( fp );


	hr = device->CreateVertexShader( csoData.get(), csoSize, NULL, vertexShader );
	assert(!hr);

	if (inputElementDesc)
	{
		hr = device->CreateInputLayout(inputElementDesc, numElements, csoData.get(), csoSize, inputLayout);
		assert(!hr);
	}


	cache.insert( std::make_pair( csoName, SetOfVertexShaderAndInputLayout( *vertexShader, *inputLayout ) ) );

}

void ResourceManager::CreateGeometryShader
 (  ID3D11Device* device,
	const char* csoName,
	ID3D11GeometryShader** geometryShader )
{
	
	HRESULT hr = S_OK;


	static std::map<std::string, Microsoft::WRL::ComPtr<ID3D11GeometryShader>> cache;


	auto it = cache.find( csoName );
	if ( it != cache.end() )
	{
		*geometryShader = it->second.Get();
		( *geometryShader )->AddRef();
		return;
	}


	//Load CSO (ps)
	FILE* fp = 0;
	fopen_s( &fp, csoName, "rb" );


	fseek( fp, 0, SEEK_END );
	long csoSize = ftell( fp );
	fseek( fp, 0, SEEK_SET );


	std::unique_ptr<unsigned char[]> csoData = std::make_unique<unsigned char[]>( csoSize );
	fread( csoData.get(), csoSize, 1, fp );
	fclose( fp );

	fp = nullptr;


	hr = device->CreateGeometryShader( csoData.get(), csoSize, NULL, geometryShader );
	assert(!hr);


	cache.insert( std::make_pair( csoName, *geometryShader ) );

}

void ResourceManager::CreatePixelShader
  ( ID3D11Device *device,
	const char *csoName,
	ID3D11PixelShader **pixelShader )
{

	HRESULT hr = S_OK;


	static std::map<std::string, Microsoft::WRL::ComPtr<ID3D11PixelShader>> cache;


	auto it = cache.find( csoName );
	if ( it != cache.end() )
	{
		*pixelShader = it->second.Get();
		( *pixelShader )->AddRef();
		return;
	}


	//Load CSO (ps)
	FILE* fp = 0;
	fopen_s( &fp, csoName, "rb" );


	fseek( fp, 0, SEEK_END );
	long csoSize = ftell( fp );
	fseek( fp, 0, SEEK_SET );


	std::unique_ptr<unsigned char[]> csoData = std::make_unique<unsigned char[]>( csoSize );
	fread( csoData.get(), csoSize, 1, fp );
	fclose( fp );

	fp = nullptr;


	hr = device->CreatePixelShader( csoData.get(), csoSize, NULL, pixelShader );
	assert(!hr);


	cache.insert( std::make_pair( csoName, *pixelShader ) );

}

void ResourceManager::CreateComputeShader
  ( ID3D11Device *device,
	const char *csoName,
	ID3D11ComputeShader **computeShader )
{

	HRESULT hr = S_OK;


	static std::map<std::string, Microsoft::WRL::ComPtr<ID3D11ComputeShader>> cache;


	auto it = cache.find( csoName );
	if ( it != cache.end() )
	{
		*computeShader = it->second.Get();
		( *computeShader )->AddRef();
		return;
	}


	//Load CSO (ps)
	FILE* fp = 0;
	fopen_s( &fp, csoName, "rb" );


	fseek( fp, 0, SEEK_END );
	long csoSize = ftell( fp );
	fseek( fp, 0, SEEK_SET );


	std::unique_ptr<unsigned char[]> csoData = std::make_unique<unsigned char[]>( csoSize );
	fread( csoData.get(), csoSize, 1, fp );
	fclose( fp );

	fp = nullptr;


	hr = device->CreateComputeShader( csoData.get(), csoSize, NULL, computeShader );
	assert(!hr);


	cache.insert( std::make_pair( csoName, *computeShader ) );

}