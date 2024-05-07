cbuffer cbPerObject : register(b0)
{
	float4x4 gWorldViewProj;
};

cbuffer cbPerMaterial : register(b1)
{
	float4 Albedo;
}

struct VertexIn
{
	float3 PosL  : POSITION;
};

struct VertexOut
{
	float4 PosH  : SV_POSITION;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	
	// Transform to homogeneous clip space.
	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);
    
    return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
    return Albedo;
}