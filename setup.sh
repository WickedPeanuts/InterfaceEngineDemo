#!/bin/bash

# Exit on any error
set -e

echo "🚀 Starting environment setup for InterfaceEngineDemo..."

# 1. Create directory structure
mkdir -p external
mkdir -p build

# 2. Download ImGui (Latest Docking Tag)
echo "📥 Fetching ImGui v1.92.7-docking..."
if [ ! -d "external/imgui" ]; then
    git clone --branch v1.92.7-docking --depth 1 https://github.com/ocornut/imgui.git external/imgui
else
    echo "✅ ImGui already present."
fi

# 3. Download Raylib (Latest Tag 5.5)
echo "📥 Fetching Raylib v5.5..."
if [ ! -d "external/raylib" ]; then
    git clone --branch 5.5 --depth 1 https://github.com/raysan5/raylib.git external/raylib
else
    echo "✅ Raylib already present."
fi

# 4. Configure CMake
echo "⚙️  Configuring CMake..."
cmake -S . -B build

echo "------------------------------------------------"
echo "✅ SETUP COMPLETE!"
echo "You can now build with: cmake --build build"
echo "Or press F5 in VS Code to start debugging."
echo "------------------------------------------------"