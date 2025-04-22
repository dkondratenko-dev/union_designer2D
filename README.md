# Modern OpenGL App with wxWidgets

This project is a modern OpenGL application using wxWidgets as the UI framework to provide native OS controls for Mac, Windows, and Linux. The application demonstrates how to integrate OpenGL rendering with wxWidgets and render PNG images with transparent backgrounds.

![Demo](output.gif)

## Features

- **Cross-Platform**: Works on Windows, Mac, and Linux.
- **Native UI**: Utilizes wxWidgets for native OS controls.
- **Modern OpenGL**: Uses core profile OpenGL for rendering.
- **Scene Management**: Implements a scene management system with various scene items (Triangle, Circle, Square, Button).
- **Dynamic Controls**: Includes dynamic controls for rotation, zoom, and visibility toggling.

## Warning

The project is still in development and has not been tested on MacOS and Windows platforms.

## How This Works

All the build and configuration steps are performed inside a Docker container. The container is built using the Dockerfile in the root directory of the project.

This project uses CMake to manage the build process. It searches for the wxWidgets library using `FindPackage`. If wxWidgets is not found, it downloads the library source from GitHub, compiles it, and links it with the main project using the super build pattern with `ExternalProject_Add`.

## Requirements

- **Linux**: Ubuntu 20.04 or later
- **Docker**: [Docker Installation Guide](https://www.docker.com/)

## Building the Project

To build the project, follow these steps:

1. **Clone the Repository**:

    ```bash
    git clone https://github.com/dkondratenko-dev/union_designer2D.git
    cd union_designer2D
    ```

2. **Build and Run the Project**:

    ```bash
    ./run.sh
    ```

    This step will use Docker to build the project and run it.

## Class diagram

```plantuml
@startuml
!define RECTANGLE class

RECTANGLE Application {
  +OnInit(): bool
}

RECTANGLE ApplicationFrame {
  +ApplicationFrame(title: wxString): void
  -openGLCanvas: OpenGLCanvas
}

RECTANGLE SceneItem {
  +SceneItem(canvas: OpenGLCanvas): void
  +Render(): void
  -canvas: OpenGLCanvas
}

RECTANGLE Triangle {
  +Triangle(canvas: OpenGLCanvas): void
  +Render(): void
}

RECTANGLE Circle {
  +Circle(canvas: OpenGLCanvas): void
  +Render(): void
}

RECTANGLE Square {
  +Square(canvas: OpenGLCanvas): void
  +Render(): void
}

RECTANGLE Button {
  +Button(canvas: OpenGLCanvas, x: float, y: float, width: float, height: float, onClick: std::function<void()>): void
  +Initialize(): void
  +Render(): void
  +LoadButtonTexture(filename: wxString): bool
  +CheckClick(mouseX: float, mouseY: float): void
  -x: float
  -y: float
  -width: float
  -height: float
  -onClick: std::function<void()>
  -textureLoaded: bool
  -buttonTexture: GLuint
}

RECTANGLE OpenGLCanvas {
  +OpenGLCanvas(parent: ApplicationFrame, canvasAttrs: wxGLAttributes): void
  ~OpenGLCanvas(): void
  +OnPaint(event: wxPaintEvent): void
  +OnSize(event: wxSizeEvent): void
  +OnMouseClick(event: wxMouseEvent): void
  +RenderScene(): void
  +ToggleControlPanel(): void
  +GetControlPanel(): wxPanel*
  +model(): Model*
  -openGLContext: std::unique_ptr<wxGLContext>
  -isOpenGLInitialized: bool
  -isControlPanelVisible: bool
  -controlPanel: wxPanel*
  -rotationSlider: wxSlider*
  -zoomSlider: wxSlider*
  -toggleCheckbox: wxCheckBox*
  -m_model: Model
  -sceneItems: std::vector<std::unique_ptr<SceneItem>>
  -toggleButton: std::unique_ptr<Button>
}

RECTANGLE Model {
  +setZoomLevel(zoomLevel: float): void
  +setSize(width: int, height: int): void
  +getRotationAngle(): float
  +setRotationAngle(rotationAngle: float): void
  +getZoomLevel(): float
  +getShowTriangle(): bool
  +setShowTriangle(showTriangle: bool): void
  +getWidth(): int
  +getHeight(): int
  +getCx(): float
  +getCy(): float
  -recalculateCenter(): void
  -rotationAngle: float
  -zoomLevel: float
  -showTriangle: bool
  -width: int
  -height: int
  -cx: float
  -cy: float
}

Application --> ApplicationFrame : Creates
ApplicationFrame --> OpenGLCanvas : Contains
OpenGLCanvas --> SceneItem : Contains
SceneItem <|-- Triangle
SceneItem <|-- Circle
SceneItem <|-- Square
SceneItem <|-- Button
OpenGLCanvas --> Model : Uses
@enduml

```

## Sequence diagram

```plantuml
@startuml
actor User

User -> Application : OnInit()
Application -> ApplicationFrame : new ApplicationFrame("Hello OpenGL")
ApplicationFrame -> OpenGLCanvas : new OpenGLCanvas(this, attrs)
OpenGLCanvas -> OpenGLCanvas : Bind(wxEVT_PAINT, &OpenGLCanvas::OnPaint, this)
OpenGLCanvas -> OpenGLCanvas : Bind(wxEVT_SIZE, &OpenGLCanvas::OnSize, this)
OpenGLCanvas -> OpenGLCanvas : Bind(wxEVT_LEFT_DOWN, &OpenGLCanvas::OnMouseClick, this)
OpenGLCanvas -> wxPanel : new wxPanel(parent, wxID_ANY)
OpenGLCanvas -> wxSlider : new wxSlider(controlPanel, wxID_ANY, 200, 10, 300)
OpenGLCanvas -> wxSlider : new wxSlider(controlPanel, wxID_ANY, 0, 0, 360)
OpenGLCanvas -> wxCheckBox : new wxCheckBox(controlPanel, wxID_ANY, "Show Triangle")
OpenGLCanvas -> SceneItem : sceneItems.push_back(std::make_unique<Triangle>(this))
OpenGLCanvas -> SceneItem : sceneItems.push_back(std::make_unique<Circle>(this))
OpenGLCanvas -> SceneItem : sceneItems.push_back(std::make_unique<Square>(this))
OpenGLCanvas -> Button : toggleButton = std::make_unique<Button>(this, 10.0f, 10.0f, 40.0f, 40.0f, [this]() { ToggleControlPanel(); })

User -> OpenGLCanvas : OnPaint(wxPaintEvent& event)
OpenGLCanvas -> OpenGLCanvas : SetCurrent(*openGLContext)
OpenGLCanvas -> OpenGLCanvas : glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
OpenGLCanvas -> OpenGLCanvas : if (!isOpenGLInitialized)
OpenGLCanvas -> OpenGLCanvas : glewInit()
OpenGLCanvas -> OpenGLCanvas : isOpenGLInitialized = true
OpenGLCanvas -> Button : toggleButton->Initialize()
OpenGLCanvas -> OpenGLCanvas : RenderScene()
OpenGLCanvas -> OpenGLCanvas : SwapBuffers()

User -> OpenGLCanvas : OnSize(wxSizeEvent& event)
OpenGLCanvas -> OpenGLCanvas : if (isOpenGLInitialized)
OpenGLCanvas -> OpenGLCanvas : GetClientSize(&width, &height)
OpenGLCanvas -> Model : m_model.setSize(width, height)
OpenGLCanvas -> OpenGLCanvas : glViewport(0, 0, width, height)
OpenGLCanvas -> OpenGLCanvas : glMatrixMode(GL_PROJECTION)
OpenGLCanvas -> OpenGLCanvas : glLoadIdentity()
OpenGLCanvas -> OpenGLCanvas : glOrtho(0, width, height, 0, -1, 1)
OpenGLCanvas -> OpenGLCanvas : glMatrixMode(GL_MODELVIEW)
OpenGLCanvas -> OpenGLCanvas : glLoadIdentity()

User -> OpenGLCanvas : OnMouseClick(wxMouseEvent& event)
OpenGLCanvas -> OpenGLCanvas : wxPoint pos = event.GetPosition()
OpenGLCanvas -> Button : toggleButton->CheckClick(pos.x, pos.y)

OpenGLCanvas -> OpenGLCanvas : RenderScene()
OpenGLCanvas -> OpenGLCanvas : glClearColor(0.1f, 0.1f, 0.1f, 1.0f)
OpenGLCanvas -> OpenGLCanvas : glClear(GL_COLOR_BUFFER_BIT)
OpenGLCanvas -> OpenGLCanvas : glPushMatrix()
OpenGLCanvas -> Model : glScalef(m_model.getZoomLevel(), m_model.getZoomLevel(), 1.0f)
OpenGLCanvas -> SceneItem : for (const auto& item : sceneItems)
SceneItem -> SceneItem : item->Render()
OpenGLCanvas -> OpenGLCanvas : glPopMatrix()
OpenGLCanvas -> Button : toggleButton->Render()

User -> OpenGLCanvas : ToggleControlPanel()
OpenGLCanvas -> OpenGLCanvas : isControlPanelVisible = !isControlPanelVisible
OpenGLCanvas -> wxPanel : controlPanel->Show(isControlPanelVisible)
OpenGLCanvas -> ApplicationFrame : GetParent()->Layout()
@enduml

```

## Troubleshooting

If you encounter any issues during the build or run process, please check the console output for error messages and refer to the project's documentation for troubleshooting guides.

## Contributing

Contributions are welcome! Please open an issue or submit a pull request if you have any improvements or bug fixes.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
