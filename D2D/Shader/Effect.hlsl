/////////////////////////////////////
// Effect.hlsl
// HLSL : High Level Shader Language
//        1) 어셈블러
//        2) C언어 처럼 만들어짐
//            --> 컴파일 --> 바이너리
//////////////////////////////////////
// 정점
// float4  x,y,z,w(동차)
// 2D --> 3x3
// 3D --> 4x4 
struct VertexInput
{
	float4 Position : POSITION0;  // float4 Position(변수명) 시멘틱네임(0-16)
	float4 Color:     COLOR0;
	// float4 Position2 : POSITION1;  만약 정점을 2개 Define한 경우
};
struct PixelInput
{
	float4 Position : SV_POSITION0;  // SV --> System Value (좀더 중요한 의미)
	float4 Color:     COLOR0;
};

//////////////////////////////////////////////////
// Vertex Shader 출력 프로 그램
// 정점으로 만든 데이터가 GPU계산한후 이프로 그램을
// 통과
//////////////////////////////////////////////////
PixelInput VS(VertexInput input)
{
	PixelInput output = input;

//	output.Position.x = 1.0f;
//	output.Position.y = 0.8f;
	return output;
}
///////////////////////////////////////////////////
// Pixel Shader 프로그램
///////////////////////////////////////////////////
float4 PS(PixelInput input) : SV_TARGET0
{
	float4 value = float4(1.0f,0.0f,1.0f,1.0f);
	return input.Color;
}