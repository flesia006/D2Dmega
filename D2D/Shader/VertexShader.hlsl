// Á¤Á¡
struct VertexInput
{
	float4 position : POSITION0; // Semantic Name
	float2 uv : TEXCOORD0; // Texture coordinate
};
struct PixelInput
{
	float4 position : SV_POSITION0; // System  Value
	float2 uv : TEXCOORD0;
};