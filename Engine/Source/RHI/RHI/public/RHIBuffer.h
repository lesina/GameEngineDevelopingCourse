#pragma once

#include <RHICommon.h>
#include <RHICore.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		class RHIBuffer : public RefCounter<RenderBackendResource>
		{
		public:
			enum class UsageFlag : uint8_t
			{
				ConstantBuffer = 0,
				CpuWrite,
				GpuReadOnly
			};

			struct Description
			{
				uint32_t Count = 1;
				uint32_t ElementSize = 1;
				UsageFlag UsageFlag = UsageFlag::GpuReadOnly;
				void* initData = nullptr;
			};

		public:
			using Ptr = RefCountPtr<RHIBuffer>;

		public:
			RHIBuffer() = delete;
			RHIBuffer(const Description& desc) : m_Description(desc) {}

		public:
			virtual void CopyData(int elementIndex, void* data, uint32_t DataSize) = 0;
			
			const Description& GetDesc() const { return m_Description; }

		protected:
			Description m_Description;
		};
	}
}