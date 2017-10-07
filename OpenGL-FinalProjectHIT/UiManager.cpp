#include "UiManager.h"

UiManager::UiManager(GLFWwindow* window)
{
	// Setup ImGui binding
	ImGui_ImplGlfwGL3_Init(window, true);
}


UiManager::~UiManager()
{
}

void UiManager::RenderBegin()
{
	
	ImGui_ImplGlfwGL3_NewFrame();

}

void UiManager::RenderSet()
{
	static bool no_titlebar = false;
	static bool no_border = true;
	static bool no_resize = false;
	static bool no_move = false;
	static bool no_scrollbar = false;
	static bool no_collapse = false;
	static bool no_menu = true;
	static bool p_open = true;
	// Demonstrate the various window flags. Typically you would just use the default.
	ImGuiWindowFlags window_flags = 0;
	if (no_titlebar)  window_flags |= ImGuiWindowFlags_NoTitleBar;
	if (!no_border)   window_flags |= ImGuiWindowFlags_ShowBorders;
	if (no_resize)    window_flags |= ImGuiWindowFlags_NoResize;
	if (no_move)      window_flags |= ImGuiWindowFlags_NoMove;
	if (no_scrollbar) window_flags |= ImGuiWindowFlags_NoScrollbar;
	if (no_collapse)  window_flags |= ImGuiWindowFlags_NoCollapse;
	if (!no_menu)     window_flags |= ImGuiWindowFlags_MenuBar;

	
	ImGui::SetNextWindowSize(ImVec2(400, 600));
	ImGui::Begin("Application Settings", &p_open, window_flags);
	
	
	ImGui::Text("Wellcome to the OpenGL Final Project in HIT 2017b");
	ImGui::Text("Author: Jacob Sobloev, ID: 316898162");

	static float f = 0.0f;
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	ImGui::PushItemWidth(-140);
	ImGui::Spacing();

	if (ImGui::TreeNode("Camera"))
	{
		if (ImGui::TreeNode("Position"))
		{
			ImGui::DragFloat("X", &Camera->Position.x, 0.005f);
			ImGui::DragFloat("Y", &Camera->Position.y, 0.005f);
			ImGui::DragFloat("Z", &Camera->Position.z, 0.005f);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Front"))
		{
			ImGui::DragFloat("X", &Camera->Front.x, 0.005f);
			ImGui::DragFloat("Y", &Camera->Front.y, 0.005f);
			ImGui::DragFloat("Z", &Camera->Front.z, 0.005f);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Up"))
		{
			ImGui::DragFloat("X", &Camera->Up.x, 0.005f);
			ImGui::DragFloat("Y", &Camera->Up.y, 0.005f);
			ImGui::DragFloat("Z", &Camera->Up.z, 0.005f);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Zoom"))
		{
			ImGui::DragFloat("", &Camera->Zoom, 0.05f);
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Scene and Lights"))
	{
		if (ImGui::TreeNode("Theme Presets"))
		{
			ImGui::Combo("Preset", &Lights->LightPresets, "None\0Desert\0Factory\0Horror\0Bio Chemical Lab\0\0");
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Global"))
		{
			ImGui::ColorEdit3("clear color", (float*)Lights->ClearColor);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Directional Light"))
		{
			ImGui::Checkbox("Enabled", &Lights->LightDirectionalLight->enabled);
			ImGui::SliderFloat("X", &Lights->LightDirectionalLight->direction.x, -20.0f, 20.0f, "%.4f", 3.0f);
			ImGui::SliderFloat("Y", &Lights->LightDirectionalLight->direction.y, -20.0f, 20.0f, "%.4f", 3.0f);
			ImGui::SliderFloat("Z", &Lights->LightDirectionalLight->direction.z, -20.0f, 20.0f, "%.4f", 3.0f);
			ImGui::ColorEdit3("Ambient", (float*)&Lights->LightDirectionalLight->ambient);
			ImGui::ColorEdit3("Diffuse", (float*)&Lights->LightDirectionalLight->diffuse);
			ImGui::ColorEdit3("Specular", (float*)&Lights->LightDirectionalLight->specular);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Point Lights"))
		{
			static int index = 0;
			ImGui::InputInt("Light Index", &index);
			if (index < 0) {
				index = 0;
			}
			else if (index > Lights->LightPointLights->size() - 1) {
				index = Lights->LightPointLights->size() - 1;
			}
			ImGui::Checkbox("Enabled", &Lights->LightPointLights->at(index)->enabled);
			ImGui::SameLine();
			ImGui::Checkbox("Visiable", &Lights->LightPointLights->at(index)->visiable);
			ImGui::ColorEdit3("Ambient", (float*)&Lights->LightPointLights->at(index)->ambient);
			ImGui::ColorEdit3("Diffuse", (float*)&Lights->LightPointLights->at(index)->diffuse);
			ImGui::ColorEdit3("Specular", (float*)&Lights->LightPointLights->at(index)->specular);
			if (ImGui::TreeNode("Position"))
			{
				ImGui::DragFloat("X", &Lights->LightPointLights->at(index)->position.x, 0.005f);
				ImGui::DragFloat("Y", &Lights->LightPointLights->at(index)->position.y, 0.005f);
				ImGui::DragFloat("Z", &Lights->LightPointLights->at(index)->position.z, 0.005f);
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Flash Light"))
		{
			ImGui::Checkbox("Enabled", &Lights->LightSpotLight->enabled);
			ImGui::ColorEdit3("Ambient", (float*)&Lights->LightSpotLight->ambient);
			ImGui::ColorEdit3("Diffuse", (float*)&Lights->LightSpotLight->diffuse);
			ImGui::ColorEdit3("Specular", (float*)&Lights->LightSpotLight->specular);
			//ImGui::SliderFloat("Constant", &LightSpotLight->constant, -1.0f, 1.0f, "%.4f", 3.0f);
			//ImGui::SliderFloat("Linear", &LightSpotLight->linear, -1.0f, 1.0f, "%.4f", 3.0f);
			//ImGui::SliderFloat("Quadratic", &LightSpotLight->quadratic, -1.0f, 1.0f, "%.4f", 3.0f);
			//ImGui::SliderFloat("CutOff", &LightSpotLight->cutOff, 0.0f, 1.0f, "%.4f", 3.0f);
			//ImGui::SliderFloat("OuterCutOff", &LightSpotLight->outerCutOff, 0.0f, 1.0f, "%.4f", 3.0f);
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Cubes"))
	{
		if (ImGui::TreeNode("All")) 
		{
			ImGui::Checkbox("Animation Enabled", &Cubes->AnimateCubes);
			ImGui::SameLine();
			ImGui::Checkbox("Visiable", &Cubes->VisiableCubes);
			ImGui::Combo("Texture", &Cubes->TextureItem, "Texture1\0Texture2\0Texture3\0\0");
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Single"))
		{
			static int cubeIndex = 0;
			static int cubeTextureIndex = 0;
			ImGui::InputInt("Cube Index", &cubeIndex);
			if (cubeIndex < 0) {
				cubeIndex = 0;
			}
			else if (cubeIndex > Cubes->ObjectsVec->size() - 1) {
				cubeIndex = Cubes->ObjectsVec->size() - 1;
			}
			ImGui::Checkbox("Animation Enabled", &Cubes->ObjectsVec->at(cubeIndex)->animationEnabled);
			ImGui::SameLine();
			ImGui::Checkbox("Visiable", &Cubes->ObjectsVec->at(cubeIndex)->visiable);
			ImGui::Checkbox("Blin", &Cubes->ObjectsVec->at(cubeIndex)->blin);
			ImGui::Combo("Texture", &cubeTextureIndex, "Texture1\0Texture2\0Texture3\0\0");
			ImGui::SameLine();
			if (ImGui::Button("Apply"))
			{
				Cubes->SetTextureToCube(cubeTextureIndex, cubeIndex);
			}
			ImGui::DragFloat("Shininess", &Cubes->ObjectsVec->at(cubeIndex)->material.shininess, 0.005f);
			if (ImGui::TreeNode("Position"))
			{
				ImGui::DragFloat("X", &Cubes->ObjectsVec->at(cubeIndex)->position.x, 0.005f);
				ImGui::DragFloat("Y", &Cubes->ObjectsVec->at(cubeIndex)->position.y, 0.005f);
				ImGui::DragFloat("Z", &Cubes->ObjectsVec->at(cubeIndex)->position.z, 0.005f);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Scale"))
			{
				ImGui::DragFloat("X", &Cubes->ObjectsVec->at(cubeIndex)->scale.x, 0.005f);
				ImGui::DragFloat("Y", &Cubes->ObjectsVec->at(cubeIndex)->scale.y, 0.005f);
				ImGui::DragFloat("Z", &Cubes->ObjectsVec->at(cubeIndex)->scale.z, 0.005f);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Rotation"))
			{
				ImGui::DragFloat("Angle", &Cubes->ObjectsVec->at(cubeIndex)->rotationAngle, 0.005f);
				ImGui::DragFloat("X", &Cubes->ObjectsVec->at(cubeIndex)->rotation.x, 0.005f);
				ImGui::DragFloat("Y", &Cubes->ObjectsVec->at(cubeIndex)->rotation.y, 0.005f);
				ImGui::DragFloat("Z", &Cubes->ObjectsVec->at(cubeIndex)->rotation.z, 0.005f);
				ImGui::TreePop();
			}


			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Floor"))
	{
		static int index = 0;
		ImGui::Checkbox("Blin", &Floor->ObjectsVec->at(index)->blin);
		ImGui::DragFloat("Shininess", &Floor->ObjectsVec->at(index)->material.shininess, 0.005f);
		if (ImGui::TreeNode("Position"))
		{
			ImGui::DragFloat("X", &Floor->ObjectsVec->at(index)->position.x, 0.005f);
			ImGui::DragFloat("Y", &Floor->ObjectsVec->at(index)->position.y, 0.005f);
			ImGui::DragFloat("Z", &Floor->ObjectsVec->at(index)->position.z, 0.005f);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Scale"))
		{
			ImGui::DragFloat("X", &Floor->ObjectsVec->at(index)->scale.x, 0.005f);
			ImGui::DragFloat("Y", &Floor->ObjectsVec->at(index)->scale.y, 0.005f);
			ImGui::DragFloat("Z", &Floor->ObjectsVec->at(index)->scale.z, 0.005f);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Rotation"))
		{
			ImGui::DragFloat("Angle", &Floor->ObjectsVec->at(index)->rotationAngle, 0.005f);
			ImGui::DragFloat("X", &Floor->ObjectsVec->at(index)->rotation.x, 0.005f);
			ImGui::DragFloat("Y", &Floor->ObjectsVec->at(index)->rotation.y, 0.005f);
			ImGui::DragFloat("Z", &Floor->ObjectsVec->at(index)->rotation.z, 0.005f);
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
	
	ImGui::End();
	
	
}

void UiManager::RenderEnd()
{
	ImGui::Render();
}

void UiManager::CleanUp()
{
	ImGui_ImplGlfwGL3_Shutdown();
}
