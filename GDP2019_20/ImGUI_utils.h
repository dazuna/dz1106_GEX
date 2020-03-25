#pragma once
#include <GLFW/glfw3.h>
#include <nlohmann/json.hpp>

typedef std::vector<nlohmann::json> vJSON;

struct ImGUI_utils
{
	static void init(GLFWwindow *window);
	static void render();
	/*
	 * Creates a window and displays the info of a json object in it.
	 * `data` can carry specific fields to control how the window looks:
	 * `pos` two-number vector in pixels
	 * `size` two-number vector in pixels
	 * `title` string to set a title to the window. It acts as the name of the
	 * window. If repeated, the both json will be displayed in the same window
	 *
	 * The rest of the fields will be shown with the name of the field
	 * to the left and the value to the right
	 *
	 * Must be called every frame you want the json to be displayed
	 * (we can make this different if needed)
	 */
	static void displayJSON(nlohmann::json data);
	static void renderJSON(nlohmann::json data);
	static void renderMenu();
	static vJSON jsonsToDisplay;
	static GLFWwindow* window;
};

