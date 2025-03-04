#pragma once

#include <Debug/Assertions.h>
#include <RHI/RHI/export.h>

namespace cqe
{
	namespace Render::RHI
	{
		enum class ResourceFormat : uint8_t
		{
			UNKNOWN,

			R8_UINT,
			R8_SINT,
			R8_UNORM,
			R8_SNORM,
			RG8_UINT,
			RG8_SINT,
			RG8_UNORM,
			RG8_SNORM,
			R16_UINT,
			R16_SINT,
			R16_UNORM,
			R16_SNORM,
			R16_FLOAT,
			BGRA4_UNORM,
			B5G6R5_UNORM,
			B5G5R5A1_UNORM,
			RGBA8_UINT,
			RGBA8_SINT,
			RGBA8_UNORM,
			RGBA8_SNORM,
			BGRA8_UNORM,
			SRGBA8_UNORM,
			SBGRA8_UNORM,
			R10G10B10A2_UNORM,
			R11G11B10_FLOAT,
			RG16_UINT,
			RG16_SINT,
			RG16_UNORM,
			RG16_SNORM,
			RG16_FLOAT,
			R32_UINT,
			R32_SINT,
			R32_FLOAT,
			RGBA16_UINT,
			RGBA16_SINT,
			RGBA16_FLOAT,
			RGBA16_UNORM,
			RGBA16_SNORM,
			RG32_UINT,
			RG32_SINT,
			RG32_FLOAT,
			RGB32_UINT,
			RGB32_SINT,
			RGB32_FLOAT,
			RGBA32_UINT,
			RGBA32_SINT,
			RGBA32_FLOAT,

			D16,
			D24S8,
			X24G8_UINT,
			D32,
			D32S8,
			X32G8_UINT,

			BC1_UNORM,
			BC1_UNORM_SRGB,
			BC2_UNORM,
			BC2_UNORM_SRGB,
			BC3_UNORM,
			BC3_UNORM_SRGB,
			BC4_UNORM,
			BC4_SNORM,
			BC5_UNORM,
			BC5_SNORM,
			BC6H_UFLOAT,
			BC6H_SFLOAT,
			BC7_UNORM,
			BC7_UNORM_SRGB,

			COUNT,
		};

		struct Viewport
		{
			float LeftX = 0.f;
			float RightX = 0.f;
			float TopY = 0.f;
			float BottomY = 0.f;
			float MinDepth = 0.f;
			float MaxDepth = 1.f;

			Viewport() = default;

			Viewport(
				float _LeftX,
				float _RightX,
				float _TopY,
				float _BottomY,
				float _MinDepth,
				float _MaxDepth
			) :
				LeftX(_LeftX),
				RightX(_RightX),
				TopY(_TopY),
				BottomY(_BottomY),
				MinDepth(_MinDepth),
				MaxDepth(_MaxDepth)
			{

			}

			bool operator ==(const Viewport& b) const
			{
				return LeftX == b.LeftX
					&& TopY == b.TopY
					&& MinDepth == b.MinDepth
					&& RightX == b.RightX
					&& BottomY == b.BottomY
					&& MaxDepth == b.MaxDepth;
			}

			bool operator !=(const Viewport& b) const
			{
				return !(*this == b);
			}

			float GetWidth() const
			{
				return RightX - LeftX;
			}

			float GetHeight() const
			{
				return BottomY - TopY;
			}
		};

		struct Rect
		{
			uint32_t LeftX = 0;
			uint32_t RightX = 0;
			uint32_t TopY = 0;
			uint32_t BottomY = 0;

			Rect() = default;

			Rect(
				uint32_t _LeftX,
				uint32_t _RightX,
				uint32_t _TopY,
				uint32_t _BottomY
			) :
				LeftX(_LeftX),
				RightX(_RightX),
				TopY(_TopY),
				BottomY(_BottomY)
			{
			}

			bool operator ==(const Rect& b) const
			{
				return LeftX == b.LeftX
					&& TopY == b.TopY
					&& RightX == b.RightX
					&& BottomY == b.BottomY;
			}

			bool operator !=(const Rect& b) const
			{
				return !(*this == b);
			}

			uint32_t GetWidth() const
			{
				return RightX - LeftX;
			}

			uint32_t GetHeight() const
			{
				return BottomY - TopY;
			}
		};

		enum class LogicOperation : uint8_t
		{
			Clear = 0,
			Set,
			Copy,
			CopyInverted,
			Noop,
			Invert,
			And,
			Nand,
			Or,
			Nor,
			Xor,
			Equiv,
			AndReverse,
			AndInverted,
			OrReverse,
			OrInverted
		};

		enum class Blend : uint8_t
		{
			Zero = 1,
			One,
			ScrColor,
			InvScrColor,
			ScrAlpha,
			InvScrAlpha,
			DestAlpha,
			InvDestAlpha,
			DestColor,
			InvDestColor,
			SrcAlphaSat,
			BlendFactor,
			InvBlendFactor,
			Src1Color,
			InvSrc1Color,
			Src1Alpha,
			InvSrc1Alpha,
			AlphaFactor,
			InvAlphaFactor
		};

		enum class BlendOperation : uint8_t
		{
			Add = 1,
			Substract,
			RevertSubstract,
			Min,
			Max
		};

		struct RenderTargetBlendDescription
		{
			bool BlendEnable = false;
			bool LogicOpEnable = false;
			Blend SrcBlend = Blend::One;
			Blend DestBlend = Blend::Zero;
			BlendOperation BlendOp = BlendOperation::Add;
			Blend SrcBlendAlpha = Blend::One;
			Blend DestBlendAlpha = Blend::Zero;
			BlendOperation BlendOpAlpha = BlendOperation::Add;
			LogicOperation LogicOp = LogicOperation::Noop;
			uint8_t RenderTargetWriteMask = 15;
		};

		struct BlendDescription
		{
			bool AlphaToCoverageEnable = false;
			bool IndependentBlendEnable = false;
			RenderTargetBlendDescription RenderTarget[8];
		};

		enum class FillMode : uint8_t
		{
			Wireframe = 0,
			Solid
		};

		enum class CullMode : uint8_t
		{
			None = 0,
			Front,
			Back
		};

		struct RasterizerDescription
		{
			FillMode fillMode = FillMode::Solid;
			CullMode cullMode = CullMode::Back;
			bool FrontCounterClockwise = false;
			int32_t DepthBias = 0;
			float DepthBiasClamp = 0.f;
			float SlopeScaledDepthBias = 0.f;
			bool DepthClipEnable = true;
			bool MultisampleEnable = false;
		};

		enum class ComparisonFunc : uint8_t
		{
			None = 0,
			Never,
			Less,
			Equal,
			LessEqual,
			Greater,
			NotEqual,
			GreaterEqual,
			Always
		};

		enum class StencilOp : uint8_t
		{
			Keep = 1,
			Zero,
			Replace,
			IncrSat,
			DecrSat,
			Invert,
			Incr,
			Decr
		};

		struct StencilOpDescription
		{
			StencilOp StencilFailOp = StencilOp::Keep;
			StencilOp StencilDepthFailOp = StencilOp::Keep;
			StencilOp StencilPassOp = StencilOp::Keep;
			ComparisonFunc StencilFunc = ComparisonFunc::Always;
		};

		struct DepthStencilDescription
		{
			bool DepthEnable = true;
			ComparisonFunc DepthFunc = ComparisonFunc::Less;
			bool StencilEnable = false;
			uint8_t StencilReadMask = 0xff;
			uint8_t StencilWriteMask = 0xff;
			StencilOpDescription FrontFace;
			StencilOpDescription BackFace;
		};

		enum class PrimitiveTopologyType : uint8_t
		{
			Undefined = 0,
			Point,
			Line,
			Triangle,
			Patch
		};

		enum class PrimitiveTopology : uint8_t
		{
			Undefined = 0,
			TriangleList
		};

		inline uint32_t GetFormatSize(const ResourceFormat& format)
		{
			switch (format)
			{
			case ResourceFormat::R16_UINT:
				return 2;
			case ResourceFormat::R16_UNORM:
				return 2;
			case ResourceFormat::RG32_FLOAT:
				return 8;
			case ResourceFormat::RGB32_FLOAT:
				return 12;
			default:
				ASSERT_NOT_IMPLEMENTED;
				return 0;
			}
		}

		// If we want to use it as bitfield, it cannot be enum.
		// Enum suggests that it is a data type.
		// If you bitwise OR two instances of the enum class data type, it will be no longer the same type
		// And I still want this params to leave in a namespace, that's why I used struct for that
		struct ClearFlags
		{
			using Flag = uint8_t;

			static const Flag Depth = 0x01;
			static const Flag Stencil = 0x02;
		};

		namespace Settings
		{
			constexpr ResourceFormat BackBuffer = ResourceFormat::RGBA8_UNORM;
		};
	}
}