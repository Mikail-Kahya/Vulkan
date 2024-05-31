# Source files

set(ENGINE_NAME MkVulktra)
set(ENGINE_NAME ${ENGINE_NAME} PARENT_SCOPE)


#file(GLOB_RECURSE GLSL_SOURCE_FILES
#    "${SHADER_SOURCE_DIR}/*.frag"
#    "${SHADER_SOURCE_DIR}/*.vert"
#)
#
#foreach(GLSL ${GLSL_SOURCE_FILES})
#    get_filename_component(FILE_NAME ${GLSL} NAME)
#    set(SPIRV "${SHADER_BINARY_DIR}/${FILE_NAME}.spv")
#    add_custom_command(
#        OUTPUT ${SPIRV}
#        COMMAND ${Vulkan_GLSLC_EXECUTABLE} ${GLSL} -o ${SPIRV}
#        DEPENDS ${GLSL}
#    )
#    list(APPEND SPIRV_BINARY_FILES ${SPIRV})
#endforeach(GLSL)
#
#add_custom_target(
#    Shaders 
#    DEPENDS ${SPIRV_BINARY_FILES}
#)

set(SOURCES
    "MkVulktra.cpp"

# -----------------------------------------------------------------
# --------------------------VULKAN---------------------------------
# -----------------------------------------------------------------
    "vulkan/VulkanStructs.cpp"
    "vulkan/VulkanUtils.cpp" 
    "vulkan/VulkanBase.cpp"

    "abstraction/Shader.cpp"
    "abstraction/SwapChain.cpp" 
    "abstraction/CommandPool.cpp" 
    "abstraction/DescriptorPool.cpp" 
    "abstraction/UniformBuffer.cpp" 
    "abstraction/Pipeline2D.cpp" 
     
    "abstraction/Pipeline3D.cpp" 
    "abstraction/VertexBuffer.cpp" 
    "abstraction/RenderPass.cpp" 
    "abstraction/Device.cpp" 
    "abstraction/CommandBuffer.cpp" 
    "abstraction/Window.cpp" 
    "abstraction/SecondaryCommandBuffer.cpp" 
    "abstraction/Texture.cpp" 
    "abstraction/DepthBuffer.cpp"

# -----------------------------------------------------------------
# ------------------------COMPONENTS-------------------------------
# -----------------------------------------------------------------
    "components/MeshComponent.cpp"

# -----------------------------------------------------------------
# --------------------------CORE-----------------------------------
# -----------------------------------------------------------------
    "core/GameObject.cpp"
    "core/IComponent.cpp"
    "core/IObserver.cpp"
    "core/ResourceManager.cpp" 
    "core/Scene.cpp"
    "core/SceneManager.cpp"
    "core/ServiceLocator.cpp" 
    "core/structs.cpp"
    "core/Time.cpp" 
    "core/Transform.cpp"
    "core/Utils.cpp" 

# -----------------------------------------------------------------
# --------------------------CRTP-----------------------------------
# -----------------------------------------------------------------
    "CRTP/ISingleton.h"

# -----------------------------------------------------------------
# --------------------------INPUT----------------------------------
# -----------------------------------------------------------------
    "input/CommandInput.h"
    "input/InputKeys.h"
    "input/InputMapping.h"
    "input/Action.cpp"
    "input/ControllerInput.cpp"
    "input/KeyboardInput.cpp"
    "input/Controller.cpp"
    "input/GLFWInput.cpp"
    "input/Keyboard.cpp"
    "input/Mouse.cpp"
    "input/Camera.cpp"

# -----------------------------------------------------------------
# --------------------------RENDER---------------------------------
# -----------------------------------------------------------------
    "abstraction/Mesh3D.cpp"
    "abstraction/Mesh2D.cpp"
    "render/VulkanRenderer.cpp"
    "components/cameracomponent.cpp"


# -----------------------------------------------------------------
# --------------------------SOUND----------------------------------
# -----------------------------------------------------------------
    "sound/DefaultSoundSystem.cpp"
    "sound/NullSoundSystem.h"

# -----------------------------------------------------------------
# -------------------------PHYSICS---------------------------------
# -----------------------------------------------------------------
    "physics/NullPhysicsSystem.h" 
    "physics/IPhysicsSystem.h" 
    "physics/JoltPhysicsSystem.cpp" "components/ColliderComponent.cpp"
)

# Create the executable
add_library(${ENGINE_NAME} STATIC ${SOURCES} )
target_include_directories(${ENGINE_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
# 
# Link libraries
include(FetchContent)

FetchContent_Declare(tinyobjloader
  GIT_REPOSITORY https://github.com/tinyobjloader/tinyobjloader.git
  GIT_TAG v2.0.0rc13
  GIT_SHALLOW TRUE
  GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(tinyobjloader)

SET(XINPUT_LIB XInput)


target_include_directories(${ENGINE_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external/stb)
target_link_libraries(${ENGINE_NAME} PUBLIC 
    ${Vulkan_LIBRARIES} 
    glfw
    glm
    tinyobjloader
    ${XINPUT_LIB} 
)

#soloud
target_link_libraries(${ENGINE_NAME} PUBLIC soloud)

if(EMSCRIPTEN)
    target_compile_definitions(soloud PUBLIC WITH_SDL2_STATIC)
else()
    target_compile_definitions(soloud PUBLIC WITH_MINIAUDIO)
    #target_link_libraries(soloud PUBLIC ${sdl2_SOURCE_DIR})
endif()

#jolt (help by Matias Devred)
#FetchContent_Declare(
#    jolt
#    GIT_REPOSITORY https://github.com/jrouwe/JoltPhysics.git
#    GIT_SHALLOW TRUE
#    GIT_TAG        v5.0.0
#    SOURCE_SUBDIR "Build"
#)
#FetchContent_MakeAvailable(jolt)
#
#target_include_directories(${ENGINE_NAME} PRIVATE ${JoltPhysics_SOURCE_DIR}/..)
#target_link_libraries(${ENGINE_NAME} PRIVATE Jolt)



#add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/resources" "$<TARGET_FILE_DIR:${PROJECT_NAME}>/resources"
#)

#add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#    COMMAND "${CMAKE_COMMAND}" -E copy_directory TEXTURE_SOURCE_DIR TEXTURE_EXPORT_DIR
#)
