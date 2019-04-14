#pragma once

#include "KeyCodes.h"
#include "MouseButtonCodes.h"
#include "core/Common.h"

#include <tuple>

namespace bge
{

class Input
{
public:
  static bool IsKeyPressed(KeyCode key);
  static bool IsMouseButtonPressed(MouseButtonCode button);
  static std::pair<float, float> GetMousePosition();
  static float GetMouseX();
  static float GetMouseY();
};

} // namespace bge