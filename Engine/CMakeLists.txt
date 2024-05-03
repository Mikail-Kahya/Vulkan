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
    # Add other source files here    
    "vulkan/VulkanStructs.cpp"
    "vulkan/VulkanUtils.cpp" 
    "vulkan/VulkanBase.cpp"

    "engine/ResourceManager.cpp" 
    "engine/SceneManager.cpp" 
    "engine/Scene.cpp" 
    "engine/Mesh3D.cpp"
    "engine/Mesh2D.cpp"
    "engine/Camera.cpp" 
    "engine/Utils.cpp" 
    "engine/Mouse.cpp" 
    "engine/Keyboard.cpp" 
    "engine/Time.cpp" 
    "engine/structs.cpp"

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
    "abstraction/Texture.cpp")

# Create the executable
add_library(${ENGINE_NAME} STATIC ${SOURCES})
# 
# Link libraries

target_include_directories(${ENGINE_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${ENGINE_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external/stb)
target_link_libraries(${ENGINE_NAME} PUBLIC ${Vulkan_LIBRARIES} glfw)
target_link_libraries(${ENGINE_NAME} PUBLIC ${Vulkan_LIBRARIES} glm)

#add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/resources" "$<TARGET_FILE_DIR:${PROJECT_NAME}>/resources"
#)

#add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#    COMMAND "${CMAKE_COMMAND}" -E copy_directory TEXTURE_SOURCE_DIR TEXTURE_EXPORT_DIR
#)