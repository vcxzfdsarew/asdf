#include "framework.h"
#include "Render2D.h"

Render2D::Render2D(wstring shaderFile)
	: position(0, 0), scale(1, 1), pass(0)
{
	String::Replace(&shaderFile, Shaders, L"");

	wstring file = L"";
	if (shaderFile.length() > 1)
		file = Shaders + shaderFile;
	else
		file = Shaders + L"026_Render2D.fx";

	shader = new Shader(file);


	//Create Vertex Buffer
	{
		VertexTexture* vertices = new VertexTexture[6];
		vertices[0].Position = D3DXVECTOR3(-0.5f, -0.5f, 0.0f);
		vertices[1].Position = D3DXVECTOR3(-0.5f, 0.5f, 0.0f);
		vertices[2].Position = D3DXVECTOR3(0.5f, -0.5f, 0.0f);
		vertices[3].Position = D3DXVECTOR3(0.5f, -0.5f, 0.0f);
		vertices[4].Position = D3DXVECTOR3(-0.5f, 0.5f, 0.0f);
		vertices[5].Position = D3DXVECTOR3(0.5f, 0.5f, 0.0f);

		vertices[0].Uv = D3DXVECTOR2(0, 1);
		vertices[1].Uv = D3DXVECTOR2(0, 0);
		vertices[2].Uv = D3DXVECTOR2(1, 1);
		vertices[3].Uv = D3DXVECTOR2(1, 1);
		vertices[4].Uv = D3DXVECTOR2(0, 0);
		vertices[5].Uv = D3DXVECTOR2(1, 0);


		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(VertexTexture) * 6;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = vertices;

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
		assert(SUCCEEDED(hr));

		SAFE_DELETE_ARRAY(vertices);
	}

	D3DXMatrixLookAtLH
	(
		&view, &D3DXVECTOR3(0, 0, -1), &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 1, 0)
	);

	//Create Ortho Matrix
	{
		D3DDesc desc;
		D3D::GetDesc(&desc);

		D3DXMatrixOrthoOffCenterLH(&orthographic, 0, desc.Width, 0, desc.Height, -1, 1);
	}

	shader->AsMatrix("View2")->SetMatrix(view);
	shader->AsMatrix("Projection2")->SetMatrix(orthographic);

	UpdateWorld();
}

Render2D::~Render2D()
{
	SAFE_DELETE(shader);

	SAFE_RELEASE(vertexBuffer);
}

void Render2D::Position(float x, float y)
{
	Position(D3DXVECTOR2(x, y));
}

void Render2D::Position(D3DXVECTOR2 vec)
{
	position = vec;

	UpdateWorld();
}

void Render2D::Scale(float x, float y)
{
	Scale(D3DXVECTOR2(x, y));
}

void Render2D::Scale(D3DXVECTOR2 vec)
{
	scale = vec;

	UpdateWorld();
}

void Render2D::Update()
{

}

void Render2D::Render()
{
	UINT stride = sizeof(VertexTexture);
	UINT offset = 0;

	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	shader->Draw(0, pass, 6);
}

void Render2D::SRV(ID3D11ShaderResourceView * srv)
{
	shader->AsShaderResource("Map")->SetResource(srv);
}

void Render2D::UpdateWorld()
{
	D3DXMATRIX S, T, W;
	D3DXMatrixScaling(&S, scale.x, scale.y, 1);
	D3DXMatrixTranslation
	(
		&T, position.x + scale.x * 0.5f, position.y + scale.y * 0.5f, 0
	);

	W = S * T;
	shader->AsMatrix("World")->SetMatrix(W);
}
