#ifndef MODEL_H_
#define MODEL_H_

/**
 * @brief Class to hold the model data.
 */
class Model {
public:
  /**
   * @brief Set the zoom level and recalculate cx and cy.
   * @param zoomLevel The new zoom level.
   */
  void setZoomLevel(float zoomLevel) {
    this->zoomLevel = zoomLevel;
    recalculateCenter();
  }

  /**
   * @brief Set the size and recalculate cx and cy.
   * @param width The new width.
   * @param height The new height.
   */
  void setSize(int width, int height) {
    this->width = width;
    this->height = height;
    recalculateCenter();
  }

  /**
   * @brief Get the rotation angle.
   * @return The rotation angle.
   */
  auto getRotationAngle() const -> float { return rotationAngle; }

  /**
   * @brief Set the rotation angle.
   * @param rotationAngle The new rotation angle.
   */
  void setRotationAngle(float rotationAngle) {
    this->rotationAngle = rotationAngle;
  }

  /**
   * @brief Get the zoom level.
   * @return The zoom level.
   */
  auto getZoomLevel() const -> float { return zoomLevel; }

  /**
   * @brief Get the show triangle flag.
   * @return The show triangle flag.
   */
  auto getShowTriangle() const -> bool { return showTriangle; }

  /**
   * @brief Set the show triangle flag.
   * @param showTriangle The new show triangle flag.
   */
  void setShowTriangle(bool showTriangle) { this->showTriangle = showTriangle; }

  /**
   * @brief Get the width.
   * @return The width.
   */
  auto getWidth() const -> int { return width; }

  /**
   * @brief Get the height.
   * @return The height.
   */
  auto getHeight() const -> int { return height; }

  /**
   * @brief Get the center x coordinate.
   * @return The center x coordinate.
   */
  auto getCx() const -> float { return cx; }

  /**
   * @brief Get the center y coordinate.
   * @return The center y coordinate.
   */
  auto getCy() const -> float { return cy; }

private:
  /**
   * @brief Recalculate the center coordinates based on the current size and
   * zoom level.
   */
  void recalculateCenter() {
    cx = width / 2.0f / zoomLevel;
    cy = height / 2.0f / zoomLevel;
  }

private:
  float rotationAngle{0.0f}; ///< Rotation angle for the triangle.
  float zoomLevel{1.0f};     ///< Zoom level for the scene.
  bool showTriangle{true}; ///< Flag indicating if the triangle should be shown.
  int width{0};            ///< Width of the window.
  int height{0};           ///< Height of the window.
  float cx{0.0f};          ///< Center x coordinate.
  float cy{0.0f};          ///< Center y coordinate.
};

#endif // MODEL_H_