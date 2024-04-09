#pragma once
#include <string>
#include <vector>
#include <vulkan/vulkan_core.h>

#include "Structs.h"

namespace mk
{
	class Shader final
	{
	public:
		Shader(const std::string& vertexShaderFile, const std::string& fragmentShaderFile);
		~Shader() = default;

		Shader(const Shader& other)					= delete;
		Shader(Shader&& other) noexcept				= delete;
		Shader& operator=(const Shader& other)		= delete;
		Shader& operator=(Shader&& other) noexcept	= delete;

		void Initialize(VkDevice vkDevice);
		const std::vector<VkPipelineShaderStageCreateInfo>& GetShaderStages() const;
		void DestroyModules(VkDevice vkDevice);

	private:
		VkPipelineShaderStageCreateInfo CreateShaderStageInfo(VkDevice device, const std::string& fileName, VkShaderStageFlagBits stage) const;
		VkPipelineShaderStageCreateInfo CreateVertexShaderInfo(VkDevice device, const std::string& fileName) const;
		VkPipelineShaderStageCreateInfo CreateFragmentShaderInfo(VkDevice device, const std::string& fileName) const;
		VkShaderModule CreateShaderModule(VkDevice device, const std::vector<char>& code) const;

		inline static const std::string SHADER_FOLDER{ "shaders/" };
		inline static const std::string EXTENSION{ ".spv" };

		const std::string m_VertexShaderFile{};
		const std::string m_FragmentShaderFile{};

		std::vector<VkPipelineShaderStageCreateInfo> m_ShaderStages{};
	};
}