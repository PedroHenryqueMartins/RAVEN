// Description: Simple shader that applies transformation matrices.

cbuffer TransformBuffer : register(b0)
{
	matrix world;
	matrix view;
	matrix projection;
}

struct VSInput
{
	float3 position : POSITION;
	float4 color : COLOR;
};

struct VSOutput
{
	float4 position : SV_Position;
	float4 color : COLOR;
};

VSOutput VS(VSInput input)
{
	float4 localPosition = float4(input.position, 1.0f);
	float4 worldPosition = mul(localPosition, world);
	float4 viewPosition = mul(worldPosition, view);
	float4 ndcPosition = mul(viewPosition, projection);

	VSOutput output;
	output.position = ndcPosition;
	output.color = input.color;
	return output;
}

float4 PS(VSOutput input) : SV_Target
{
	return input.color;
}