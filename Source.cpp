#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment (lib,"d3d9.lib")
#pragma comment (lib,"d3dx9.lib")

LRESULT CALLBACK WinProc(HWND HWnd, UINT Msg, WPARAM Wparam, LPARAM Lparam);

WNDCLASSEX WindowsClass;
MSG Msg;
HWND HWnd;

LPDIRECT3D9 pDirect3D = 0;
LPDIRECT3DDEVICE9 pDirect3DDevice = 0;

HRESULT InitialDX(HWND hWnd);//������� ������������� DirectX
void Render();//������� ����������
void ReleaseDX();//������� ������������ ���������

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{

	WindowsClass.cbSize = sizeof(WindowsClass);//������ ���� ���������
	WindowsClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);//���� ���� ����
	WindowsClass.hCursor = LoadCursor(NULL, IDC_ARROW);//��� �������
	WindowsClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);//������ ����������
	WindowsClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);//������ � ��������� ����
	WindowsClass.hInstance = hInstance;//��������� ����������
	WindowsClass.lpfnWndProc = WinProc;//���� ������� ��������� �������
	WindowsClass.lpszClassName = "WindowsClass";//��� ������ ����
	WindowsClass.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;//������� ����� ����

	RegisterClassEx(&WindowsClass);//����������� ������ ���� � ��

	//�������� ������
	HWnd = CreateWindow(
		"WindowsClass",//��� ������ ���� � ����
		"EFT2",//�������� ����
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,//����� ����
		100, 100,//���������� ������ �������� ����
		600,//������ ����
		600,//������ ����
		NULL,//���������� ������������� ����
		NULL,//���������� ����
		hInstance,//���������
		NULL);//������������� ���������
	if (SUCCEEDED(InitialDX(HWnd)))
	{
		ShowWindow(HWnd, nShowCmd);//����� ����
		UpdateWindow(HWnd);//���������� ����

		ZeroMemory(&Msg, sizeof(Msg));//������� ���������

		while (Msg.message != WM_QUIT)//���� ��������� ���������
		{
			if (PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&Msg);
				DispatchMessage(&Msg);
			}
			else
			{
				Render();//��� ����������
			}
		}
	}
}

LRESULT CALLBACK WinProc(HWND HWnd, UINT Msg, WPARAM Wparam, LPARAM Lparam) //������� ��������� ���������
{
	switch (Msg)
	{
		case WM_DESTROY: //�������� ����
			{
				ReleaseDX();
				PostQuitMessage(0);
				return 0;
			}
	}
	return (DefWindowProc(HWnd, Msg, Wparam, Lparam));
}

HRESULT InitialDX(HWND hWnd)
{	
	pDirect3D = Direct3DCreate9(D3D_SDK_VERSION);//�������� ��������� ����������
	if (pDirect3D == NULL)
	{
		return E_FAIL;
	}

	D3DDISPLAYMODE Display;//��������� � ����������� ��������
	if (FAILED(pDirect3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &Display)))//��������� �������� ��������
	{
		return E_FAIL;
	}

	D3DPRESENT_PARAMETERS Direct3DParametr;//��������� � �����������
	ZeroMemory(&Direct3DParametr, sizeof(Direct3DParametr));//��������� ���������
	Direct3DParametr.Windowed = TRUE;//����� ����������� ���������� (true - �������, false - �������������)
	Direct3DParametr.SwapEffect = D3DSWAPEFFECT_DISCARD;//����� ������������ �������
	Direct3DParametr.BackBufferFormat = Display.Format;//������ ������� (���-�� �������� �����)
	Direct3DParametr.EnableAutoDepthStencil = TRUE;//���������� ������� �������
	Direct3DParametr.AutoDepthStencilFormat = D3DFMT_D16;//������ ������ �������
	Direct3DParametr.BackBufferWidth = Display.Width;//������ ������
	Direct3DParametr.BackBufferHeight = Display.Height;//������ ������
	//Direct3DParametr.FullScreen_RefreshRateInHz = Display.RefreshRate;// ������� ���������� ������ (��� �������������� ������)

	if (FAILED(pDirect3D->CreateDevice(
		D3DADAPTER_DEFAULT,//������������ ������
		D3DDEVTYPE_HAL,//��� ����������
		hWnd,//���������� ������������� ����
		D3DCREATE_HARDWARE_VERTEXPROCESSING,//����� ��������� ��������
		&Direct3DParametr,//��������� � �����������
		&pDirect3DDevice)))//��������� �� ����������
	{
		return E_FAIL;
	}
	return S_OK;
}

void Render()
{
	pDirect3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);//������� ���������� ������
	pDirect3DDevice->BeginScene();//������ ����������
	//��� ��������� �����
	pDirect3DDevice->EndScene();//����� ����������
	pDirect3DDevice->Present(NULL, NULL, NULL, NULL);//����������� ���������� ������
}

void ReleaseDX()
{
	if (pDirect3DDevice != NULL)
	{
		pDirect3DDevice->Release();
	}
	if (pDirect3D != NULL)
	{
		pDirect3D->Release();
	}
}