# ccgame
 A simple game library based on sdl2

# Getting Start
## Configure SDL2 develop environment
### Window(MSVC)
Use [vcpkg](https://github.com/microsoft/vcpkg) to install SDL2, SDL2-image and SDL2-ttf
```powershell
.\vcpkg.exe install SDL2:x64-windows
.\vcpkg.exe install SDL2-image:x64-windows
.\vcpkg.exe install SDL2-ttf:x64-windows
```
### Linux
Use [vcpkg](https://github.com/microsoft/vcpkg) or package manager to install.

## Example CMakeLists.txt
Copy the source of this library into your project and do `add_subdirectory()` to the library in order to use it.
```cmake
cmake_minimum_required(VERSION 3.10)

project(Demo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(ccgame)

add_executable(${PROJECT_NAME}
    main.cc
)
target_link_libraries(${PROJECT_NAME} PRIVATE ccgame)
```

## A simple demo
```cpp
#include "ccgame.h"
#include <iostream>
#include <stdexcept>

using namespace ccgame;

constexpr int WINDOW_WIDTH = 640;
constexpr int WINDOW_HEIGHT = 480;

int main(int argc, char **argv) {
  try {
    // The SDL type is used for initializing sdl2 and create SDL_Window and SDL_Renderer
    SDL sdl{ccgame::CCGAME_INIT_VIDEO};
    // The window will manage the game loop
    Window window("Demo", WINDOW_WIDTH, WINDOW_HEIGHT);

    // Use PaintView to Draw a simple rectangle
    PaintView paint(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    paint.SetBackgroundColor(SDL_Color{0xFF, 0, 0, 0xFF});

    paint.DrawRect(SDL_Rect{10, 10, 30, 30}, true,
                   SDL_Color{0, 0xFF, 0xFF, 0xFF});

    // You need to register the view in order to make it be rendered and get the event
    window.RegisterView(&paint);

    // You need to register the window to make it work
    sdl.RegisterWindow(window);

    // Start the main game loop
    window.Start();

    return 0;
  } catch (std::runtime_error &e) {
    // Every exception is throwed as std::runtime_error for now
    std::cout << e.what() << std::endl;
    return -1;
  }
}
```

More demos are put in the demo folder. You can build it by 
```bash
mkdir build
cd build
cmake -DCCGAME_WITH_EXAMPLE=ON ..
cmake --build .
```