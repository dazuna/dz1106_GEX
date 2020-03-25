#include "ImGUI_utils.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <glm/vec3.hpp>
#include <iostream>

/*
 * Esto esta aqui mientras porque no puedo incluir tools y
 * no se donde ponerlo
 */
bool jsonContains(nlohmann::json object, std::string key)
{
	return object.find(key) != object.end();
}

vJSON ImGUI_utils::jsonsToDisplay;
GLFWwindow* ImGUI_utils::window;

void ImGUI_utils::init(GLFWwindow *window)
{
	ImGUI_utils::window = window;
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	io.Fonts->AddFontFromFileTTF("./assets/fonts/dejavu/DejaVuSans.ttf", 30.0f);

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 420");
}

bool show_demo_window = true,
	show_another_window = false;
glm::vec3 color(1.0, 0.4, 0.0);



void ImGUI_utils::render()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	//ImGui::ShowDemoWindow();

	// Everything has to be called for render each frame and on this "context"
	// So that's why there's a function to add jsons to render and another
	// function to actually render them

	// Render jsons info
	for (auto &data : jsonsToDisplay)
	{
		renderJSON(data);
	}
	jsonsToDisplay.clear();

	renderMenu();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGUI_utils::displayJSON(nlohmann::json data)
{
	jsonsToDisplay.push_back(data);
}

void ImGUI_utils::renderJSON(nlohmann::json data)
{
	if (!data.is_object()) return;

	int flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

	if (jsonContains(data, "title"))
	{
		ImGui::Begin(data["title"].get<std::string>().c_str(), nullptr, flags);
		data.erase("title");
	}
	else
	{
		ImGui::Begin("the title is a lie", nullptr, flags | ImGuiWindowFlags_NoTitleBar);
	}
	
	if (jsonContains(data, "pos"))
	{
		ImGui::SetWindowPos(ImVec2(data["pos"][0], data["pos"][1]));
		data.erase("pos");
	}

	if (jsonContains(data, "size"))
	{
		ImGui::SetWindowSize(ImVec2(data["size"][0], data["size"][1]));
		data.erase("size");
	}

	for (auto& key_value : data.items())
	{
		auto entry = key_value.value();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), key_value.key().c_str());
		ImGui::SameLine(0, 20);
		if (entry.is_string())
		{
			ImGui::Text(key_value.value().get<std::string>().c_str());
		}
		if (entry.is_number_integer())
		{
			ImGui::Text(std::to_string(key_value.value().get<int>()).c_str());
		}
		if (entry.is_number_float())
		{
			ImGui::Text(std::to_string(key_value.value().get<float>()).c_str());
		}
		
	}

	ImGui::End();
}

void ImGUI_utils::renderMenu()
{
	int flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	ImGui::Begin("Actions", nullptr, flags);
	int screenWidth, screenHeight;
	int width = 200;
	int height = 150;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
	ImGui::SetWindowPos(ImVec2(screenWidth - width, screenHeight - height));
	ImGui::SetWindowSize(ImVec2(width,  height));
	// Si necesitamos hacer el menú más dinámico, podemos agregar variables estáticas
	// para aparecer/desaparecer los botones
	ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.9);
	bool movedThisFrame = ImGui::Button("Move");
	ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * 0.9);
	bool attackedThisFrame = ImGui::Button("Attack");

	// Podemos cambiar esto a que los botones actualicen variables estaticas que
	// sean leidas por otras cosas o que directamente esta función llame a
	// funciones para manejar que los botones sean picados
	if (movedThisFrame)
	{
		std::cout << "Move action was pressed" << std::endl;
	}

	if (attackedThisFrame)
	{
		std::cout << "Attack!!" << std::endl;
	}
	ImGui::End();
}
