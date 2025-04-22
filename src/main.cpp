#include <GL/glew.h> // must be included before glcanvas.h
#include <cmath>     // For M_PI
#include <functional>
#include <iostream>
#include <memory>
#include <png.h>
#include <vector>
#include <wx/glcanvas.h>
#include <wx/wx.h>

#include "Model.h"

/**
 * @brief The main application class.
 */
class Application : public wxApp {
public:
  Application() = default;
  auto OnInit() -> bool override;
};

wxIMPLEMENT_APP(Application);

class OpenGLCanvas;

/**
 * @brief The main frame class containing the OpenGL canvas.
 */
class ApplicationFrame : public wxFrame {
public:
  /**
   * @brief Constructor for ApplicationFrame.
   * @param title The title of the frame.
   */
  ApplicationFrame(const wxString &title);

private:
  OpenGLCanvas *openGLCanvas{nullptr}; ///< Pointer to the OpenGL canvas.
};

/**
 * @brief Base class for scene items.
 */
class SceneItem {
public:
  /**
   * @brief Constructor for SceneItem.
   * @param canvas Pointer to the OpenGL canvas.
   */
  SceneItem(OpenGLCanvas *canvas) : canvas(canvas) {}

  /**
   * @brief Virtual destructor.
   */
  virtual ~SceneItem() = default;

  /**
   * @brief Render the scene item.
   */
  virtual void Render() = 0;

protected:
  OpenGLCanvas *canvas; ///< Pointer to the OpenGL canvas.
};

/**
 * @brief Class for rendering a triangle.
 */
class Triangle : public SceneItem {
public:
  /**
   * @brief Constructor for Triangle.
   * @param canvas Pointer to the OpenGL canvas.
   */
  Triangle(OpenGLCanvas *canvas) : SceneItem(canvas) {}

  /**
   * @brief Render the triangle.
   */
  void Render() override;
};

/**
 * @brief Class for rendering a circle.
 */
class Circle : public SceneItem {
public:
  /**
   * @brief Constructor for Circle.
   * @param canvas Pointer to the OpenGL canvas.
   */
  Circle(OpenGLCanvas *canvas) : SceneItem(canvas) {}

  /**
   * @brief Render the circle.
   */
  void Render() override;
};

/**
 * @brief Class for rendering a square.
 */
class Square : public SceneItem {
public:
  /**
   * @brief Constructor for Square.
   * @param canvas Pointer to the OpenGL canvas.
   */
  Square(OpenGLCanvas *canvas) : SceneItem(canvas) {}

  /**
   * @brief Render the square.
   */
  void Render() override;
};

/**
 * @brief Class for rendering a button.
 */
class Button : public SceneItem {
public:
  /**
   * @brief Constructor for Button.
   * @param canvas Pointer to the OpenGL canvas.
   * @param x The x-coordinate of the button.
   * @param y The y-coordinate of the button.
   * @param width The width of the button.
   * @param height The height of the button.
   * @param onClick The callback function for click events.
   */
  Button(OpenGLCanvas *canvas, float x, float y, float width, float height,
         std::function<void()> onClick)
      : SceneItem(canvas), x(x), y(y), width(width), height(height),
        onClick(onClick) {}

  /**
   * @brief Initialize the button.
   */
  void Initialize();

  /**
   * @brief Render the button.
   */
  void Render() override;

  /**
   * @brief Load a button texture from a PNG file.
   * @param filename The path to the PNG file.
   * @return True if the texture is loaded successfully, false otherwise.
   */
  auto LoadButtonTexture(const wxString &filename) -> bool;

  /**
   * @brief Check if the button is clicked.
   * @param mouseX The x-coordinate of the mouse click.
   * @param mouseY The y-coordinate of the mouse click.
   */
  void CheckClick(float mouseX, float mouseY);

private:
  float x;                       ///< X-coordinate of the button.
  float y;                       ///< Y-coordinate of the button.
  float width;                   ///< Width of the button.
  float height;                  ///< Height of the button.
  std::function<void()> onClick; ///< Callback function for click events.
  bool textureLoaded{
      false};              ///< Flag indicating if the button texture is loaded.
  GLuint buttonTexture{0}; ///< OpenGL texture ID for the button.
};

/**
 * @brief The OpenGL canvas class for rendering.
 */
class OpenGLCanvas : public wxGLCanvas {
public:
  /**
   * @brief Constructor for OpenGLCanvas.
   * @param parent The parent frame.
   * @param canvasAttrs The OpenGL canvas attributes.
   */
  OpenGLCanvas(ApplicationFrame *parent, const wxGLAttributes &canvasAttrs);

  /**
   * @brief Destructor for OpenGLCanvas.
   */
  ~OpenGLCanvas();

  /**
   * @brief Event handler for paint events.
   * @param event The paint event.
   */
  void OnPaint(wxPaintEvent &event);

  /**
   * @brief Event handler for size events.
   * @param event The size event.
   */
  void OnSize(wxSizeEvent &event);

  /**
   * @brief Event handler for mouse click events.
   * @param event The mouse event.
   */
  void OnMouseClick(wxMouseEvent &event);

  /**
   * @brief Render the OpenGL scene.
   */
  void RenderScene();

  /**
   * @brief Toggle the visibility of the control panel.
   */
  void ToggleControlPanel();

  /**
   * @brief Get the control panel.
   * @return Pointer to the control panel.
   */
  auto GetControlPanel() const -> wxPanel * { return controlPanel; }

  /**
   * @brief Get the model.
   * @return Pointer to the model.
   */
  auto model() -> Model * { return &m_model; }

private:
  std::unique_ptr<wxGLContext>
      openGLContext; ///< Unique pointer to the OpenGL context.
  bool isOpenGLInitialized{
      false}; ///< Flag indicating if OpenGL is initialized.
  bool isControlPanelVisible{
      false}; ///< Flag indicating if the control panel is visible.

  wxPanel *controlPanel;      ///< Pointer to the control panel.
  wxSlider *rotationSlider;   ///< Slider to control rotation.
  wxSlider *zoomSlider;       ///< Slider to control zoom.
  wxCheckBox *toggleCheckbox; ///< Checkbox to toggle triangle visibility.

  Model m_model; ///< Model data.

  std::vector<std::unique_ptr<SceneItem>> sceneItems; ///< List of scene items.
  std::unique_ptr<Button> toggleButton; ///< Button to toggle the control panel.
};

auto Application::OnInit() -> bool {
  if (!wxApp::OnInit())
    return false;

  auto frame = new ApplicationFrame("Hello OpenGL");
  frame->Show(true);

  return true;
}

ApplicationFrame::ApplicationFrame(const wxString &title)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(640, 480)) {
  wxGLAttributes attrs;
  attrs.PlatformDefaults().Defaults().EndList();

  openGLCanvas = new OpenGLCanvas(this, attrs);

  auto *sizer = new wxBoxSizer(wxVERTICAL);
  sizer->Add(openGLCanvas, 1, wxEXPAND);
  sizer->Add(openGLCanvas->GetControlPanel(), 0, wxEXPAND | wxALL, 5);

  SetSizer(sizer);
}

OpenGLCanvas::OpenGLCanvas(ApplicationFrame *parent,
                           const wxGLAttributes &attrs)
    : wxGLCanvas(parent, attrs),
      openGLContext(std::make_unique<wxGLContext>(this)) {
  Bind(wxEVT_PAINT, &OpenGLCanvas::OnPaint, this);
  Bind(wxEVT_SIZE, &OpenGLCanvas::OnSize, this);
  Bind(wxEVT_LEFT_DOWN, &OpenGLCanvas::OnMouseClick, this);

  // Create control panel
  controlPanel = new wxPanel(parent, wxID_ANY);
  controlPanel->Hide();

  auto *sizer = new wxBoxSizer(wxVERTICAL);

  // Add labels and sliders
  auto *zoomLabel = new wxStaticText(controlPanel, wxID_ANY, "Zoom:");
  zoomSlider = new wxSlider(controlPanel, wxID_ANY, 200, 10,
                            300); // Zoom slider from 0.1x to 3.0x
  zoomSlider->SetValue(model()->getZoomLevel() * 100.0f);

  auto *rotateLabel =
      new wxStaticText(controlPanel, wxID_ANY, "Rotate Triangle:");
  rotationSlider = new wxSlider(controlPanel, wxID_ANY, 0, 0, 360);

  toggleCheckbox = new wxCheckBox(controlPanel, wxID_ANY, "Show Triangle");
  toggleCheckbox->SetValue(true);

  sizer->Add(zoomLabel, 0, wxALL, 5);
  sizer->Add(zoomSlider, 0, wxEXPAND | wxALL, 5);

  sizer->Add(rotateLabel, 0, wxALL, 5);
  sizer->Add(rotationSlider, 0, wxEXPAND | wxALL, 5);

  sizer->Add(toggleCheckbox, 0, wxEXPAND | wxALL, 5);
  controlPanel->SetSizer(sizer);

  rotationSlider->Bind(wxEVT_SLIDER, [this](wxCommandEvent &) {
    auto const value = rotationSlider->GetValue();
    std::cout << "Rotation : " << value << "°" << std::endl;
    m_model.setRotationAngle(value);
    Refresh();
  });

  zoomSlider->Bind(wxEVT_SLIDER, [this](wxCommandEvent &) {
    auto const value = zoomSlider->GetValue() / 100.0f;
    std::cout << "Zoom : " << value << "x" << std::endl;
    m_model.setZoomLevel(value);
    Refresh();
  });

  toggleCheckbox->Bind(wxEVT_CHECKBOX, [this](wxCommandEvent &) {
    m_model.setShowTriangle(toggleCheckbox->IsChecked());
    Refresh();
  });

  // Initialize scene items
  sceneItems.push_back(std::make_unique<Triangle>(this));
  sceneItems.push_back(std::make_unique<Circle>(this));
  sceneItems.push_back(std::make_unique<Square>(this));

  // Initialize toggle button
  toggleButton = std::make_unique<Button>(this, 10.0f, 10.0f, 40.0f, 40.0f,
                                          [this]() { ToggleControlPanel(); });
}

OpenGLCanvas::~OpenGLCanvas() = default;

void OpenGLCanvas::OnPaint(wxPaintEvent &event) {
  wxPaintDC dc(this);
  SetCurrent(*openGLContext);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (!isOpenGLInitialized) {
    glewInit();
    isOpenGLInitialized = true;

    toggleButton->Initialize();
  }

  RenderScene();
  SwapBuffers();
}

void OpenGLCanvas::OnSize(wxSizeEvent &event) {
  if (isOpenGLInitialized) {
    int width, height;
    GetClientSize(&width, &height); // Get window size
    m_model.setSize(width, height);

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, -1, 1); // (0,0) in the top-left corner

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
  }
  event.Skip();
}

void OpenGLCanvas::OnMouseClick(wxMouseEvent &event) {
  wxPoint pos = event.GetPosition();
  toggleButton->CheckClick(pos.x, pos.y);
}

void OpenGLCanvas::RenderScene() {
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glPushMatrix();
  glScalef(m_model.getZoomLevel(), m_model.getZoomLevel(), 1.0f); // Apply zoom

  for (const auto &item : sceneItems) {
    item->Render();
  }

  glPopMatrix(); // Restore original scale

  // Render the button separately, outside the zoom transformation
  toggleButton->Render();
}

void OpenGLCanvas::ToggleControlPanel() {
  isControlPanelVisible = !isControlPanelVisible;
  controlPanel->Show(isControlPanelVisible);
  GetParent()->Layout();
}

void Triangle::Render() {
  if (!canvas->model()->getShowTriangle())
    return;

  float cx = canvas->model()->getCx();
  float cy = canvas->model()->getCy();
  float size = 50.0f; // Triangle size

  glPushMatrix();
  glTranslatef(cx, cy, 0.0f);
  glRotatef(canvas->model()->getRotationAngle(), 0.0f, 0.0f, 1.0f);

  glBegin(GL_TRIANGLES);
  glColor3f(1.0f, 0.0f, 0.0f);
  glVertex2f(-size, size);
  glColor3f(0.0f, 1.0f, 0.0f);
  glVertex2f(size, size);
  glColor3f(0.0f, 0.0f, 1.0f);
  glVertex2f(0.0f, -size);
  glEnd();

  glPopMatrix();
}

void Circle::Render() {
  constexpr float distanceFromCenterPx = 100.0f;
  float circleRadius = 30.0f;
  float cx = canvas->model()->getCx() - distanceFromCenterPx;
  float cy = canvas->model()->getCy() - distanceFromCenterPx;
  int circleSegments = 100;

  glColor3f(1.0f, 1.0f, 0.0f); // Yellow color
  glBegin(GL_LINE_LOOP);
  for (int i = 0; i <= circleSegments; ++i) {
    float angle = 2.0f * M_PI * float(i) / float(circleSegments);
    float x = cx + circleRadius * cos(angle);
    float y = cy + circleRadius * sin(angle);
    glVertex2f(x, y);
  }
  glEnd();
}

void Square::Render() {
  constexpr float distanceFromCenterPx = 100.0f;
  float squareSize = 40.0f;
  float cx = canvas->model()->getCx() + distanceFromCenterPx;
  float cy = canvas->model()->getCy() + distanceFromCenterPx;

  glColor3f(1.0f, 0.0f, 0.0f); // Red color
  glBegin(GL_LINE_LOOP);
  glVertex2f(cx - squareSize / 2, cy - squareSize / 2);
  glVertex2f(cx + squareSize / 2, cy - squareSize / 2);
  glVertex2f(cx + squareSize / 2, cy + squareSize / 2);
  glVertex2f(cx - squareSize / 2, cy + squareSize / 2);
  glEnd();
}

void Button::Initialize() {
  LoadButtonTexture("button.png"); // Load only when the context is active
}

void Button::Render() {
  if (textureLoaded) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, buttonTexture);
    glColor4f(1.0f, 1.0f, 1.0f,
              1.0f); // White for correct texture rendering with alpha channel

    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex2f(x, y);
    glTexCoord2f(1, 0);
    glVertex2f(x + width, y);
    glTexCoord2f(1, 1);
    glVertex2f(x + width, y + height);
    glTexCoord2f(0, 1);
    glVertex2f(x, y + height);
    glEnd();

    glDisable(GL_TEXTURE_2D);
  } else {
    std::cout << "Button::Render textureLoaded == false" << std::endl;
  }
}

auto Button::LoadButtonTexture(const wxString &filename) -> bool {
  // Generate and bind the texture
  glGenTextures(1, &buttonTexture);
  glBindTexture(GL_TEXTURE_2D, buttonTexture);

  // Check for errors
  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    std::cout << "OpenGL error after glGenTextures: " << error << std::endl;
    return false;
  }

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  // Open the file
  FILE *fp = fopen(filename.c_str(), "rb");
  if (!fp) {
    std::cout << "Failed to open file: " << filename << std::endl;
    return false;
  }

  // Create the png read struct
  png_structp png =
      png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
  if (!png) {
    std::cout << "Failed to create png read struct" << std::endl;
    fclose(fp);
    return false;
  }

  // Create the png info struct
  png_infop info = png_create_info_struct(png);
  if (!info) {
    std::cout << "Failed to create png info struct" << std::endl;
    png_destroy_read_struct(&png, nullptr, nullptr);
    fclose(fp);
    return false;
  }

  // Set error handling
  if (setjmp(png_jmpbuf(png))) {
    std::cout << "Error during png creation" << std::endl;
    png_destroy_read_struct(&png, &info, nullptr);
    fclose(fp);
    return false;
  }

  // Set up the input control
  png_init_io(png, fp);

  // Read the file information
  png_read_info(png, info);

  int width = png_get_image_width(png, info);
  int height = png_get_image_height(png, info);
  png_byte color_type = png_get_color_type(png, info);
  png_byte bit_depth = png_get_bit_depth(png, info);

  // Read any color_type into 8bit depth, RGBA format.
  // See http://www.libpng.org/pub/png/libpng-manual.txt

  if (bit_depth == 16)
    png_set_strip_16(png);

  if (color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_palette_to_rgb(png);

  // PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
  if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
    png_set_expand_gray_1_2_4_to_8(png);

  if (png_get_valid(png, info, PNG_INFO_tRNS))
    png_set_tRNS_to_alpha(png);

  // These color_type don't have an alpha channel then fill it with 0xff.
  if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_GRAY ||
      color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

  if (color_type == PNG_COLOR_TYPE_GRAY ||
      color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
    png_set_gray_to_rgb(png);

  png_read_update_info(png, info);

  // Allocate memory to hold the image
  std::vector<png_byte> image_data(width * height * 4);

  // Row pointers
  std::vector<png_bytep> row_pointers(height);
  for (int y = 0; y < height; y++) {
    row_pointers[y] = &image_data[y * width * 4];
  }

  // Read the image
  png_read_image(png, row_pointers.data());

  // Clean up
  png_destroy_read_struct(&png, &info, nullptr);
  fclose(fp);

  // Create the OpenGL texture
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, image_data.data());

  // Check if the texture is valid
  if (!glIsTexture(buttonTexture)) {
    std::cout << "Texture is not valid!" << std::endl;
    return false;
  }

  GLint boundTexture;
  glGetIntegerv(GL_TEXTURE_BINDING_2D, &boundTexture);
  if (boundTexture != static_cast<GLint>(buttonTexture)) {
    std::cout << "Texture is not bound correctly!" << std::endl;
    return false;
  }

  textureLoaded = true;
  return true;
}

void Button::CheckClick(float mouseX, float mouseY) {
  if (mouseX > x && mouseX < x + width && mouseY > y && mouseY < y + height) {
    onClick();
  }
}
