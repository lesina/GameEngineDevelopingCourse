#include <array>

#include <Debug/Console.h>
#include <VulkanRHIFactory.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		VKAPI_ATTR VkBool32 VKAPI_CALL VulkanRHIFactory::DebugMessengerCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT severity,
			VkDebugUtilsMessageTypeFlagsEXT /*type*/,
			const VkDebugUtilsMessengerCallbackDataEXT* data,
			void* /*userData*/
		)
		{
			switch (severity)
			{
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
				Core::Console::PrintDebug("VVL [INFO]: {} {}: {}\n", data->messageIdNumber, data->pMessageIdName, data->pMessage);
				break;

			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
				Core::Console::PrintDebug("VVL [WARN]: {} {}: {}\n", data->messageIdNumber, data->pMessageIdName, data->pMessage);
				break;

			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
				Core::Console::PrintDebug("VVL [ERROR]: {} {}: {}\n", data->messageIdNumber, data->pMessageIdName, data->pMessage);
				break;

			default:
				break;
			}

			return VK_FALSE;
		}

		VulkanRHIFactory::VulkanRHIFactory()
		{
			constexpr bool ENABLE_VALIDATION = true;

			static constexpr std::array INSTANCE_EXTENSIONS =
			{
				VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
				VK_KHR_SURFACE_EXTENSION_NAME,
				VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
			};

			static constexpr std::array LAYERS =
			{
				"VK_LAYER_KHRONOS_validation"
			};

			const VkApplicationInfo appInfo =
			{
				.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
				.pNext = nullptr,
				.pApplicationName = nullptr,
				.applicationVersion = 0,
				.pEngineName = "GameEngineDevelopingCourse",
				.engineVersion = VK_MAKE_VERSION(1, 0, 0),
				.apiVersion = VK_API_VERSION_1_3,
			};

			const VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerInfo =
			{
				.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
				.pNext = nullptr,
				.flags = 0,
				.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
					VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
					VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
					VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
				.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT,
				.pfnUserCallback = &VulkanRHIFactory::DebugMessengerCallback,
				.pUserData = nullptr,
			};

			const VkInstanceCreateInfo instanceInfo =
			{
				.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
				.pNext = &debugUtilsMessengerInfo,
				.flags = 0,
				.pApplicationInfo = &appInfo,
				.enabledLayerCount = ENABLE_VALIDATION ? static_cast<uint32_t>(LAYERS.size()) : 0,
				.ppEnabledLayerNames = ENABLE_VALIDATION ? LAYERS.data() : nullptr,
				.enabledExtensionCount = static_cast<uint32_t>(INSTANCE_EXTENSIONS.size()),
				.ppEnabledExtensionNames = INSTANCE_EXTENSIONS.data(),
			};

			VULKAN_CALL_CHECK(volkInitialize());

			VULKAN_CALL_CHECK(vkCreateInstance(&instanceInfo, nullptr, &m_NativeHandle));

			volkLoadInstanceOnly(m_NativeHandle);

			VULKAN_CALL_CHECK(vkCreateDebugUtilsMessengerEXT(m_NativeHandle, &debugUtilsMessengerInfo, nullptr, &m_DebugUtilsMessenger));
		}

		VulkanRHIFactory::~VulkanRHIFactory()
		{
			if (m_DebugUtilsMessenger != VK_NULL_HANDLE)
			{
				vkDestroyDebugUtilsMessengerEXT(m_NativeHandle, m_DebugUtilsMessenger, nullptr);
				m_DebugUtilsMessenger = VK_NULL_HANDLE;
			}

			if (m_NativeHandle != VK_NULL_HANDLE)
			{
				vkDestroyInstance(m_NativeHandle, nullptr);
				m_NativeHandle = VK_NULL_HANDLE;
			}
		}

		RenderNativeObject VulkanRHIFactory::GetNativeObject()
		{
			return RenderNativeObject(m_NativeHandle);
		}

		VkInstance VulkanRHIFactory::GetHandle() const
		{
			return m_NativeHandle;
		}
	}
}