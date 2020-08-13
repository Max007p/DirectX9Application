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

HRESULT InitialDX(HWND hWnd);//Функция инициализации DirectX
void Render();//Функция рендеринга
void ReleaseDX();//Функция освобождения устройств

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{

	WindowsClass.cbSize = sizeof(WindowsClass);//Размер всей структуры
	WindowsClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);//Цвет фона окна
	WindowsClass.hCursor = LoadCursor(NULL, IDC_ARROW);//Вид курсора
	WindowsClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);//Иконка приложения
	WindowsClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);//Иконка в загаловке окна
	WindowsClass.hInstance = hInstance;//Экземпляр приложения
	WindowsClass.lpfnWndProc = WinProc;//Наша функция обработки событий
	WindowsClass.lpszClassName = "WindowsClass";//Имя класса окна
	WindowsClass.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;//Задание стиля окна

	RegisterClassEx(&WindowsClass);//Регистрация класса окна в ОС

	//Создание класса
	HWnd = CreateWindow(
		"WindowsClass",//Имя класса окна в коде
		"EFT2",//Название окна
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,//Стиль окна
		100, 100,//Координаты левого верхнего угла
		600,//Ширина окна
		600,//Высота окна
		NULL,//Дескриптор родительского окна
		NULL,//Дескриптор меню
		hInstance,//Экземпляр
		NULL);//Доплнительные параметры
	if (SUCCEEDED(InitialDX(HWnd)))
	{
		ShowWindow(HWnd, nShowCmd);//Вывод окна
		UpdateWindow(HWnd);//Обновление окна

		ZeroMemory(&Msg, sizeof(Msg));//Очистка сообщений

		while (Msg.message != WM_QUIT)//Цикл обработки сообщений
		{
			if (PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&Msg);
				DispatchMessage(&Msg);
			}
			else
			{
				Render();//Код выполнения
			}
		}
	}
}

LRESULT CALLBACK WinProc(HWND HWnd, UINT Msg, WPARAM Wparam, LPARAM Lparam) //функция обработки сообщений
{
	switch (Msg)
	{
		case WM_DESTROY: //закрытие окна
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
	pDirect3D = Direct3DCreate9(D3D_SDK_VERSION);//Создание основного интерфейса
	if (pDirect3D == NULL)
	{
		return E_FAIL;
	}

	D3DDISPLAYMODE Display;//Структура с настройками монитора
	if (FAILED(pDirect3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &Display)))//Получение настроек монитора
	{
		return E_FAIL;
	}

	D3DPRESENT_PARAMETERS Direct3DParametr;//Структура с настройками
	ZeroMemory(&Direct3DParametr, sizeof(Direct3DParametr));//Обнуление структуры
	Direct3DParametr.Windowed = TRUE;//Режим отображения приложения (true - оконное, false - полноэкранное)
	Direct3DParametr.SwapEffect = D3DSWAPEFFECT_DISCARD;//Метод переключения буферов
	Direct3DParametr.BackBufferFormat = Display.Format;//Формат пикселя (кол-во разрядов цвета)
	Direct3DParametr.EnableAutoDepthStencil = TRUE;//Управление буфером глубины
	Direct3DParametr.AutoDepthStencilFormat = D3DFMT_D16;//Формат буфера глубины
	Direct3DParametr.BackBufferWidth = Display.Width;//Ширина буфера
	Direct3DParametr.BackBufferHeight = Display.Height;//Высота буфера
	//Direct3DParametr.FullScreen_RefreshRateInHz = Display.RefreshRate;// Частота обновления экрана (для полноэкранного режима)

	if (FAILED(pDirect3D->CreateDevice(
		D3DADAPTER_DEFAULT,//Используемая видюха
		D3DDEVTYPE_HAL,//Тип устройства
		hWnd,//Дескриптор родительского окна
		D3DCREATE_HARDWARE_VERTEXPROCESSING,//Режим обработки шейдеров
		&Direct3DParametr,//Структура с настройками
		&pDirect3DDevice)))//Указатель на устройство
	{
		return E_FAIL;
	}
	return S_OK;
}

void Render()
{
	pDirect3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);//Очистка вторичного буфера
	pDirect3DDevice->BeginScene();//Начало рендеринга
	//Код отрисовки сцены
	pDirect3DDevice->EndScene();//Конец рендеринга
	pDirect3DDevice->Present(NULL, NULL, NULL, NULL);//Копирование вторичного буфера
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