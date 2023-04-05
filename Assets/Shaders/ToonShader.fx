// Description: Toon shader for RAVEN engine.

static float4 finalColor = (float4)0.0f;

cbuffer TransformBuffer : register(b0)
{
	matrix world;
	matrix wvp[2];
	float3 viewPosition;
}

cbuffer BoneTransformBuffer : register(b1)
{
	matrix boneTransforms[256];
}

cbuffer LightBuffer : register(b2)
{
	float4 lightAmbient = 0.0f;
	float4 lightDiffuse;
	float4 lightSpecular = 0.0f;
	float3 lightDirection;
}

cbuffer SettingsBuffer : register(b3)
{
	int numSegments;
	bool useSkinning;
}

Texture2D diffuseMap : register(t0);

SamplerState textureSampler : register(s0);

static matrix Identity =
{
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
};

matrix GetBoneTransform(int4 indices, float4 weights)
{
	if (length(weights) <= 0.0f)
	{
		return Identity;
	}

	matrix transform = boneTransforms[indices[0]] * weights[0];
	transform += boneTransforms[indices[1]] * weights[1];
	transform += boneTransforms[indices[2]] * weights[2];
	transform += boneTransforms[indices[3]] * weights[3];

	return transform;
}

struct VSInput
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float2 texCoord : TEXCOORD;
	int4 blendIndices : BLENDINDICES;
	float4 blendWeights : BLENDWEIGHT;
};

struct VSOutput
{
	float4 position : SV_Position;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float2 texCoord : TEXCOORD1;
	float4 lightNDCPosition : TEXCOORD2;
};

VSOutput VS(VSInput input)
{
	float4 localPosition = float4(input.position, 1.0f);

	matrix toWorld = world;
	matrix toNDC = wvp[0];
	matrix toLightNDC = wvp[1];

	if (useSkinning)
	{
		matrix boneTransform = GetBoneTransform(input.blendIndices, input.blendWeights);
		toWorld = mul(boneTransform, toWorld);
		toNDC = mul(boneTransform, toNDC);
		toLightNDC = mul(boneTransform, toLightNDC);
	}

	float4 worldPosition = mul(localPosition, toWorld);
	float4 ndcPosition = mul(localPosition, toNDC);

	float3 worldNormal = mul(input.normal, (float3x3)toWorld);
	float3 worldTangent = mul(input.tangent, (float3x3)toWorld);

	float3 normal = input.normal;

	VSOutput output;
	output.position = ndcPosition;
	output.normal = worldNormal;
	output.tangent = worldTangent;
	output.texCoord = input.texCoord;
	output.lightNDCPosition = mul(localPosition, toLightNDC);
	return output;
}

float4 PS(VSOutput input) : SV_Target
{
	//	Toon base color calculations
	float3 normal = normalize(input.normal);	
	float3 dirToLight = -lightDirection;
	float3 nLightDir = normalize(dirToLight);

	float intensity = dot(nLightDir, normal);
	float4 diffuseMapColor = diffuseMap.Sample(textureSampler, input.texCoord) * lightDiffuse;
	float4 color = (float4)0.0f;
	float4 outLiner = (float4)0.0f;

	if (intensity < 0)
	{
		intensity = 0;
	}

	if (numSegments == 2)
	{
		if (intensity > 0.95)
		{
			color = float4(1.0, 0.5, 0.5, 1.0) * diffuseMapColor;
			diffuseMapColor += color;
		}
		else
		{
			color = float4(0.2, 0.1, 0.1, 1.0) * diffuseMapColor;
			diffuseMapColor += color;
		}
	}

	if (numSegments == 3)
	{
		if (intensity > 0.95)
		{
			color = float4(1.0, 0.5, 0.5, 1.0) * diffuseMapColor;
			diffuseMapColor += color;
		}
		else if (intensity > 0.5)
		{
			color = float4(0.6, 0.3, 0.3, 1.0) * diffuseMapColor;
			diffuseMapColor += color;
		}
		else
		{
			color = float4(0.2, 0.1, 0.1, 1.0) * diffuseMapColor;
			diffuseMapColor += color;
		}
	}
	
	if (numSegments == 4)
	{
		if (intensity > 0.95)
		{
			color = float4(1.0, 0.5, 0.5, 1.0) * diffuseMapColor;
			diffuseMapColor += color;
		}
		else if (intensity > 0.5)
		{
			color = float4(0.6, 0.3, 0.3, 1.0) * diffuseMapColor;
			diffuseMapColor += color;
		}
		else if (intensity > 0.25)
		{
			color = float4(0.4, 0.2, 0.2, 1.0) * diffuseMapColor;
			diffuseMapColor += color;
		}
		else
		{
			color = float4(0.2, 0.1, 0.1, 1.0) * diffuseMapColor;
			diffuseMapColor += color;
		}
	}

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
		outLiner += diffuseMap.Sample(textureSampler, input.texCoord + offset[i] * 1.0f) * kernalV[i];
		outLiner += diffuseMap.Sample(textureSampler, input.texCoord + offset[i] * 1.0f) * kernalH[i];
	}

	float strength = (outLiner.r + outLiner.g + outLiner.b) / 3.0f;
	outLiner = (strength > 0.4f) ? 0 : 1;

	//diffuseMapColor = diffuseMapColor * outLiner;
	return diffuseMapColor;
}