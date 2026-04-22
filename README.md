# Raylib + ImGui Integration (Proof of Concept)

This repository is a **Proof of Concept (PoC)** demonstrating how to seamlessly integrate **Dear ImGui** within a **Raylib** rendering environment.

This setup allows you to leverage Raylib for drawing graphics (like 2D/3D viewports, render textures, and game states) while using ImGui to provide powerful, dockable developer tools, inspectors, and UI overlays. It is designed as a foundational template for building plugin-based architectures or custom game engines.

Current raylib version: 5.5 - [Check release page](https://github.com/raysan5/raylib/releases/tag/5.5)

Current Imgui version: v1.92.7-docking - [Check release page](https://github.com/ocornut/imgui/releases/tag/v1.92.7) - Remember to use the docking version

---

## 📦 Dependencies & Setup

Before building, ensure your system has the required development libraries. The instructions below are primarily targeted at Linux environments.

### 1. Raylib System Dependencies

To run Raylib on Linux, you need several display server (X11/Wayland) and OpenGL libraries.

Please follow the official Raylib wiki for your specific Linux distribution:  
[Working on GNU Linux (Raylib Wiki)](https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux)

---

### 2. C/C++ Build Tools (ImGui & Core)

You must have a C++ compiler and CMake installed. ImGui does not require external libraries; it compiles directly from source alongside this project.

- **CMake**: Used to configure the build system  
- **C++ Compiler**: `gcc` or `clang`  
  - Arch/CachyOS: `base-devel`  
  - Ubuntu: `build-essential`

---

### 3. Debugging Tools (VS Code)

To enable step-through breakpoint debugging within VS Code:

- **GDB**: The GNU Debugger  
  - Arch: `sudo pacman -S gdb`  
  - Ubuntu: `sudo apt install gdb`

> ⚠️ If the build fails during the CMake configuration step, check terminal logs for missing system dependencies.

### 4. Downloading sources

In order to download all sources you can manually place them within `./external/imgui` and `./external/raylib`.

There is a script that can be executed to do it in root folder of the repo:

```bash
chmod +x ./setup.sh && ./setup.sh
```

---

## 🚀 Building the Project (Terminal)

Build the project using standard CMake commands:

```bash
# 1. Make build directory
mkdir build

# 2. Generate the build directory and cache
cmake -S . -B build

# 3. Compile the project
cmake --build build
```

The resulting executable will be placed inside the build/ directory.

## 💻 VS Code Environment Setup

For the best developer experience, you can automate the build and debug process using VS Code.

If they don't exist yet, create a `.vscode/` folder in the root of your project and add the following files:

### `.vscode/tasks.json`

```json
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "CMake: build",
            "type": "shell",
            "command": "cmake --build ${workspaceFolder}/build",
            "group": {
                "kind": "build",
                "isDefault": true
            }
        }
    ]
}
```

### `.vscode/launch.json`

```json
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "Debug ImGui App",
            "type": "cppdbg",
            "request": "launch",
            "program": "${workspaceFolder}/build/ImGuiTemplate",
            "args": [],
            "stopAtEntry": false,
            "cwd": "${workspaceFolder}",
            "environment": [],
            "externalConsole": false,
            "MIMode": "gdb",
            "setupCommands": [
                {
                    "description": "Enable pretty-printing for gdb",
                    "text": "-enable-pretty-printing",
                    "ignoreFailures": true
                }
            ],
            "preLaunchTask": "CMake: build",
            "miDebuggerPath": "/usr/bin/gdb"
        }
    ]
}
```