#include <VulkanRHIBuffer.h>
#include <VulkanRHICore.h>
#include <VulkanRHICommandList.h>
#include <VulkanRHICommandAllocator.h>
#include <VulkanRHIPipelineStateObject.h>
#include <VulkanRHITechnique.h>
#include <VulkanRHITexture.h>
#include <RHIMesh.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		VulkanRHICommandList::VulkanRHICommandList(VulkanRHIDevice::Ptr device)
			: m_Device(device)
		{
			assert(m_Device != nullptr);

			m_CommandAllocator = new VulkanRHICommandAllocator(m_Device);

			const VkCommandBufferAllocateInfo allocateInfo =
			{
				.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
				.pNext = nullptr,
				.commandPool = m_CommandAllocator->GetNativeObject(),
				.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
				.commandBufferCount = 1,
			};

			VULKAN_CALL_CHECK(vkAllocateCommandBuffers(m_Device->GetHandle(), &allocateInfo, &m_NativeCommandBuffer));
		}

		void VulkanRHICommandList::ClearRenderTarget(RHITexture::Ptr renderTarget, RenderCore::Color& color)
		{
			ResetIfNeeded();

			assert(renderTarget != nullptr);

			VulkanRHITexture* vulkanRenderTarget = reinterpret_cast<VulkanRHITexture*>(renderTarget.Get());

			const VkClearColorValue clearColor =
			{
				.float32 = { color.x, color.y, color.z, color.w }
			};

			const VkImageSubresourceRange range =
			{
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.baseMipLevel = 0,
				.levelCount = 1,
				.baseArrayLayer = 0,
				.layerCount = 1,
			};

			ValidateTextureState(
				vulkanRenderTarget,
				{
					.Stages = VK_PIPELINE_STAGE_2_TRANSFER_BIT,
					.Access = VK_ACCESS_2_TRANSFER_WRITE_BIT,
					.Layout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				}
			);

			vkCmdClearColorImage(m_NativeCommandBuffer, vulkanRenderTarget->GetHandle(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &clearColor, 1, &range);
		}

		void VulkanRHICommandList::ClearDepthStencilView(RHITexture::Ptr depthStencil, ClearFlags::Flag clearFlags, float depth, uint8_t stencil)
		{
			ResetIfNeeded();

			assert(depthStencil != nullptr);

			VulkanRHITexture* vulkanDepthStencil = reinterpret_cast<VulkanRHITexture*>(depthStencil.Get());

			const VkClearDepthStencilValue clearColor = { .depth = depth, .stencil = stencil };

			const VkImageSubresourceRange range =
			{
				.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT,
				.baseMipLevel = 0,
				.levelCount = 1,
				.baseArrayLayer = 0,
				.layerCount = 1,
			};

			ValidateTextureState(
				vulkanDepthStencil,
				{
					.Stages = VK_PIPELINE_STAGE_2_TRANSFER_BIT,
					.Access = VK_ACCESS_2_TRANSFER_WRITE_BIT,
					.Layout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				}
			);

			vkCmdClearDepthStencilImage(m_NativeCommandBuffer, vulkanDepthStencil->GetHandle(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &clearColor, 1, &range);
		}

		void VulkanRHICommandList::SetRenderTargets(uint32_t TargetsNum, RHITexture::Ptr renderTarget, RHITexture::Ptr depthStencil)
		{
			ResetIfNeeded();

			assert(renderTarget != nullptr);
			assert(depthStencil != nullptr);
			assert(TargetsNum == 1); // Do not support more targets at the moment

			VulkanRHITexture* vulkanRenderTarget = reinterpret_cast<VulkanRHITexture*>(renderTarget.Get());
			ValidateTextureState(
				vulkanRenderTarget,
				{
					.Stages = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
					.Access = VK_ACCESS_2_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT,
					.Layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
				}
			);

			VulkanRHITexture* vulkanDepthStencil = reinterpret_cast<VulkanRHITexture*>(depthStencil.Get());
			ValidateTextureState(
				vulkanDepthStencil,
				{
					.Stages = VK_PIPELINE_STAGE_2_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_2_LATE_FRAGMENT_TESTS_BIT,
					.Access = VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_2_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
					.Layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
				}
			);

			const VkRenderingAttachmentInfo colorAttachmentInfo =
			{
				.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
				.pNext = nullptr,
				.imageView = vulkanRenderTarget->GetView(),
				.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
				.resolveMode = VK_RESOLVE_MODE_NONE,
				.resolveImageView = VK_NULL_HANDLE,
				.resolveImageLayout = VK_IMAGE_LAYOUT_UNDEFINED,
				.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD,
				.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
				.clearValue = {}
			};

			const VkRenderingAttachmentInfo depthStencilAttachmentInfo =
			{
				.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
				.pNext = nullptr,
				.imageView = vulkanDepthStencil->GetView(),
				.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
				.resolveMode = VK_RESOLVE_MODE_NONE,
				.resolveImageView = VK_NULL_HANDLE,
				.resolveImageLayout = VK_IMAGE_LAYOUT_UNDEFINED,
				.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD,
				.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
				.clearValue = {}
			};

			const VkRenderingInfo renderingInfo =
			{
				.sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
				.pNext = nullptr,
				.flags = 0,
				.renderArea =
				{
					.offset = {0, 0},

					.extent =
					{
						.width = static_cast<uint32_t>(vulkanRenderTarget->GetWidth()),
						.height = static_cast<uint32_t>(vulkanRenderTarget->GetHeight()),
					},
				},
				.layerCount = 1,
				.viewMask = 0,
				.colorAttachmentCount = 1,
				.pColorAttachments = &colorAttachmentInfo,
				.pDepthAttachment = &depthStencilAttachmentInfo,
				.pStencilAttachment = &depthStencilAttachmentInfo
			};

			vkCmdBeginRendering(m_NativeCommandBuffer, &renderingInfo);
			m_BoundRenderTarget = static_cast<VulkanRHITexture*>(renderTarget.Get());
			m_BoundDepthStencil = static_cast<VulkanRHITexture*>(depthStencil.Get());
		}

		void VulkanRHICommandList::SetViewport(const Viewport& viewport)
		{
			ResetIfNeeded();

			const VkViewport vulkanViewport =
			{
				.x = viewport.LeftX,
				.y = viewport.BottomY,
				.width = viewport.GetWidth(),
				.height = -viewport.GetHeight(),
				.minDepth = viewport.MinDepth,
				.maxDepth = viewport.MaxDepth,
			};

			vkCmdSetViewport(m_NativeCommandBuffer, 0, 1, &vulkanViewport);
		}

		void VulkanRHICommandList::SetScissorRect(const Rect& scissorRect)
		{
			ResetIfNeeded();

			const VkRect2D vulkanScissorRect =
			{
				.offset =
				{
					.x = static_cast<int32_t>(scissorRect.LeftX),
					.y = static_cast<int32_t>(scissorRect.TopY),
				},

				.extent =
				{
					.width = static_cast<uint32_t>(scissorRect.GetWidth()),
					.height = static_cast<uint32_t>(scissorRect.GetHeight()),
				},
			};

			vkCmdSetScissor(m_NativeCommandBuffer, 0, 1, &vulkanScissorRect);
		}

		RHICommandAllocator::Ptr VulkanRHICommandList::GetAllocator() const
		{
			return m_CommandAllocator;
		}

		void VulkanRHICommandList::Close()
		{
			if (m_Recording)
			{
				EndRenderPassIfBound();

				m_BoundTechnique = nullptr;
				m_Recording = false;
				VULKAN_CALL_CHECK(vkEndCommandBuffer(m_NativeCommandBuffer));
			}
		}

		void VulkanRHICommandList::Reset()
		{
			const VkCommandBufferBeginInfo beginInfo =
			{
				.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
				.pNext = nullptr,
				.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
				.pInheritanceInfo = nullptr
			};

			VULKAN_CALL_CHECK(vkBeginCommandBuffer(m_NativeCommandBuffer, &beginInfo));

			m_Recording = true;
			m_CommandsRecorded = 0;
		}

		void VulkanRHICommandList::SetPipelineStateObject(RHIPipelineStateObject::Ptr pso)
		{
			ResetIfNeeded();

			assert(pso != nullptr);
			vkCmdBindPipeline(m_NativeCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pso->GetNativeObject());
		}

		void VulkanRHICommandList::SetMesh(RHIMesh::Ptr mesh)
		{
			ResetIfNeeded();

			assert(mesh != nullptr);

			const VkBuffer vulkanVertexBuffer = mesh->GetVertexBuffer()->GetNativeObject();
			const VkBuffer vulkanIndexBuffer = mesh->GetIndexBuffer()->GetNativeObject();

			const VkDeviceSize offset = 0;

			vkCmdBindVertexBuffers(m_NativeCommandBuffer, 0, 1, &vulkanVertexBuffer, &offset);
			vkCmdBindIndexBuffer(m_NativeCommandBuffer, vulkanIndexBuffer, offset, ConvertToVkIndexType(mesh->GetIndexFormat()));
		}

		void VulkanRHICommandList::SetGraphicsConstantBuffer(uint32_t ParameterIdx, RHIBuffer::Ptr buffer, uint32_t bufferOffset)
		{
			ResetIfNeeded();

			assert(buffer != nullptr);
			assert(m_BoundTechnique != nullptr);

			const VkDescriptorBufferInfo bufferInfo =
			{
				.buffer = buffer->GetNativeObject(),
				.offset = bufferOffset * buffer->GetDesc().ElementSize,
				.range = buffer->GetDesc().ElementSize,
			};

			const VkWriteDescriptorSet dsWrite =
			{
				.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
				.pNext = nullptr,
				.dstSet = VK_NULL_HANDLE,
				.dstBinding = ParameterIdx,
				.dstArrayElement = 0,
				.descriptorCount = 1,
				.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
				.pImageInfo = nullptr,
				.pBufferInfo = &bufferInfo,
				.pTexelBufferView = nullptr,
			};

			vkCmdPushDescriptorSetKHR(m_NativeCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
				m_BoundTechnique->GetPipelineLayout(), 0, 1, &dsWrite);
		}

		void VulkanRHICommandList::SetTechnique(RHITechnique::Ptr technique)
		{
			ResetIfNeeded();

			assert(technique != nullptr);
			m_BoundTechnique = static_cast<VulkanRHITechnique*>(technique.Get());
		}

		void VulkanRHICommandList::SetPrimitiveTopology(PrimitiveTopology topology)
		{
			ResetIfNeeded();
			vkCmdSetPrimitiveTopology(m_NativeCommandBuffer, ConvertToVkPrimitiveTopology(topology));
		}

		void VulkanRHICommandList::DrawIndexedInstanced(
			uint32_t IndexCountPerInstance,
			uint32_t InstanceCount,
			uint32_t StartIndexLocation,
			int32_t BaseVertexLocation,
			uint32_t StartInstanceLocation
		)
		{
			ResetIfNeeded();

			vkCmdDrawIndexed(
				m_NativeCommandBuffer,
				IndexCountPerInstance,
				InstanceCount,
				StartIndexLocation,
				BaseVertexLocation,
				StartInstanceLocation);
		}

		RenderNativeObject VulkanRHICommandList::GetNativeObject()
		{
			return RenderNativeObject(m_NativeCommandBuffer);
		}

		VkCommandBuffer VulkanRHICommandList::GetHandle() const
		{
			return m_NativeCommandBuffer;
		}

		uint32_t VulkanRHICommandList::GetCommandsRecorded() const
		{
			return m_CommandsRecorded;
		}

		void VulkanRHICommandList::ResetIfNeeded()
		{
			if (!m_Recording)
			{
				Reset();
			}

			++m_CommandsRecorded;
		}

		void VulkanRHICommandList::EndRenderPassIfBound()
		{
			if (m_BoundRenderTarget == nullptr)
			{
				return;
			}

			vkCmdEndRendering(m_NativeCommandBuffer);

			m_BoundRenderTarget = nullptr;
			m_BoundDepthStencil = nullptr;
		}

		void VulkanRHICommandList::CopyBufferToBuffer(RHIBuffer::Ptr src, RHIBuffer::Ptr dst, uint32_t sizeBytes)
		{
			ResetIfNeeded();

			const VkBufferCopy region =
			{
				.srcOffset = 0,
				.dstOffset = 0,
				.size = sizeBytes,
			};

			VulkanRHIBuffer* vulkanSrcBuffer = reinterpret_cast<VulkanRHIBuffer*>(src.Get());
			VulkanRHIBuffer* vulkanDstBuffer = reinterpret_cast<VulkanRHIBuffer*>(dst.Get());

			vkCmdCopyBuffer(m_NativeCommandBuffer, vulkanSrcBuffer->GetHandle(), vulkanDstBuffer->GetHandle(), 1, &region);
		}

		void VulkanRHICommandList::ValidateTextureState(VulkanRHITexture* texture, const VulkanRHITexture::State& state)
		{
			assert(texture != nullptr);

			const VulkanRHITexture::State currentState = texture->GetCurrentState();

			if (currentState.Layout != state.Layout)
			{
				const VkImageMemoryBarrier2 barrier =
				{
					.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
					.pNext = nullptr,
					.srcStageMask = currentState.Stages,
					.srcAccessMask = currentState.Access,
					.dstStageMask = state.Stages,
					.dstAccessMask = state.Access,
					.oldLayout = currentState.Layout,
					.newLayout = state.Layout,
					.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
					.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
					.image = texture->GetHandle(),

					.subresourceRange =
					{
						.aspectMask = ConvertToImageAspectFlags(texture->GetFormat()),
						.baseMipLevel = 0,
						.levelCount = 1,
						.baseArrayLayer = 0,
						.layerCount = 1,
					},
				};

				const VkDependencyInfo dependencyInfo =
				{
					.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
					.pNext = nullptr,
					.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT,
					.memoryBarrierCount = 0,
					.pMemoryBarriers = nullptr,
					.bufferMemoryBarrierCount = 0,
					.pBufferMemoryBarriers = nullptr,
					.imageMemoryBarrierCount = 1,
					.pImageMemoryBarriers = &barrier
				};

				vkCmdPipelineBarrier2(m_NativeCommandBuffer, &dependencyInfo);

				texture->SetCurrentState(state);
			}
		}
	}
}