#include <algorithm>

#include "mf/core.h"

namespace logic {
MF_Color ScaleColor(MF_Color c, float factor) {
  /**
   * @brief This function modifies color by darkening or brightening it
   * @param factor 0 - 1.0 for darkening, above 1.0 for lightening
   */

  c.r = std::clamp((int)(c.r * factor), 0, 255);
  c.g = std::clamp((int)(c.g * factor), 0, 255);
  c.b = std::clamp((int)(c.b * factor), 0, 255);

  return c;
}
} // namespace logic