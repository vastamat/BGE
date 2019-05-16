#pragma once

#include "KeyCodes.h"
#include "MouseButtonCodes.h"
#include "core/Common.h"

#include <tuple>

namespace bge
{
namespace Input
{

/**
 * Check if a key has been pressed
 * @param key the keycode of the desired key to check
 * @return flag whether the key has been pressed
 */
bool IsKeyPressed(KeyCode key);

/**
 * Check if a mouse button has been pressed
 * @param button the mouse button code of the desired button to check
 * @return flag whether the button has been pressed
 */
bool IsMouseButtonPressed(MouseButtonCode button);

/**
 * Get the current mouse window coordinates
 * @return pair of the x and y window coords
 */
std::pair<float, float> GetMousePosition();

/**
 * Get the current x-axis mouse window coordinate
 * @return the x-axis window coordinate
 */
float GetMouseX();

/**
 * Get the current y-axis mouse window coordinate
 * @return the y-axis window coordinate
 */
float GetMouseY();

} // namespace Input
} // namespace bge