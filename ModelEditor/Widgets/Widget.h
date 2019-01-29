#pragma once
#include "stdafx.h"

class Widget
{
public:
	Widget()
	{
		title = "##";
		xMin = 455;
		yMin = 455;
		xMax = FLT_MAX;
		yMax = FLT_MAX;

		windowFlags = ImGuiWindowFlags_NoCollapse;
		bVisible = true;
	}
	virtual ~Widget() {}

	virtual void Render() = 0;

	virtual void Begin()
	{
		ImGui::Begin(title.c_str(), &bVisible, windowFlags);
	}

	virtual void End()
	{
		ImGui::End();
	}

private:
	string title;

	float xMin, yMin, xMax, yMax;

	int windowFlags;
	bool bVisible;

};