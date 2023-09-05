////////////////////////////////////////////////////
//  Color.hlsl
//  HLSL : High Level Shader Lanange
//         --> 과거에는 Assember
//         --> C하고 유사
//  * 사용되는 function명은 동일이름으로 쉐이더생성(프로그램)
//     해주어야 한다
//  * 전역변수 : Matrix등
///////////////////////////////////////////////////
// 정점
struct VertexInput
{
	float4 position : POSITION0; // Semantic Name
	float4 color    : COLOR0;
};
struct PixellInput
{
	float4 position : SV_POSITION0; // System  Value
	float4 color : COLOR0;
};
// Effect.hlsl에서 추가된 내용
// 상수Buffer를 받을 세이더 작성
cbuffer Transformbuffer : register(b0) // 0~13 슬롯에 등록 (4096*16바이트 이하 16배수)
{
	matrix world;     // float4x4 나자신에 대한 위치,스케일, 회전
	matrix view;      // 눈이 보는 방향에 대한
	matrix proj;      // Projectetor를 생각하자  직교사영
}
cbuffer ColorBuffer : register(b1)
{
    float4 TextColor; // 바뀔 Color값
    int Path; // switch문 사용시의 Tag
    float Time;
    float dummy1;
    float dummy2;
}
//////////////////////////////////////////////////////////
// Vertex  프로그램
// multiply --> mul (곱셈)
// https://blog.naver.com/ateliersera/222316810387
//////////////////////////////////////////////////////////
PixellInput VS(VertexInput input)
{
	PixellInput output = input;
	output.position = mul(input.position, world); // 월드변환
	output.position = mul(output.position, view); // 뷰변환
	output.position = mul(output.position, proj); // 프로젝션변환
	output.color = input.color;
	return output;
}
//////////////////////////////////////////////////////////
// Pixel  프로그램
//////////////////////////////////////////////////////////
float4 PS(PixellInput input) : SV_Target // 현재세팅된 메인렌더 타깃을 사용하라고 알려준다
{
    float4 returnColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
	
	[forcecase]
	
    switch (Path)
    {
        case 0:
            returnColor = input.color;
            break;
        case 1:
            returnColor = TextColor;
            break;	
    }
 
    return returnColor;

}