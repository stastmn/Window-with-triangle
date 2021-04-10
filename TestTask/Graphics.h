#pragma once
#include <Windows.h>
#include <d2d1.h>


class Graphics
{
	ID2D1Factory* m_factory;
	ID2D1HwndRenderTarget* m_renderTarget;
	ID2D1PathGeometry* m_trianglePath;
	ID2D1SolidColorBrush* m_brush;
	

public:
	
	Graphics();
	~Graphics();
	bool Init(HWND windowHandle);


	void BeginDraw() { m_renderTarget->BeginDraw(); }
	void EndDraw() { m_renderTarget->EndDraw(); }

	void ClearScreen(float r,float g, float b);
	void ClearScreen(const COLORREF& color);
	void DrawTriangle(float x, float y);
	void Resize(RECT rect);

};

