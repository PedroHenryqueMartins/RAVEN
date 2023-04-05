
// Description: Standard shader for RAVEN engine.

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
	float4 lightAmbient;
	float4 lightDiffuse;
	float4 lightSpecular;
	float3 lightDirection;
}

cbuffer MaterialBuffer : register(b3)
{
	float4 materialAmbient;
	float4 materialDiffuse;
	float4 materialSpecular;
	float4 materialEmissive;
	float materialPower;
}

cbuffer SettingsBuffer : register(b4)
{
	// Note: A bool in HLSL is 4 bytes
	bool useDiffuseMap;
	bool useSpecularMap;
	bool useNormalMap;
	float displacementScale;

	bool useShadowMap;
	bool useSkinning;
	float depthBias;
}

Texture2D diffuseMap : register(t0);
Texture2D specularMap : register(t1);
Texture2D displacementMap : register(t2);
Texture2D normalMap : register(t3);
Texture2D shadowMap : register(t4);


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
	float3 binormal : BINORMAL;
	float3 dirToView : TEXCOORD0;	//	Cheat to hide it as a texCoord.
	float2 texCoord : TEXCOORD1;
	float4 lightNDCPosition : TEXCOORD2;
};

VSOutput VS(VSInput input)
{
	float displacement = displacementMap.SampleLevel(textureSampler, input.texCoord, 0).r;

	float4 localPosition = float4(input.position + (input.normal * displacement * displacementScale), 1.0f);

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
	float worldBinormal = cross(worldNormal, worldTangent);


	VSOutput output;
	output.position = ndcPosition;
	output.normal = worldNormal;
	output.tangent = worldTangent;
	output.binormal = worldBinormal;
	output.dirToView = viewPosition - worldPosition.xyz;
	output.texCoord = input.texCoord;
	output.lightNDCPosition = mul(localPosition, toLightNDC);
	return output;
}

float4 PS(VSOutput input) : SV_Target
{
	//	Renormalize normal vectors after being interpolated by the rasterizer.
	float3 normal = normalize(input.normal);	
	float3 dirToLight = -lightDirection;
	float3 dirToView = normalize(input.dirToView);

	if (useNormalMap)
	{
		float3 binormal = normalize(input.binormal);
		float3 tangent = normalize(input.tangent);
		float3x3 tbnw = float3x3(tangent, binormal, normal);

		float4 normalMapColor = normalMap.Sample(textureSampler, input.texCoord);

		float3 unpackedNormal = float3(normalMapColor.xyz * 2.0f) - 1.0f;

		normal = normalize(mul(unpackedNormal, tbnw));
	}
	
	
	float4 ambientColor = lightAmbient * materialAmbient;

	float diffuseIntensity = saturate(dot(dirToLight, normal));
	float4 diffuseColor = diffuseIntensity * lightDiffuse * materialDiffuse;

	float3 reflected = reflect(lightDirection, normal);
	float specularBase = saturate(dot(dirToView, reflected));
	float spcularIntensity = pow(specularBase, materialPower);
	float4 specularColor = spcularIntensity * lightSpecular * materialSpecular;


	float4 diffuseMapColor = 1.0f;
	if(useDiffuseMap)
		diffuseMapColor = diffuseMap.Sample(textureSampler, input.texCoord);

	float4 specularMapColor = 1.0f;
	if(useSpecularMap)
		specularMapColor = specularMap.Sample(textureSampler, input.texCoord).r;

	float4 finalColor = (ambientColor + diffuseColor + materialEmissive) * diffuseMapColor + (specularColor * specularMapColor);

	if (useShadowMap)
	{
		float actualDepth = 1.0f - (input.lightNDCPosition.z / input.lightNDCPosition.w);
		float2 shadowMapUV = input.lightNDCPosition.xy / input.lightNDCPosition.w;
		float u = (shadowMapUV.x + 1.0f) * 0.5f;
		float v = 1.0f - ((shadowMapUV.y + 1.0f) * 0.5f);
		if (saturate(u) == u && saturate(v) == v)
		{
			float4 savedColor = shadowMap.Sample(textureSampler, float2(u, v));
			float savedDepth = savedColor.x;
			if (savedDepth > actualDepth + depthBias)
			{
				finalColor = ambientColor * diffuseMapColor;
			}
		}
	}
	return finalColor;
}