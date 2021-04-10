#include "Graphics.h"

Graphics::Graphics()
{
	m_factory = nullptr;
	m_renderTarget = nullptr;
}

Graphics::~Graphics()
{
	if (m_factory)	m_factory->Release();
	if (m_renderTarget)	m_renderTarget->Release();
	if (m_brush) m_brush->Release();
	if (m_trianglePath) m_trianglePath->Release();
}

bool Graphics::Init(HWND windowHandle)
{
	HRESULT result = D2D1CreateFactory(D2D1_FACTORY_TYPE::D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_factory);
	if (result != S_OK) return false;

	RECT rect;
	GetClientRect(windowHandle, &rect);
	D2D1_SIZE_U size = D2D1::SizeU(rect.right, rect.bottom);
	D2D1_HWND_RENDER_TARGET_PROPERTIES properties = D2D1::HwndRenderTargetProperties(windowHandle, size);

	result = m_factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
		properties,
		&m_renderTarget);
	if (result != S_OK) return false;

	result = m_renderTarget->CreateSolidColorBrush(D2D1::ColorF(0.99f, 0.7f, 0.6f), &m_brush);
	if (result != S_OK) return false;

	return true;
}

void Graphics::ClearScreen(float r, float g, float b)
{
	m_renderTarget->Clear(D2D1::ColorF(r, g, b));
}

void Graphics::ClearScreen(const COLORREF& color)
{
	D2D1::ColorF normalizedColor = D2D1::ColorF(GetRValue(color) / 256.f, GetGValue(color) / 256.f, GetBValue(color) / 256.f);
	m_renderTarget->Clear(normalizedColor);
}

void Graphics::DrawTriangle(float x, float y)
{
	ID2D1GeometrySink* pSink;
	HRESULT res = m_factory->CreatePathGeometry(&m_trianglePath);
	if (res == S_OK) {
		res = m_trianglePath->Open(&pSink);
		if (res == S_OK) {
			pSink->BeginFigure(
				D2D1::Point2F(x, y),
				D2D1_FIGURE_BEGIN_FILLED
			);
			pSink->AddLine(D2D1::Point2F(x - 50, y + 50));
			pSink->AddLine(D2D1::Point2F(x + 50, y + 50));
			pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
			res = pSink->Close();
		}
	}
	m_renderTarget->DrawGeometry(m_trianglePath, m_brush);
	m_renderTarget->FillGeometry(m_trianglePath, m_brush);
}

void Graphics::Resize(RECT rect)
{
	D2D1_SIZE_U size = D2D1::SizeU(rect.right, rect.bottom);
	m_renderTarget->Resize(size);
}
