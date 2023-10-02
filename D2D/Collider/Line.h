#pragma once
class Line
{
public:
	Line();
	~Line();
public:
	void			Update();
	void			Render();


private:
	void			CreateVertexBuffer();
	bool			LoadLine(string filename);
	bool			SaveLine(string filename);
public: 
	void			AddLine(float x1, float y1, float x2, float y2);
	void			ClearLine();
	void			EndLine();
	int				GetLineCount();
	vector<pair<Vector2, Vector2>> GetLine() {return m_cvLines;	}
//	Vector2			GetStartPoint(int no);
//	Vector2			GetEndPoint(int no);

private:
	struct Vertex
	{
		Vector3 Position;
		Color   Color;
	};
	vector<pair<Vector2, Vector2>> m_cvLines;
	class LineShader* m_pShader = nullptr;
	ID3D11Buffer*				   m_pVertexBuffer = nullptr;
};

