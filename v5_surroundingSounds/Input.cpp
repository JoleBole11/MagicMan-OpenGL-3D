#include "Input.h"

std::unordered_map<int, bool> Input::key_states;
std::unordered_map<int, bool> Input::last_key_states;
std::unordered_map<int, bool> Input::special_states;
std::unordered_map<int, bool> Input::last_special_states;
std::unordered_map<int, bool> Input::mouse_button_states;
std::unordered_map<int, bool> Input::last_mouse_button_states;
glm::vec2 Input::mouse_position = glm::vec2(0);
glm::vec2 Input::last_mouse_position = glm::vec2(0);
glm::vec2 Input::delta_mouse_position = glm::vec2(0);
bool Input::is_cursor_locked = false;
std::unordered_map<const char*, float> Input::axes;