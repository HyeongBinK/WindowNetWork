
// MFC1View.cpp: CMFC1View 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MFC1.h"
#endif

#include "MFC1Doc.h"
#include "MFC1View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFC1View

IMPLEMENT_DYNCREATE(CMFC1View, CView)

BEGIN_MESSAGE_MAP(CMFC1View, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CMFC1View 생성/소멸

CMFC1View::CMFC1View() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CMFC1View::~CMFC1View()
{
}

BOOL CMFC1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CMFC1View 그리기

void CMFC1View::OnDraw(CDC* pDC)
{
	CMFC1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	/*CDC* pdc = GetDC();
	pdc->TextOutW(100, 50, _T("MFC Application"));
	ReleaseDC(pdc);

	CClientDC dc(this);
	dc.TextOutW(100, 100, _T("MFC Application"));

	pDC->TextOutW(100, 150, _T("MFC Application"));*/

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	/*CPoint pt1(100, 100), pt2(200, 200);
	pDC->MoveTo(pt1);
	pDC->LineTo(pt2);

	CRect rc1(300, 100, 400, 200);
	pDC->Rectangle(&rc1);

	CRect rc2(500, 100, 600, 200);
	pDC->Ellipse(&rc2);*/

	//pDC->Rectangle(m_Pt.x)

	pDC->Rectangle(m_Pt.x - 10, m_Pt.y - 10, m_Pt.x + 10, m_Pt.y + 10);
}


// CMFC1View 인쇄

BOOL CMFC1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMFC1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMFC1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CMFC1View 진단

#ifdef _DEBUG
void CMFC1View::AssertValid() const
{
	CView::AssertValid();
}

void CMFC1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFC1Doc* CMFC1View::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFC1Doc)));
	return (CMFC1Doc*)m_pDocument;
}
#endif //_DEBUG


// CMFC1View 메시지 처리기


void CMFC1View::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetCapture();
	m_ptPrev = point;

	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//CString str;
	//str.Format(_T("[%d, %d]"), point.x, point.y);
	//CClientDC dc(this);
	//dc.TextOutW(point.x, point.y, str);
	CView::OnLButtonDown(nFlags, point);
}


void CMFC1View::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	ReleaseCapture();

	CView::OnLButtonUp(nFlags, point);
}


void CMFC1View::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (GetCapture() != this) return;

	CClientDC dc(this);
	dc.MoveTo(m_ptPrev);
	dc.LineTo(point);
	m_ptPrev = point;

	CView::OnMouseMove(nFlags, point);
}


void CMFC1View::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_ViewSize = CSize(cx, cy);
	m_Pt = CPoint(cx * 0.5f, cy * 0.5f);
}


void CMFC1View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	switch (nChar)
	{
	case VK_UP:
		m_Pt.y -= 10;
		if (0 > m_Pt.y) m_Pt.y = m_ViewSize.cy;
		break;
	case VK_DOWN:
		m_Pt.y += 10;
		if (m_ViewSize.cy < m_Pt.y) m_Pt.y = 0;
		break;
	case VK_LEFT:
		m_Pt.x -= 10;
		if (0 > m_Pt.x) m_Pt.x = m_ViewSize.cx;
		break;
	case VK_RIGHT:
		m_Pt.x += 10;
		if (m_ViewSize.cx < m_Pt.x) m_Pt.x = 0;
		break;
	}
	Invalidate(); 

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
