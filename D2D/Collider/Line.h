#pragma once
class Line
{
public:
	Line();
	~Line();
public:
	void			Update(Matrix V, Matrix P);
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
	Vector2			GetStartPoint(int no);
	Vector2			GetEndPoint(int no);

private:
	struct Vertex
	{
		Vector3 Position;
		Color   Color;
	};
	vector<shared_ptr<Vector2>>    m_cvLines;  // (0,1) Line, (2,3)
	shared_ptr<LineShader>		   m_pShader;
	ID3D11Buffer*				   m_pVertexBuffer = nullptr;
};

