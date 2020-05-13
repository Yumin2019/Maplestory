#include "CCore.h"
#include "CTimer.h"
#include "CSceneManager.h"
#include "CCreateManager.h"
#include "CCollisionManager.h"
#include "CInput.h"
#include "CPencil.h"


DEFINITION_SINGLE(CCore);
bool CCore::m_bLoop;

CCore::CCore()
{
	//_CrtSetBreakAlloc(648);

}


CCore::~CCore()
{
	DESTROY_SINGLE(CTimer);
	DESTROY_SINGLE(CSceneManager);
	DESTROY_SINGLE(CCreateManager);
	DESTROY_SINGLE(CInput);
	DESTROY_SINGLE(CCollisionManager);
	DESTROY_SINGLE(CPencil);

	SelectObject(m_hBackDC, OldFont);
	DeleteObject(Font);

	DeleteDC(m_hBackDC);
	ReleaseDC(m_hWnd, m_hDC);
	DeleteObject(m_hBackBitmap);


	
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); 

}

bool CCore::Init(HINSTANCE hInst)
{
	m_hInst = hInst;

	m_bLoop = true;
	
	MyRegisterClass();

	InitInstance();


	// ȭ��� DC
	m_hDC = GetDC(m_hWnd);

	// ȭ��� DC�� ȣȯ�� �Ǵ� �޸� DC ����
	m_hBackDC = CreateCompatibleDC(m_hDC);

	// �޸� DC�� ��Ȯ�� ������ ����
	m_hBackBitmap = CreateCompatibleBitmap(m_hDC, RS_WIDTH, RS_HEIGHT);

	// ������ ������ ������ BackDC���� ����
	SelectObject(m_hBackDC, m_hBackBitmap);


	// �ð� ������ �ʱ�ȭ
	if (!GET_SINGLE(CTimer)->Init(m_hWnd))
		return false;

	// �Է� ������ �ʱ�ȭ
	if (!GET_SINGLE(CInput)->Init())
		return false;

	// ���� ������ �ʱ�ȭ
	if (!GET_SINGLE(CCreateManager)->Init(m_hInst, m_hBackDC))
		return false;

	// ��� ������ �ʱ�ȭ
	if (!GET_SINGLE(CSceneManager)->Init(m_hWnd))
		return false;

	// �浹 ������ �ʱ�ȭ
	if (!GET_SINGLE(CCollisionManager)->Init())
		return false;

	
	if (!GET_SINGLE(CPencil)->Init(m_hBackDC))
		return false;

	return true;
}

int CCore::Run()
{

	MSG msg;

	// �⺻ �޽��� �����Դϴ�.
	while (m_bLoop)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Logic();
		}
	}

	return (int)msg.wParam;
}

void CCore::Logic()
{
	// Timer ������Ʈ
	GET_SINGLE(CTimer)->Update();

	float fDeltaTime = GET_SINGLE(CTimer)->GetDeltaTime();

	Input(fDeltaTime);
	
	Collision(fDeltaTime);

	if (Update(fDeltaTime) == CS_CHANGE)
		return;

	Render(fDeltaTime);

}

ATOM CCore::MyRegisterClass()
{

	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = CCore::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hInst;
	wcex.hIcon = LoadIcon(m_hInst, MAKEINTRESOURCE(IDI_ICON1));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL; 
	wcex.lpszClassName = L"MapleStory";
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

	return RegisterClassExW(&wcex);
}

BOOL CCore::InitInstance()
{
	 m_hWnd = CreateWindowW(L"MapleStory", L"MapleStory", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, m_hInst, nullptr);

	if (!m_hWnd)
	{
		return FALSE;
	}

	RECT rc = { 0, 0, RS_WIDTH, RS_HEIGHT };

	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, rc.right - rc.left,
		rc.bottom - rc.top, SWP_NOZORDER);

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	return TRUE;
}

LRESULT CCore::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

			case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hWnd, &ps);
				EndPaint(hWnd, &ps);
			}
			break;
			case WM_DESTROY:
				m_bLoop = false;
				PostQuitMessage(0);
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}

	return 0;
}

void CCore::Input(float fDeltaTime)
{

	GET_SINGLE(CInput)->Update(fDeltaTime);
	GET_SINGLE(CSceneManager)->Input(fDeltaTime);
}

void CCore::Collision(float fDeltaTime)
{
	// ���� ���� Object�� CollisionManager�� �ű�� ����
	GET_SINGLE(CSceneManager)->Collision(fDeltaTime);

	// Collision Manager���� ������Ʈ�� ���� �浹 ó���� �ϴ� ����
	GET_SINGLE(CCollisionManager)->Collision(fDeltaTime);
}

int CCore::Update(float fDeltaTime)
{
	CHANGE_SCENE sc;
	sc = GET_SINGLE(CSceneManager)->Update(fDeltaTime);

	return sc;
}

void CCore::Render(float fDeltaTime)
{
	
	GET_SINGLE(CSceneManager)->Render(m_hBackDC, fDeltaTime);

	 //BackDC�� ���� ���� m_hDC�� �ű��.

	BitBlt(m_hDC, 0, 0, RS_WIDTH, RS_HEIGHT, m_hBackDC,  
		 0, 0, SRCCOPY );

	
	TCHAR strFPS[20];
	swprintf_s(strFPS, L"FPS : %.f", CTimer::m_fFPS);
	
	TextOut(m_hDC, RS_WIDTH - 100.f, RS_HEIGHT - 50.f, strFPS, lstrlen(strFPS));
}

