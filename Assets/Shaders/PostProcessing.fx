// Description: Post processing shader for RAVEN.

cbuffer SettingsBuffer : register(b0)
{
	int mode;
	float timer;
	float scale;
	float blursize;
}

Texture2D textureMap0 : register(t0);
Texture2D textureMap1 : register(t1);
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
	if (mode == 0)
	{
		return textureColor;
	}
	else if (mode == 1)
	{
		textureColor = textureColor.r + textureColor.g + textureColor.b / 3.0f;
	}
	else if (mode == 2)
	{
		textureColor = 1.0f - textureColor;
	}
	else if (mode == 3)
	{
		textureColor.r = 0.0f;
		textureColor.b = 0.0f;

		float weight = abs(sin(input.texCoord.y * 350 + (timer * 14.0f)));
		textureColor *= weight;
	}
	else if (mode == 4)
	{
		if (scale == 0.0f)
		{
			return textureColor;
		}
		input.texCoord.x = floor((input.texCoord.x - 0.5f)/ scale) * scale;
		input.texCoord.y = floor((input.texCoord.y - 0.5f)/ scale) * scale;
		textureColor = textureMap0.Sample(textureSampler, input.texCoord + 0.5f);
	}
	else if (mode == 5)
	{
		
		float TwoPi = 6.28318530718f;
		float direction = 16.0f; 
		float quality = 8.0f; 

		float2 radius = blursize / input.position;

		for (float i = 0.0f; i < TwoPi; i += TwoPi / direction)
		{
			for (float j = 1.0f / quality; j <= 1.0f; j += 1.0f / quality)
			{
				textureColor += textureMap0.Sample(textureSampler, input.texCoord + float2(cos(i), sin(i)) * radius * j);
			}
		}

		textureColor /= quality * direction - 15.0f;
	}
	else if (mode == 6)
	{
		textureColor += textureMap1.Sample(textureSampler, input.texCoord);
	}
	else if (mode == 7) // Edge Detection
	{
		// Way I want to apply
		static float kernalV[9] =
		{
			3, 0, -3,
			10, 0, -10,
			3, 0, -3
		};

		static float kernalH[9] =
		{
			-3, -10, -3,
			0, 0, 0,
			3, 10, 3
		};

		// Get the window size
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
			color += textureMap0.Sample(textureSampler, input.texCoord + offset[i]) * kernalV[i];
			color += textureMap0.Sample(textureSampler, input.texCoord + offset[i]) * kernalH[i];
		}
		float strength = (color.r + color.g + color.b) / 3.0f;
		color = (strength > 0.4f) ? 0 : 1;
		//color *= textureColor;
		//textureColor = strength;
		textureColor = textureColor * color;
	}
	
	return textureColor;
}