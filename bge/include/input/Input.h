#pragma once

#include "KeyCodes.h"
#include "MouseButtonCodes.h"
#include "core/Common.h"

#include <tuple>

namespace bge
{
namespace Input
{

bool IsKeyPressed(KeyCode key);
bool IsMouseButtonPressed(MouseButtonCode button);
std::pair<float, float> GetMousePosition();
float GetMouseX();
float GetMouseY();

} // namespace Input
} // namespace bge