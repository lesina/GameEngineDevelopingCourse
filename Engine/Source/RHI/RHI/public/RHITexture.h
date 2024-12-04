#pragma once

#include <RHICommon.h>
#include <RHICore.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		// It represents render backend resource so we still inherit it from RenderBackendResource
		class RHITexture : public RefCounter<RenderBackendResource>
		{
		public:
			enum class Dimensions : uint8_t
			{
				One = 0,
				Two,
				Three
			};

			// If we want to use it as bitfield, it cannot be enum.
			// Enum suggests that it is a data type.
			// If you bitwise OR two instances of the enum class data type, it will be no longer the same type
			// And I still want this params to leave in a namespace, that's why I used struct for that
			struct UsageFlags
			{
				using Flag = uint16_t;

				static constexpr Flag ShaderResource = 0x01;
				static constexpr Flag RenderTarget = 0x02;
				static constexpr Flag DepthStencil = 0x04;
			};

			struct Description
			{
				Dimensions Dimension = Dimensions::Two;
				uint64_t Width = 1;
				uint64_t Height = 1;
				uint16_t MipLevels = 1;
				ResourceFormat Format = ResourceFormat::UNKNOWN;
				UsageFlags::Flag Flags = UsageFlags::RenderTarget;
			};

		public:
			using Ptr =  RefCountPtr<RHITexture>;

		public:
			RHITexture() = delete;
			RHITexture(const Description& desc)
				: m_Description(desc)
			{
			}
			virtual ~RHITexture() = default;

		public:
			uint64_t GetWidth() const { return m_Description.Width; }
			uint64_t GetHeight() const { return m_Description.Height; }
			ResourceFormat GetFormat() const { return m_Description.Format; }

		protected:
			Description m_Description;
		};
	}
}