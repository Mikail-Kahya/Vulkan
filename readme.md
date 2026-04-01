# Vulkan Engine (Ratchet & Clank Demo)
**Graphics Programming Project** - C++20, Vulkan API, GLFW | [GitHub Repo](https://github.com/Mikail-Kahya/Vulkan) | [Portfolio](https://mikailkahya.netlify.app/vulkanengine)

![vulkan-card](https://github.com/user-attachments/assets/92c1637d-45c3-4cb2-b7a5-3da7032ef2ff)


## Overview
Custom game engine built from scratch to explore **low-level graphics programming** with Vulkan API. Features a **standalone graphics library** that links into any project via CMake, plus core engine systems for input, commands, and resource loading. Test demo loads Ratchet & Clank 3D model into massive open space where player can freely walk around using keyboard/XInput controllers.\
\
**Core goal:** Learn Vulkan render pipelines while maintaining clean engine architecture that supports multiple game projects.

## Design Philosophy
- **Engine neutrality** - No hardcoded game logic
- Graphics isolation - Vulkan in separate library
- **Game Programming Patterns** applied throughout
- This proves I can handle low-level graphics programming with clean, extensible architecture.

## Tech Stack
- C++20
- Vulkan API
- GLFW (window/input)
- CMake
- Git

## Architecture

[Game Project]\
     ↑ (CMake)\
[Core Engine]\
     ↑ (CMake)\
[Vulkan Graphics Library] (pimpl abstraction)

**Key design decisions:**
- **Graphics API as separate library** - Link via CMake, no direct Vulkan dependencies in game code (pimpl principles; Hides implementation details, reduces recompiles)
- **Abstraction layers**
- **No game-specific code** - Engine supports any project type

## Features

### Graphics Pipeline
- Full Vulkan implementation (render passes, pipelines, descriptor sets)
- 3D model loading (Ratchet & Clank demo model)
- 2D image loading support

### Input & Controls
- WASD - Movement
- Mouse - Camera look
- XInput controllers - Full support (not used in demo)

### Development
- **Command pattern** - Undo/redo, unit selection (RTS-friendly)
- Text-based UI - Screen size, debug info
- **Custom 2D collision** pre-Vulkan [GitHub Repo](https://github.com/Mikail-Kahya/custom-engine)
- Resource management - Models, textures, shaders

## Build
I built it through MSVC but any way of compiling a CMake project will work. The launching of the project can be found in the Out folder. 

## References
- [Game Programming Patterns](https://gameprogrammingpatterns.com)
- [Vulkan tutorial](https://vulkan-tutorial.com)
