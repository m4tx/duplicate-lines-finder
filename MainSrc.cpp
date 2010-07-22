// Duplicate Lines Finder
// Author: m4tx
// www.m4tx.2ap.pl
// 07.15.2010 09:29:16am

#include <windows.h> // Okna
#include <commctrl2.h> // Toolbary, tooltipy, paski itd
#include <fstream> // Pliki
#include <string> // Ci¹gi znaków
#include <vector> // Tablice dynamiczne
#include <math.h> // Matematyka

#include <baloon.h>

//#define ID_pbMainProgressBarTimer 100

#define TTS_BALOON 0x40
#define TTM_SETTITLE WM_USER+32
#define MYTOOLTIPTEST 0x00DEAD00

#define TTS_ICONINFORMATION 1

#define ID_FILE_EXIT     		100
#define ID_HELP_ABOUT			101

CONST CHAR MainForm[] = "MainForm";

MSG Message;
WPARAM hNormalFont = (WPARAM)(HFONT)GetStockObject(DEFAULT_GUI_FONT);

HWND MainFormHwnd;
HWND eInputEdit;
HWND bInputButton;
HWND eOutputEdit;
HWND bOutputButton;
HWND bStart;
//HWND pbMainProgressBar;
HWND eSeparator;

HWND lSavedSpace;
HWND lRepeatedLines;
HWND lTime;

WNDPROC MainHwndEditsOldWndProc;

HWND lAuthorUrl; // Etykieta strony autora
HWND AboutHwnd; // Okno o programie

WNDPROC lAuthorUrlOldWndProc;
WNDPROC AboutHwndOldWndProc;

WNDCLASSEX AboutWc = {0};

HINSTANCE MainInstance;

struct MYTOOLTIP
{
    DWORD dwTest;
    LPSTR szTip;
};

LRESULT CALLBACK WndProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK MainHwndEditsWndProc (HWND hwnd, UINT mesg, WPARAM wParam, LPARAM lParam);
void CreateTooltip (HWND hParent, LPCSTR Text, LPCSTR Title);
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX MainWndClass;

    MainWndClass.cbSize = sizeof (WNDCLASSEX); // Rozmiar struktury w bajtach
    MainWndClass.style = 0; // Style klasy
    MainWndClass.lpfnWndProc = WndProc; // WskaŸnik do procedury obs³uguj¹cej okno
    MainWndClass.cbClsExtra = 0; // Dodatkowe bajty pamiêci dla klasy
    MainWndClass.cbWndExtra = 0; // Dodatkowe bajty pamiêci dla klasy
    MainWndClass.hInstance = hInstance; // 	 Identyfikator aplikacji, która ma byæ w³aœcicielem okna
    MainWndClass.hIcon = LoadIcon (hInstance, "APPICON"); // Ikona okna
    MainWndClass.hIconSm = LoadIcon (hInstance, "APPICON"); // Ikona okna
    MainWndClass.hCursor = LoadCursor (NULL, IDC_ARROW); // Kursor okna
    MainWndClass.hbrBackground = GetSysColorBrush (COLOR_3DFACE); // T³o okna
    MainWndClass.lpszMenuName = NULL; // Nazwa identyfikuj¹ca menu okna
    MainWndClass.lpszClassName = MainForm; // Nazwa klasy

    if (!RegisterClassEx (&MainWndClass))
    {
        MessageBox (NULL, "Nie mo¿na utworzyæ okna: b³¹d przy rejestrowaniu klasy!", "B³¹d", MB_OK | MB_ICONERROR);
        return 1;
    }

    MainInstance = hInstance;

    //==================================================================================================
    // Menu programu

    HMENU hMenu, hSubMenu; // utworzenie zmiennych hMenu i hSubMenu

    hMenu = CreateMenu(); // Zainicjowanie zmiennej hMenu

    hSubMenu = CreatePopupMenu(); // Zainicjowanie zmiennej hSubMenu
    AppendMenu(hSubMenu, MF_STRING, ID_FILE_EXIT, "&Zakoñcz"); // Dodanie pozycji Zakoñcz
    AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "&Program"); // Dodanie menu SmsPrice

    hSubMenu = CreatePopupMenu(); // Zainicjowanie zmiennej hSubMenu
    AppendMenu(hSubMenu, MF_STRING, ID_HELP_ABOUT, "&O programie"); // Dodanie pozycji O programie
    AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "P&omoc"); // Dodanie menu Pomoc

    //==================================================================================================
    // G³ówne okno

    MainFormHwnd = CreateWindowEx (
        NULL,
        MainForm,
        "Duplicate Lines Finder",
        WS_SYSMENU | WS_BORDER | WS_CAPTION | WS_VISIBLE | WS_MINIMIZEBOX,
        200,
        200,
        300,
        245,
        NULL,
        NULL,
        hInstance,
        NULL);

    SetMenu(MainFormHwnd, hMenu); // Ustawienie menu dla okna
    InitCommonControls();

    //==================================================================================================
    // Etykieta dla pola tekstowego pliku wejœcia

    HWND lInputLabel = CreateWindowEx (
        NULL,
        "STATIC",
        "Wybierz plik, który przetworzyæ:",
        WS_CHILD | WS_VISIBLE,
        5,
        5,
        285,
        20,
        MainFormHwnd,
        NULL,
        hInstance,
        NULL);
    SendMessage(lInputLabel, WM_SETFONT, hNormalFont, 0);

    //==================================================================================================
    // Pole tekstowe pliku wejœcia

    eInputEdit = CreateWindowEx (
        WS_EX_CLIENTEDGE,
        "EDIT",
        NULL,
        WS_CHILD | WS_VISIBLE,
        5,
        25,
        245,
        20,
        MainFormHwnd,
        NULL,
        hInstance,
        NULL);
    SendMessage(eInputEdit, WM_SETFONT, hNormalFont, 0);
    CreateTooltip (eInputEdit,
    "Wpisz tutaj pe³n¹ œcie¿kê pliku, z którego maj¹ zostaæ usuniête duplikaty, lub skorzystaj z przycisku po prawej stronie.",
    "Informacja");
    MainHwndEditsOldWndProc = (WNDPROC) SetWindowLong (eInputEdit, GWL_WNDPROC, (LONG)MainHwndEditsWndProc);
    SetWindowLong (eInputEdit, GWL_WNDPROC, (LONG)MainHwndEditsWndProc);

    //==================================================================================================
    // Przycisk dla pola tekstowego pliku wejœcia

    bInputButton = CreateWindowEx (
        NULL,
        "BUTTON",
        "...",
        WS_CHILD | WS_VISIBLE,
        255,
        25,
        30,
        20,
        MainFormHwnd,
        NULL,
        hInstance,
        NULL);
    SendMessage(bInputButton, WM_SETFONT, hNormalFont, 0);
    CreateTooltip (bInputButton, "Wciœnij ten przycisk, aby wybraæ plik, z którego maj¹ zostaæ usuniête duplikaty.", "Informacja");

    //==================================================================================================
    // Etykieta dla pola tekstowego pliku wejœcia

    HWND lOutputLabel = CreateWindowEx (
        NULL,
        "STATIC",
        "Wybierz plik, do którego zapisaæ wynik:",
        WS_CHILD | WS_VISIBLE,
        5,
        55,
        285,
        20,
        MainFormHwnd,
        NULL,
        hInstance,
        NULL);
    SendMessage(lOutputLabel, WM_SETFONT, hNormalFont, 0);

    //==================================================================================================
    // Pole tekstowe pliku wyjœcia

    eOutputEdit = CreateWindowEx (
        WS_EX_CLIENTEDGE,
        "EDIT",
        NULL,
        WS_CHILD | WS_VISIBLE,
        5,
        75,
        245,
        20,
        MainFormHwnd,
        NULL,
        hInstance,
        NULL);
    SendMessage(eOutputEdit, WM_SETFONT, hNormalFont, 0);
    CreateTooltip (eOutputEdit,
    "Wpisz tutaj pe³n¹ œcie¿kê pliku, do którego ma zostaæ zapisany wynik, lub skorzystaj z przycisku po prawej stronie. Plik nie musi istnieæ.",
    "Informacja");
    SetWindowLong (eOutputEdit, GWL_WNDPROC, (LONG)MainHwndEditsWndProc);

    //==================================================================================================
    // Przycisk dla pola tekstowego pliku wyjœcia

    bOutputButton = CreateWindowEx (
        NULL,
        "BUTTON",
        "...",
        WS_CHILD | WS_VISIBLE,
        255,
        75,
        30,
        20,
        MainFormHwnd,
        NULL,
        hInstance,
        NULL);
    SendMessage(bOutputButton, WM_SETFONT, hNormalFont, 0);
    CreateTooltip (bOutputButton, "Wciœnij ten przycisk, aby wybraæ plik, do którego ma zostaæ zapisany wynik.", "Informacja");

    //==================================================================================================
    // Przycisk "Start"

    bStart = CreateWindowEx (
        NULL,
        "BUTTON",
        "Start",
        WS_CHILD | WS_VISIBLE,
        5,
        110,
        60,
        30,
        MainFormHwnd,
        NULL,
        hInstance,
        NULL);
    SendMessage(bStart, WM_SETFONT, hNormalFont, 0);
    CreateTooltip (bStart, "Wciœnij ten przycisk, aby program rozpocz¹³ wykonywanie zadania.", "Start");

    //==================================================================================================
    // Etykieta "Zaoszczêdzono:"

    lSavedSpace = CreateWindowEx (
        NULL,
        "STATIC",
        "Zaoszczêdzono: n/d",
        WS_CHILD | WS_VISIBLE,
        5,
        145,
        200,
        20,
        MainFormHwnd,
        NULL,
        hInstance,
        NULL);
    SendMessage(lSavedSpace, WM_SETFONT, hNormalFont, 0);

    //==================================================================================================
    // Etykieta "Liczba powtórzonych linii:"

    lRepeatedLines = CreateWindowEx (
        NULL,
        "STATIC",
        "Liczba duplikatów: n/d",
        WS_CHILD | WS_VISIBLE,
        5,
        162,
        290,
        20,
        MainFormHwnd,
        NULL,
        hInstance,
        NULL);
    SendMessage(lRepeatedLines, WM_SETFONT, hNormalFont, 0);

    //==================================================================================================
    // Etykieta "Czas operacji:"

    lTime = CreateWindowEx (
        NULL,
        "STATIC",
        "Czas operacji: n/d (n/d)",
        WS_CHILD | WS_VISIBLE,
        5,
        179,
        290,
        20,
        MainFormHwnd,
        NULL,
        hInstance,
        NULL);
    SendMessage(lTime, WM_SETFONT, hNormalFont, 0);

    //==================================================================================================
    // Etykieta "Separator:"

    HWND lSeparator = CreateWindowEx (
        NULL,
        "STATIC",
        "Separator:",
        WS_CHILD | WS_VISIBLE | SS_RIGHT,
        180,
        113,
        70,
        20,
        MainFormHwnd,
        NULL,
        hInstance,
        NULL);
    SendMessage(lSeparator, WM_SETFONT, hNormalFont, 0);

    //==================================================================================================
    // Pole edycyjne "Separator"

    eSeparator = CreateWindowEx (
        WS_EX_CLIENTEDGE,
        "EDIT",
        "",
        WS_CHILD | WS_VISIBLE,
        255,
        110,
        30,
        20,
        MainFormHwnd,
        NULL,
        hInstance,
        NULL);
    SendMessage(eSeparator, WM_SETFONT, hNormalFont, 0);
    CreateTooltip (eSeparator,
    "W tym polu wpisz separator, który oddziela kolejne ci¹gi znaków. Pozostaw pole puste, aby separatorem by³ znak nowej linii.\n\nUwaga! Separatorem mo¿e byæ tylko 1 znak!",
    "Separator");
    SendMessage (eSeparator, EM_SETLIMITTEXT, 1, 0);
    SetWindowLong (eSeparator, GWL_WNDPROC, (LONG)MainHwndEditsWndProc);

    //==================================================================================================

    // Pasek postêpu
    /*pbMainProgressBar = CreateWindowEx(
        0,
        "STATIC",
        "0%",
        WS_CHILD | WS_VISIBLE,
        70,
        110,
        210,
        30,
        MainFormHwnd,
        NULL,
        hInstance,
        NULL);
    //SendMessage(pbMainProgressBar, PBM_SETPOS, 0, 0);*/

    //==================================================================================================*/

    if (MainFormHwnd == NULL)
    {
        MessageBox (NULL, "Nie mo¿na utworzyæ okna: b³¹d przy tworzeniu!", "B³¹d", MB_OK | MB_ICONERROR);
        return 1;
    }

    ShowWindow (MainFormHwnd, nCmdShow);
    UpdateWindow (MainFormHwnd);

    //==================================================================================================
    // Pêtla komunikatów

    while (GetMessage (&Message, NULL, 0, 0))
    {
        TranslateMessage (&Message);
        DispatchMessage (&Message);
    }

    return Message.wParam;

    //==================================================================================================

    //return 0;
}

void OpenFileDialog ();
void SaveFileDialog ();
void ProcessFile();

PSTR GetControlText(HWND hWindow);
void CreateDialogBox(HWND);
void RegisterDialogClass(HWND hwnd);
LRESULT CALLBACK WndProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
      case WM_CREATE:
          RegisterDialogClass(hwnd);
          break;
        case WM_CLOSE:
            DestroyWindow (hwnd);
            break;

        case WM_DESTROY:
            PostQuitMessage (0);
            break;

        case WM_COMMAND:
            if ((HWND) lParam == bInputButton)
            {
                OpenFileDialog();
            }

            if ((HWND) lParam == bOutputButton)
            {
                SaveFileDialog();
            }

            if ((HWND) lParam == bStart)
            {
                ProcessFile();
            }

            switch(LOWORD(wParam)) {
                case ID_FILE_EXIT:
                    PostQuitMessage(0);
                break;

                case ID_HELP_ABOUT:
                    CreateDialogBox(hwnd);
                break;
            }
            break;

        case WM_NOTIFY:
            if (((LPNMHDR)lParam)->code == TTN_GETDISPINFO)
            {
                LPNMTTDISPINFO lpnmtdi = (LPNMTTDISPINFO) lParam;

                if (lpnmtdi->lParam != (LPARAM)NULL)
                {
                    DWORD dwValue;
                    memcpy(&dwValue, (const void*)lpnmtdi->lParam, 4);

                    if (dwValue == MYTOOLTIPTEST)
                    {
                        MYTOOLTIP* mtl = (MYTOOLTIP*)(lpnmtdi->lParam);

                        lpnmtdi->lpszText = mtl->szTip;
                        lpnmtdi->hinst = NULL;
                        lpnmtdi->uFlags = TTF_DI_SETITEM;

                        SendMessage (((LPNMHDR)lParam)->hwndFrom, TTM_SETMAXTIPWIDTH, 0, 300);

                        delete mtl;
                    }
                }
            }
            break;

        case WM_KEYDOWN:
            switch ((int) wParam)
            {
                case VK_ESCAPE:
                    PostQuitMessage(0);
                    break;
            }
            break;

        default:
            return DefWindowProc (hwnd, msg, wParam, lParam);
    }

    return 0;
}

LRESULT CALLBACK MainHwndEditsWndProc (HWND hwnd, UINT mesg, WPARAM wParam, LPARAM lParam)
{
    switch (mesg)
    {
        case WM_KEYDOWN:
            switch ((int) wParam)
            {
                case VK_ESCAPE:
                    PostQuitMessage(0);
                    break;
            }
            break;
    }

    return CallWindowProc (MainHwndEditsOldWndProc, hwnd, mesg, wParam, lParam);
}

void OpenFileDialog ()
{
     OPENFILENAME ofn;
     char FileName[MAX_PATH] = "";

     ZeroMemory (&ofn, sizeof(ofn));
     ofn.lStructSize = sizeof(ofn);
     ofn.lpstrFilter = "Pliki tekstowe (*.txt)\0*.txt\0Wszystkie pliki (*.*)\0*.*\0";
     ofn.nMaxFile = MAX_PATH;
     ofn.lpstrFile = FileName;
     ofn.lpstrDefExt = "txt";
     ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

    EnableWindow(MainFormHwnd, FALSE);
    if (GetOpenFileName(&ofn))
    {
        SetWindowText(eInputEdit, FileName);
        EnableWindow(MainFormHwnd, TRUE);
    }
    else
    {
        EnableWindow(MainFormHwnd, TRUE);
    }
}

void SaveFileDialog ()
{
     OPENFILENAME ofn;
     char FileName[MAX_PATH] = "";

     ZeroMemory (&ofn, sizeof(ofn));
     ofn.lStructSize = sizeof(ofn);
     ofn.lpstrFilter = "Pliki tekstowe (*.txt)\0*.txt\0Wszystkie pliki (*.*)\0*.*\0";
     ofn.nMaxFile = MAX_PATH;
     ofn.lpstrFile = FileName;
     ofn.lpstrDefExt = "txt";
     ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

    EnableWindow(MainFormHwnd, FALSE);
    if (GetSaveFileName(&ofn))
    {
        SetWindowText(eOutputEdit, FileName);
        EnableWindow(MainFormHwnd, TRUE);
    }
    else
    {
        EnableWindow(MainFormHwnd, TRUE);
    }
}

PSTR GetControlText(HWND hWindow)
{
    int cTxtLen = GetWindowTextLength(hWindow);
    PSTR pszMem = (PSTR) VirtualAlloc((LPVOID) NULL,
        (DWORD) (cTxtLen + 1), MEM_COMMIT,
        PAGE_READWRITE);

    GetWindowText(hWindow, pszMem, cTxtLen + 1);

    return (pszMem);
}

class stats
{
    public:
    //long int Lines;
    long int Size;
    long int Size2;
    long int RepeatedLines;
    long int Time;
};

std::string Int2Str (int src)
{
    char csrc[100];
    sprintf (csrc, "%d", src);
    std::string ssrc;
    ssrc.assign(csrc);

    return (ssrc);
}

std::string SetSize (int src)
{
    if (src >= 1024) // kilobajty
    {
        if (src >= 1048576) // megabajty
        {
            if (src >= 1073741824) // gigabajty
            {
                return (Int2Str (round ((double) src / 1073741824)) + "GB");
            }
            else
            {
                return (Int2Str (round ((double) src / 1048576)) + "MB");
            }
        }
        else
        {
            return (Int2Str (round ((double) src / 1024)) + "KB");
        }
    }
    else
    {
        return (Int2Str(src) + "B");
    }
}

void ProcessFile()
{
    long int Time1 = GetTickCount();

    stats Stats;
    Stats.RepeatedLines = 0;
    Stats.Size = 0;
    Stats.Size2 = 0;
    Stats.Time = 0;

    std::fstream fp;
    // Otwarcie pliku: pobranie nazwy z kontrolki eInputEdit, otwarcie pliku do odczytu i binarnie
    fp.open(std::string(GetControlText(eInputEdit)).c_str(), std::ios::in | std::ios::binary);
    if (fp.good() != true)
    {
        MessageBox(NULL, "Nie mo¿na otworzyæ pliku wejœciowego!", "Duplicate Lines Finder", MB_OK | MB_ICONERROR);
    }
    else
    {
        unsigned long int size;
        char *buf;

        fp.seekg( 0, std::ios_base::end ); // Obliczanie rozmiaru pliku
        size = fp.tellg();
        fp.seekg( 0, std::ios_base::beg );

        Stats.Size = size;

        buf = new char[size+1];
        for (unsigned long int i = 0; i < size; i++)
        {
            fp.read(&buf[i], 1); // Odczytywanie danych bajt po bajcie
        }
        fp.close();

        std::ofstream OutFp;
        // Otwarcie pliku: pobranie nazwy z kontrolki eOutputEdit, otwarcie pliku do zapisu i binarnie
        OutFp.open (std::string(GetControlText(eOutputEdit)).c_str(), std::ios::out | std::ios::binary);

        if (OutFp.good() != true)
        {
            MessageBox (NULL, "Nie mo¿na otworzyæ pliku wyjœciowego!", "Duplicate Lines Finder", MB_OK | MB_ICONERROR);

            SetWindowText (lSavedSpace, "Zaoszczêdzono: n/d");
            SetWindowText (lRepeatedLines, "Liczba duplikatów: n/d");
            SetWindowText (lTime, "Czas operacji: n/d");
        }
        else
        {
            std::string strbuf = buf;

            unsigned long int IntThisLine = 0;
            unsigned long int IntPrevLine = 0;

            bool FirstIteration = true; // Zmienna bool eliminuj¹ca problem z uciêt¹ pierwsz¹ liter¹/znakiem nowej linii na pocz¹tku

            std::vector <std::string> MainVector;
            std::string sThisLine;
            bool IsRepeat;

            char separator;
            if (GetControlText(eSeparator)[0] == NULL)
            {
                separator = '\n';
            }
            else
            {
                separator = GetControlText(eSeparator)[0];
            }

            char separator2 = separator;
            if (separator == '\n')
            {
                separator2 = '\r';
            }

            //SendMessage(pbMainProgressBar, PBM_SETPOS, 100, 0);
            for (unsigned long int i = 0; i <= size; i++)
            {
                //SendMessage(pbMainProgressBar, PBM_SETPOS, round((double)i / strbuf.length() * 100), 0);
                /*int omg=round((double)i / strbuf.length() * 100);
                char wtf[10];
                sprintf (wtf, "%d", omg);
                std::string lawl;
                lawl.assign(wtf);
                lawl = lawl + "%";
                SetWindowText(pbMainProgressBar, lawl.c_str());*/

                if (buf[i] == separator || buf[i] == separator2 || i == strbuf.length())
                {
                    IntThisLine = i; // Ustaw zmiennej IntThisLine jego pozycjê

                    if (FirstIteration == true)
                    {
                        sThisLine = strbuf.substr (IntPrevLine, IntThisLine-IntPrevLine); // Przypisz zmiennej sThisLine aktualn¹ liniê
                        if (sThisLine != "\n" && sThisLine != "\r" && sThisLine != "" && sThisLine.length() != 0)
                        {
                            IsRepeat = false;
                            for (unsigned long int i2 = 0; i2 < MainVector.size(); i2++)
                            {
                                if (sThisLine == MainVector[i2])
                                {
                                    IsRepeat = true;
                                    Stats.RepeatedLines = Stats.RepeatedLines + 1;
                                    break;
                                }
                            }

                            if (IsRepeat == false)
                            {
                                MainVector.push_back(sThisLine);
                                OutFp << sThisLine;
                            }

                            FirstIteration = false;
                        }
                    }
                    else
                    {
                        sThisLine = strbuf.substr (IntPrevLine+1, IntThisLine-IntPrevLine-1); // Przypisz zmiennej ThisLine aktualn¹ liniê

                        if (sThisLine != "\n" && sThisLine != "\r" && sThisLine != "" && sThisLine.length() != 0)
                        {
                            IsRepeat = false;
                            for (unsigned long int i2 = 0; i2 < MainVector.size(); i2++)
                            {
                                if (sThisLine == MainVector[i2])
                                {
                                    IsRepeat = true;
                                    Stats.RepeatedLines = Stats.RepeatedLines + 1;
                                    break;
                                }
                            }

                            if (IsRepeat == false)
                            {
                                MainVector.push_back(sThisLine);
                                OutFp << separator + sThisLine;
                            }
                        }
                    }


                    IntPrevLine = i; // Ustaw pozycjê poprzedniej linii na pozycjê znaku nowej linii
                }
            }
            //Stats.Lines = MainVector.size();


            delete[] buf;
            buf = NULL;

            Stats.Time = GetTickCount() - Time1;
            //OutFp.seekg( 0, std::ios_base::end ); // Obliczanie rozmiaru pliku
            Stats.Size = Stats.Size - fp.tellg();

            OutFp.close();

            fp.open(std::string(GetControlText(eOutputEdit)).c_str(), std::ios::in | std::ios::binary);
            fp.seekg( 0, std::ios_base::end ); // Obliczanie rozmiaru pliku
            Stats.Size2 = fp.tellg();
            fp.close();
        }
    }

    SetWindowText (lSavedSpace, std::string("Zaoszczêdzono: " + SetSize(Stats.Size - Stats.Size2)).c_str());
    SetWindowText (lRepeatedLines, std::string("Liczba duplikatów: " + Int2Str(Stats.RepeatedLines)).c_str());
    SetWindowText (lTime,
        std::string("Czas operacji: " + Int2Str(Stats.Time) + "ms (" + Int2Str (round ((double)Stats.Time / 1000) ) + "s)").c_str());
}

void CreateTooltip (HWND hParent, LPCSTR Text, LPCSTR Title)
{
    // Utowrzenie Tooltip'a
    HWND hTooltip = CreateWindowEx (
        WS_EX_TOPMOST,
        TOOLTIPS_CLASS,
        NULL,
        WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP | TTS_BALOON,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        hParent,
        NULL,
        MainInstance,
        NULL);

    SetWindowPos (hTooltip, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE); // Zawsze na wierzchu

    TOOLINFO ti;
    ti.cbSize = sizeof (TOOLINFO);
    ti.uFlags = TTF_SUBCLASS | TTF_IDISHWND;
    ti.hwnd = MainFormHwnd;
    ti.hinst = NULL;
    ti.uId = (UINT_PTR) hParent;
    ti.lpszText = LPSTR_TEXTCALLBACK;

    MYTOOLTIP* mtl = new MYTOOLTIP;
    mtl->dwTest = MYTOOLTIPTEST;
    mtl->szTip = (LPSTR)Text;
    ti.lParam = (LPARAM) mtl;

    // Obszar aktywnoœci Tooltip'a
    RECT rect;
    GetClientRect (hParent, &rect);

    ti.rect.left = rect.left;
    ti.rect.top = rect.top;
    ti.rect.right = rect.right;
    ti.rect.bottom = rect.bottom;

    SendMessage (hTooltip, TTM_ADDTOOL, 0, (LPARAM)&ti);

    SendMessage (hTooltip, TTM_SETTITLE, 1, (LPARAM)Title);
}

LRESULT CALLBACK DialogProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  switch(msg)
  {
    case WM_KEYDOWN:
        switch ((int) wParam)
        {
            case VK_ESCAPE:
                EnableWindow (MainFormHwnd, TRUE);
                DestroyWindow (AboutHwnd);
                break;
        }
        break;

    case WM_COMMAND:
        if ( (HWND) lParam == lAuthorUrl )
        {
            ShellExecute(NULL, "open", "http://www.m4tx.2ap.pl/",
                NULL, NULL, SW_SHOWNORMAL);
        }
        break;

    case WM_CLOSE:
        EnableWindow(MainFormHwnd, TRUE);
        DestroyWindow(hwnd);
        break;
  }
  return (DefWindowProc(hwnd, msg, wParam, lParam));

}

LRESULT CALLBACK lAuthorUrlProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_MOUSEMOVE:
            HCURSOR curs = AboutWc.hCursor = LoadCursor (0, IDC_HAND);
            SetCursor (curs);
            break;
    }

    return CallWindowProc (lAuthorUrlOldWndProc, hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK AboutHwndProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_MOUSEMOVE:
            HCURSOR curs = AboutWc.hCursor = LoadCursor (NULL, IDC_ARROW);
            SetCursor (curs);
            break;
    }

    return CallWindowProc (AboutHwndOldWndProc, hwnd, msg, wParam, lParam);
}

void RegisterDialogClass(HWND hwnd)
{
    AboutWc.cbSize           = sizeof(WNDCLASSEX);
    AboutWc.lpfnWndProc      = (WNDPROC) DialogProc;
    AboutWc.hInstance        = MainInstance;
    AboutWc.hbrBackground    = GetSysColorBrush(COLOR_3DFACE);
    AboutWc.lpszClassName    = TEXT("AboutClass");
    RegisterClassEx(&AboutWc);
}

void CreateDialogBox(HWND hwnd)
{
    AboutHwnd = CreateWindowEx(
        WS_EX_TOOLWINDOW,
        TEXT("AboutClass"),
        TEXT("O programie..."),
        WS_VISIBLE | WS_SYSMENU | WS_CAPTION,
        100,
        100,
        180,
        118,
        MainFormHwnd,
        NULL,
        MainInstance,
        NULL);

    AboutHwndOldWndProc = (WNDPROC) SetWindowLong (AboutHwnd, GWL_WNDPROC, (LONG) AboutHwndProc);

    EnableWindow(MainFormHwnd, FALSE);

    HWND sIcon = CreateWindowEx (NULL, "STATIC", NULL, WS_CHILD | WS_VISIBLE | SS_ICON, 5, 5, 48, 48, AboutHwnd, 0, MainInstance, NULL);
    HICON hIcon = (HICON) LoadImage(MainInstance, "APPICON", IMAGE_ICON, 48, 48, NULL);
    SendMessage(sIcon, STM_SETIMAGE, IMAGE_ICON, (LPARAM)hIcon);

    //==================================================================================================
    // Nazwa programu

        HWND lName = CreateWindowEx(
            0,
            "STATIC",
            "Duplicate Lines",
            WS_CHILD | WS_VISIBLE | SS_LEFT,
            60,
            5,
            120,
            15,
            AboutHwnd,
            NULL,
            MainInstance,
            NULL);
        SendMessage(lName, WM_SETFONT, (WPARAM)hNormalFont, 0);

    //==================================================================================================
    // Nazwa programu - linia 2

        HWND lName2 = CreateWindowEx(
            0,
            "STATIC",
            "Finder",
            WS_CHILD | WS_VISIBLE | SS_LEFT,
            60,
            20,
            120,
            15,
            AboutHwnd,
            NULL,
            MainInstance,
            NULL);
        SendMessage(lName2, WM_SETFONT, (WPARAM)hNormalFont, 0);

    //==================================================================================================
    // Wersja

        HWND lVersion = CreateWindowEx(
            0,
            "STATIC",
            "wersja 1.1",
            WS_CHILD | WS_VISIBLE | SS_LEFT,
            60,
            40,
            60,
            15,
            AboutHwnd,
            NULL,
            MainInstance,
            NULL);
        SendMessage(lVersion, WM_SETFONT, (WPARAM)hNormalFont, 0);

    //==================================================================================================
    // Autor

        HWND lAuthor = CreateWindowEx(
            0,
            "STATIC",
            "Autor: m4tx",
            WS_CHILD | WS_VISIBLE | SS_LEFT,
            5,
            60,
            60,
            15,
            AboutHwnd,
            NULL,
            MainInstance,
            NULL);
        SendMessage(lAuthor, WM_SETFONT, (WPARAM)hNormalFont, 0);

    //==================================================================================================
    // Link do strony autora

    // Czcionka
        HFONT lAuthorUrlFont = CreateFont(
            14,
            0,
            0,
            0,
            FW_DONTCARE,
            FALSE,
            TRUE,
            FALSE,
            DEFAULT_CHARSET,
            OUT_CHARACTER_PRECIS,
            CLIP_CHARACTER_PRECIS,
            DEFAULT_QUALITY,
            FF_DONTCARE,
            NULL);

    // Kontrolka
        lAuthorUrl = CreateWindowEx(
            0,
            "STATIC",
            "www.m4tx.2ap.pl",
            WS_CHILD | WS_VISIBLE | SS_LEFT | SS_NOTIFY,
            5,
            75,
            90,
            15,
            AboutHwnd,
            NULL,
            MainInstance,
            NULL);
        SendMessage(lAuthorUrl, WM_SETFONT, (WPARAM)lAuthorUrlFont, 0);
        lAuthorUrlOldWndProc = (WNDPROC) SetWindowLong (lAuthorUrl, GWL_WNDPROC, (LONG)lAuthorUrlProc);

    //==================================================================================================

}

