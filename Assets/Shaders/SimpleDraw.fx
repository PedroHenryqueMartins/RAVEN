// Description: Shader for SimpleDraw.

cbuffer TransformBuffer : register(b0)
{
	matrix wvp;
}

struct VSInput
{
	float3 position : POSITION;
	float4 color : COLOR;
};

struct VSOutput
{
	float4 position : SV_Position;
	float4 color : COLOR0;
};

VSOutput VS(VSInput input)
{
	float4 localPosition = float4(input.position, 1.0f);
	float4 ndcPosition = mul(localPosition, wvp);

	VSOutput output;
	output.position = ndcPosition;
	output.color = input.color;
	return output;
}

float4 PS(VSOutput input) : SV_Target
{
	return input.color;
}