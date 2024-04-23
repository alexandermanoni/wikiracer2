// WikiRacerApp.cpp : Defines the entry point for the application.
// This website was used as a reference: https://learn.microsoft.com/en-us/cpp/windows/walkthrough-creating-windows-desktop-applications-cpp?view=msvc-170#build-the-code 


#include "framework.h"
#include "WikiRacerApp.h"
#include "WikiGraph.h"
#define MAX_LOADSTRING 100

HINSTANCE hInst;                         
WCHAR szTitle[MAX_LOADSTRING];              
WCHAR szWindowClass[MAX_LOADSTRING];         

WikiGraph graph;

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

WikiRacerApp::WikiRacerApp() {
}

WikiRacerApp::~WikiRacerApp() {
}

void PerformSearch(HWND hWnd) {
    TCHAR from[100], to[100];
    GetWindowText(GetDlgItem(hWnd, IDC_FROM_EDIT), from, 100);
    GetWindowText(GetDlgItem(hWnd, IDC_TO_EDIT), to, 100);

    std::wstring toW = to;
    std::string toS = std::string(toW.begin(), toW.end());

    std::wstring fromW = from;
    std::string fromS = std::string(fromW.begin(), fromW.end());

    unsigned int steps = graph.bfs(graph.map.find(fromS)->second, graph.map.find(toS)->second).size() - 1;
    
    TCHAR result[100];
    _stprintf_s(result, _T("Using breadth first search, it takes %u steps to get from '%s' to '%s'!"), steps, from, to);
    MessageBox(hWnd, result, _T("Search Result"), MB_OK | MB_ICONINFORMATION);
}

void PerformDJISearch(HWND hWnd) {
    TCHAR from[100], to[100];
    GetWindowText(GetDlgItem(hWnd, IDC_FROM_EDIT), from, 100);
    GetWindowText(GetDlgItem(hWnd, IDC_TO_EDIT), to, 100);

    std::wstring toW = to;
    std::string toS = std::string(toW.begin(), toW.end());

    std::wstring fromW = from;
    std::string fromS = std::string(fromW.begin(), fromW.end());

    unsigned int steps = graph.dijkstra(fromS, toS);

    TCHAR result[100];
    _stprintf_s(result, _T("Using Djikstra's Algorithm, it takes %u steps to get from '%s' to '%s'!"), steps, from, to);
    MessageBox(hWnd, result, _T("Search Result"), MB_OK | MB_ICONINFORMATION);
}


void LoadData(HWND hWnd) {
    graph.load_data("myfile.txt");
    MessageBox(hWnd, _T("Data Loaded"), _T("Load Data"), MB_OK | MB_ICONINFORMATION);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WIKIRACERAPP, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance(hInstance, nCmdShow)) {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIKIRACERAPP));

    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0)) {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance) {
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIKIRACERAPP));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WIKIRACERAPP);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
    hInst = hInstance; 

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd) {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_COMMAND: {
        int wmId = LOWORD(wParam);

        switch (wmId) {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        case ID_SEARCH:
            PerformSearch(hWnd);
            break;
        case ID_LOAD_DATA:
            LoadData(hWnd);
        case DJI_SEARCH:
            PerformDJISearch(hWnd);
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;

    case WM_CREATE: {
        
        CreateWindow(TEXT("STATIC"), TEXT("From:"), WS_VISIBLE | WS_CHILD, 10, 10, 50, 20, hWnd, NULL, NULL, NULL);
        CreateWindow(TEXT("STATIC"), TEXT("To:"), WS_VISIBLE | WS_CHILD, 10, 40, 50, 20, hWnd, NULL, NULL, NULL);

        
        CreateWindow(TEXT("EDIT"), TEXT(""), WS_VISIBLE | WS_CHILD | WS_BORDER, 60, 10, 150, 20, hWnd, (HMENU)IDC_FROM_EDIT, NULL, NULL);
        CreateWindow(TEXT("EDIT"), TEXT(""), WS_VISIBLE | WS_CHILD | WS_BORDER, 60, 40, 150, 20, hWnd, (HMENU)IDC_TO_EDIT, NULL, NULL);
        CreateWindow(TEXT("BUTTON"), TEXT("BFS Search"), WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 60, 70, 150, 30, hWnd, (HMENU)ID_SEARCH, NULL, NULL);
        CreateWindow(TEXT("BUTTON"), TEXT("Djikstra's Search"), WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 60, 100, 150, 30, hWnd, (HMENU)DJI_SEARCH, NULL, NULL);
        CreateWindow(TEXT("BUTTON"), TEXT("Load Data"), WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 60, 150, 150, 30, hWnd, (HMENU)ID_LOAD_DATA, NULL, NULL);
    }
    break;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
    }
    break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    UNREFERENCED_PARAMETER(lParam);
    switch (message) {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
