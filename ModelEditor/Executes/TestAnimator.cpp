#include "stdafx.h"
#include "TestAnimator.h"
#include "Fbx/FbxLoader.h"

void TestAnimator::Initialize()
{
	/*FbxLoader* loader = NULL;
	vector<wstring> clipList;

	loader = new FbxLoader
	(
	Assets + L"Youmu/Youmu.fbx",
	Models + L"Youmu/", L"Youmu"
	);
	loader->ExportMaterial();
	loader->ExportMesh();

	SAFE_DELETE(loader);*/
}

void TestAnimator::Ready()
{
	gameModel = new GameModel
	(
		Shaders + L"046_Model.fx",
		Models + L"Youmu/Youmu.material",
		Models + L"Youmu/Youmu.mesh"
	);
	//gameModel->AddClip(Models + L"Kachujin/Idle.animation");
	gameModel->Ready();

	gameModel->Scale(0.01f, 0.01f, 0.01f);
}

void TestAnimator::Destroy()
{
	
}

void TestAnimator::Update()
{
	gameModel->Update();
}

void TestAnimator::PreRender()
{

}

void TestAnimator::Render()
{
	gameModel->Render();
}