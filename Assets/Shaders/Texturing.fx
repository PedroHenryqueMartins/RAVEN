// Description: Texturing shader for texture mapping.

cbuffer TransformBuffer : register(b0)
{
	matrix wvp;
}

Texture2D textureMap : register(t0);
SamplerState textureSampler : register(s0);

struct VSInput
{
	float3 position : POSITION;
	float2 texCoord : TEXCOORD;
};

struct VSOutput
{
	float4 position : SV_Position;
	float2 texCoord	: TEXCOORD;
};

VSOutput VS(VSInput input)
{
	float4 localPosition = float4(input.position, 1.0f);
	float4 ndcPosition = mul(localPosition, wvp);

	VSOutput output;
	output.position = ndcPosition;
	output.texCoord = input.texCoord;
	return output;
}

float4 PS(VSOutput input) : SV_Target
{
	float4 textureColor = textureMap.Sample(textureSampler, input.texCoord);
	return textureColor;
}