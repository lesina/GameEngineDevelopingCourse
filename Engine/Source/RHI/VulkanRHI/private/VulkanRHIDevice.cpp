#include <VulkanRHIDevice.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		namespace
		{
			constexpr std::array REQUIRED_DEVICE_EXTENSIONS =
			{
				VK_KHR_SWAPCHAIN_EXTENSION_NAME,
				VK_KHR_PUSH_DESCRIPTOR_EXTENSION_NAME
			};

			bool IsPhysicalDeviceSuitable(VkPhysicalDevice physicalDevice)
			{
				/* Enumerate supported device extensions */
				uint32_t supportedExtensionCount = 0;
				VULKAN_CALL_CHECK(vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &supportedExtensionCount, nullptr));

				std::vector<VkExtensionProperties> supportedExtensions(supportedExtensionCount);
				VULKAN_CALL_CHECK(vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &supportedExtensionCount, supportedExtensions.data()));

				/* Check required extensions support */
				for (const char* requiredExtension : REQUIRED_DEVICE_EXTENSIONS)
				{
					bool requiredExtensionSupported = false;

					for (const auto& supportedExtensionProperties : supportedExtensions)
					{
						if (std::strcmp(supportedExtensionProperties.extensionName, requiredExtension) == 0)
						{
							requiredExtensionSupported = true;
							break;
						}
					}

					if (!requiredExtensionSupported)
					{
						return false;
					}
				}

				/* Check timeline semaphore support */
				VkPhysicalDeviceVulkan12Features features12{};
				features12.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
				features12.pNext = nullptr;

				VkPhysicalDeviceFeatures2 features2{};
				features2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
				features2.pNext = &features12;

				vkGetPhysicalDeviceFeatures2(physicalDevice, &features2);

				return features12.timelineSemaphore == VK_TRUE;
			}
		}

		VulkanRHIDevice::VulkanRHIDevice(VulkanRHIFactory::Ptr instance)
		{
			PickPhysicalDevice(instance->GetHandle());
			PickMainQueueFamily();
			CreateLogicalDevice(instance->GetHandle());

			volkLoadDevice(m_NativeHandle);

			vkGetDeviceQueue(m_NativeHandle, m_MainQueueFamilyIndex, 0, &m_MainQueue);
		}

		VulkanRHIDevice::~VulkanRHIDevice()
		{
			if (m_NativeHandle != VK_NULL_HANDLE) {
				vkDestroyDevice(m_NativeHandle, nullptr);
				m_NativeHandle = VK_NULL_HANDLE;
			}
		}

		RenderNativeObject VulkanRHIDevice::GetNativeObject()
		{
			return RenderNativeObject(m_NativeHandle);
		}

		VkDevice VulkanRHIDevice::GetHandle() const
		{
			return m_NativeHandle;
		}

		VkPhysicalDevice VulkanRHIDevice::GetPhysicalDevice() const
		{
			return m_PhysicalDevice;
		}

		VkQueue VulkanRHIDevice::GetMainQueue() const
		{
			return m_MainQueue;
		}

		uint32_t VulkanRHIDevice::GetMainQueueFamilyIndex() const
		{
			return m_MainQueueFamilyIndex;
		}

		VkDeviceMemory VulkanRHIDevice::AllocateDeviceMemory(VkBuffer buffer, VkMemoryPropertyFlags properties)
		{
			VkMemoryRequirements memRequirements;
			vkGetBufferMemoryRequirements(m_NativeHandle, buffer, &memRequirements);

			const VkMemoryAllocateInfo allocateInfo =
			{
				.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
				.pNext = nullptr,
				.allocationSize = memRequirements.size,
				.memoryTypeIndex = FindMemoryTypeIndex(memRequirements.memoryTypeBits, properties),
			};

			VkDeviceMemory deviceMemory = VK_NULL_HANDLE;
			VULKAN_CALL_CHECK(vkAllocateMemory(m_NativeHandle, &allocateInfo, nullptr, &deviceMemory));
			VULKAN_CALL_CHECK(vkBindBufferMemory(m_NativeHandle, buffer, deviceMemory, 0));

			return deviceMemory;
		}

		VkDeviceMemory VulkanRHIDevice::AllocateDeviceMemory(VkImage image, VkMemoryPropertyFlags properties)
		{
			VkMemoryRequirements memRequirements;
			vkGetImageMemoryRequirements(m_NativeHandle, image, &memRequirements);

			const VkMemoryAllocateInfo allocateInfo =
			{
				.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
				.pNext = nullptr,
				.allocationSize = memRequirements.size,
				.memoryTypeIndex = FindMemoryTypeIndex(memRequirements.memoryTypeBits, properties),
			};

			VkDeviceMemory deviceMemory = VK_NULL_HANDLE;
			VULKAN_CALL_CHECK(vkAllocateMemory(m_NativeHandle, &allocateInfo, nullptr, &deviceMemory));
			VULKAN_CALL_CHECK(vkBindImageMemory(m_NativeHandle, image, deviceMemory, 0));

			return deviceMemory;
		}

		void VulkanRHIDevice::PickPhysicalDevice(VkInstance instance)
		{
			/* Enumerate physical devices */
			uint32_t physicalDeviceCount = 0;
			VULKAN_CALL_CHECK(vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr));

			std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
			VULKAN_CALL_CHECK(vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.data()));

			/* Enumerate suitable physical devices */
			std::vector<VkPhysicalDevice> suitablePhysicalDevices;
			suitablePhysicalDevices.reserve(physicalDeviceCount);

			for (auto physicalDevice : physicalDevices)
			{
				if (IsPhysicalDeviceSuitable(physicalDevice))
				{
					suitablePhysicalDevices.push_back(physicalDevice);
				}
			}

			assert(!suitablePhysicalDevices.empty());

			/* Try to find a discrete GPU device */
			for (auto physicalDevice : suitablePhysicalDevices)
			{
				VkPhysicalDeviceProperties properties;
				vkGetPhysicalDeviceProperties(physicalDevice, &properties);

				if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
				{
					m_PhysicalDevice = physicalDevice;
					return;
				}
			}

			/* Fall back to whatever GPU is at least suitable */
			m_PhysicalDevice = suitablePhysicalDevices[0];
		}

		void VulkanRHIDevice::PickMainQueueFamily()
		{
			constexpr VkQueueFlags MAIN_QUEUE_FLAGS = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT;

			uint32_t queueFamilyCount = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queueFamilyCount, nullptr);

			std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
			vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queueFamilyCount, queueFamilies.data());

			for (uint32_t queueFamilyIndex = 0; queueFamilyIndex < queueFamilyCount; ++queueFamilyIndex)
			{
				if ((queueFamilies[queueFamilyIndex].queueFlags & MAIN_QUEUE_FLAGS) == MAIN_QUEUE_FLAGS)
				{
					m_MainQueueFamilyIndex = queueFamilyIndex;
					return;
				}
			}

			assert(false && "Main queue family not found!");
		}

		void VulkanRHIDevice::CreateLogicalDevice(VkInstance instance)
		{
			VkPhysicalDeviceVulkan12Features features12{};
			features12.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
			features12.pNext = nullptr;
			features12.timelineSemaphore = VK_TRUE;

			VkPhysicalDeviceSynchronization2Features sync2Feature =
			{
				.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SYNCHRONIZATION_2_FEATURES,
				.pNext = &features12,
				.synchronization2 = VK_TRUE,
			};

			VkPhysicalDeviceDynamicRenderingFeaturesKHR dynamicRenderingFeature =
			{
				.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES_KHR,
				.pNext = &sync2Feature,
				.dynamicRendering = VK_TRUE,
			};

			VkPhysicalDeviceFeatures2 features2{};
			features2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
			features2.pNext = &dynamicRenderingFeature;

			static constexpr float DEFAULT_QUEUE_PRIORITY = 1.0f;
			const VkDeviceQueueCreateInfo mainQueueInfo =
			{
				.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.queueFamilyIndex = m_MainQueueFamilyIndex,
				.queueCount = 1,
				.pQueuePriorities = &DEFAULT_QUEUE_PRIORITY,
			};

			const VkDeviceCreateInfo deviceInfo =
			{
				.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
				.pNext = &features2,
				.flags = 0,
				.queueCreateInfoCount = 1,
				.pQueueCreateInfos = &mainQueueInfo,
				.enabledLayerCount = 0,
				.ppEnabledLayerNames = nullptr,
				.enabledExtensionCount = static_cast<uint32_t>(REQUIRED_DEVICE_EXTENSIONS.size()),
				.ppEnabledExtensionNames = REQUIRED_DEVICE_EXTENSIONS.data(),
				.pEnabledFeatures = nullptr,
			};

			VULKAN_CALL_CHECK(vkCreateDevice(m_PhysicalDevice, &deviceInfo, nullptr, &m_NativeHandle));
		}

		uint32_t VulkanRHIDevice::FindMemoryTypeIndex(uint32_t memoryTypeBits, VkMemoryPropertyFlags properties) const
		{
			VkPhysicalDeviceMemoryProperties memProperties;
			vkGetPhysicalDeviceMemoryProperties(m_PhysicalDevice, &memProperties);

			for (uint32_t i = 0; i < memProperties.memoryTypeCount; ++i) {
				if ((memoryTypeBits & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
					return i;
				}
			}

			assert(!"Failed to find suitable memory type");

			return 0;
		}
	}
}