// Description: Simple shader that applies color.


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
	VSOutput output;
	output.position = float4(input.position, 1.0f);
	output.color = input.color;
	return output;
}

float4 PS(VSOutput input) : SV_Target
{
	return input.color;
}
