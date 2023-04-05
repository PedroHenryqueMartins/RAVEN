// Description: Outline shader ToonEffect in RAVEN.

cbuffer SettingsBuffer : register(b1)
{
	float lineThickness;
}

Texture2D textureMap0 : register(t0);
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
	VSOutput output;
	output.position = float4(input.position, 1.0f);
	output.texCoord = input.texCoord;
	return output;
}

float4 PS(VSOutput input) : SV_Target
{
	float4 color = (float4)0;
	float4 textureColor = textureMap0.Sample(textureSampler, input.texCoord);

	static float kernalV[9] =
	{
		1, 0, -1,
		2, 0, -2,
		1, 0, -1
	};

	static float kernalH[9] =
	{
		-1, -2, -1,
		0, 0, 0,
		1, 2, 1
	};

	static float u = 1.0f / 1280.0f;
	static float v = 1.0f / 720.0f;

	static float2 offset[] =
	{
		{ -u, -v}, {  0, -v}, { +u, -v},
		{ -u,  0}, {  0,  0}, { +u,  0},
		{ -u, +v}, {  0, +v}, { +u, +v}
	};

	for (int i = 0; i < 9; ++i)
	{
		color += textureMap0.Sample(textureSampler, input.texCoord + offset[i] * 1.0f) * kernalV[i];
		color += textureMap0.Sample(textureSampler, input.texCoord + offset[i] * 1.0f) * kernalH[i];
	}
	float strength = (color.r + color.g + color.b) / 3.0f;
	color = (strength > 0.4f) ? 0 : 1;
	textureColor = textureColor * color;

	return textureColor;
}