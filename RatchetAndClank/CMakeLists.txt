# Source files

set(GAME_NAME "RatchetAndClank")
set(SHADER_SOURCE_DIR "${CMAKE_SOURCE_DIR}/shaders")
set(SHADER_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/shaders")


file(GLOB_RECURSE GLSL_SOURCE_FILES
    "${SHADER_SOURCE_DIR}/*.frag"
    "${SHADER_SOURCE_DIR}/*.vert"
)

foreach(GLSL ${GLSL_SOURCE_FILES})
    get_filename_component(FILE_NAME ${GLSL} NAME)
    set(SPIRV "${SHADER_BINARY_DIR}/${FILE_NAME}.spv")
    add_custom_command(
        OUTPUT ${SPIRV}
        COMMAND ${Vulkan_GLSLC_EXECUTABLE} ${GLSL} -o ${SPIRV}
        DEPENDS ${GLSL}
    )
    list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(GLSL)

add_custom_target(
    Shaders 
    DEPENDS ${SPIRV_BINARY_FILES}
)

set(SOURCES
    "main.cpp"
)

# Create the executable
add_executable(${GAME_NAME} ${SOURCES} ${GLSL_SOURCE_FILES})
add_dependencies(${GAME_NAME} Shaders)


target_include_directories(${GAME_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${GAME_NAME} PRIVATE ${ENGINE_NAME})

add_custom_command(TARGET ${GAME_NAME} POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_SOURCE_DIR}/resources" "$<TARGET_FILE_DIR:${GAME_NAME}>/resources"
)

#add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#    COMMAND "${CMAKE_COMMAND}" -E copy_directory TEXTURE_SOURCE_DIR TEXTURE_EXPORT_DIR
#)