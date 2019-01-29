#include "stdafx.h"
#include "TestEditor.h"

void TestEditor::Initialize()
{
	bWindow = true;
	color = D3DXCOLOR(1, 1, 1, 1);
}

void TestEditor::Ready()
{

}

void TestEditor::Destroy()
{
	
}

void TestEditor::Update()
{

}

void TestEditor::PreRender()
{

}

void TestEditor::Render()
{
	if (bWindow)
	{
		ImGui::Begin("Environment", &bWindow, ImGuiWindowFlags_MenuBar);

		if (ImGui::BeginMenuBar())
		{
			ImGui::ColorEdit3("color", (float*)color);

			if (ImGui::Button("Close"))
				bWindow = false;

			ImGui::End();
			ImGui::EndMenuBar();
		}
	}

	if(ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open Window"))
				bWindow = true;
			if (ImGui::MenuItem("Close Window"))
				bWindow = false;

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Test"))
			{
				D3DDesc desc;
				D3D::GetDesc(&desc);

				MessageBox(desc.Handle, L"테스트", L"법규", MB_OK);
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}