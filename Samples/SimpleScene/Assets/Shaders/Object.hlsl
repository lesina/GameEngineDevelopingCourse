#include "BasicRootSignature.ihlsl"

cbuffer cbPerObject : register(b0)
{
	float4x4 gWorldViewProj;
};

cbuffer cbPerMaterial : register(b1)
{
	float4 Albedo;
}

Texture2D<float4> Texture : register(t0);
SamplerState StaticLinearSampler : register(s3);

struct VertexIn
{
	float3 PosL  : POSITION;
    float2 uv    : TEXCOORD0;
};

struct VertexOut
{
	float4 PosH  : SV_POSITION;
    float2 uv    : TEXCOORD0;
};

[RootSignature(BasicRootSignature)]
VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	
	// Transform to homogeneous clip space.
	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);
    vout.uv = vin.uv;
    
    return vout;
}

[RootSignature(BasicRootSignature)]
float4 PS(VertexOut pin) : SV_Target
{    
    return Albedo * Texture.Sample(StaticLinearSampler, pin.uv);
}