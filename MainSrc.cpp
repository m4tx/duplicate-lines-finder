// Duplicate Lines Finder
// Author: m4tx
// www.m4tx.2ap.pl
// 07.15.2010 09:29:16am

#include <windows.h>
#include <commctrl.h>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>

//#define ID_pbMainProgressBarTimer 100

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

LRESULT CALLBACK WndProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
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

    //==================================================================================================
    // G³ówne okno

    MainFormHwnd = CreateWindowEx (
        WS_EX_CLIENTEDGE,
        MainForm,
        "Duplicate Lines Finder",
        WS_SYSMENU | WS_BORDER | WS_CAPTION | WS_VISIBLE,
        200,
        200,
        300,
        180,
        NULL,
        NULL,
        hInstance,
        NULL);

    //==================================================================================================
    // Etykieta dla pola tekstowego pliku wejœcia

    HWND lInputLabel = CreateWindowEx (
        NULL,
        "STATIC",
        "Wybierz plik, który przetworzyæ:",
        WS_CHILD | WS_VISIBLE,
        5,
        5,
        280,
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
        240,
        20,
        MainFormHwnd,
        NULL,
        hInstance,
        NULL);
    SendMessage(eInputEdit, WM_SETFONT, hNormalFont, 0);

    //==================================================================================================
    // Przycisk dla pola tekstowego pliku wejœcia

    bInputButton = CreateWindowEx (
        NULL,
        "BUTTON",
        "...",
        WS_CHILD | WS_VISIBLE,
        250,
        25,
        30,
        20,
        MainFormHwnd,
        NULL,
        hInstance,
        NULL);
    SendMessage(bInputButton, WM_SETFONT, hNormalFont, 0);

    //==================================================================================================
    // Etykieta dla pola tekstowego pliku wejœcia

    HWND lOutputLabel = CreateWindowEx (
        NULL,
        "STATIC",
        "Wybierz plik, do którego zapisaæ wynik:",
        WS_CHILD | WS_VISIBLE,
        5,
        55,
        280,
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
        240,
        20,
        MainFormHwnd,
        NULL,
        hInstance,
        NULL);
    SendMessage(eOutputEdit, WM_SETFONT, hNormalFont, 0);

    //==================================================================================================
    // Przycisk dla pola tekstowego pliku wejœcia

    bOutputButton = CreateWindowEx (
        NULL,
        "BUTTON",
        "...",
        WS_CHILD | WS_VISIBLE,
        250,
        75,
        30,
        20,
        MainFormHwnd,
        NULL,
        hInstance,
        NULL);
    SendMessage(bOutputButton, WM_SETFONT, hNormalFont, 0);

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

    return 0;
}

void OpenFileDialog ();
void SaveFileDialog ();
void ProcessFile();

LRESULT CALLBACK WndProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
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
            break;

        default:
            return DefWindowProc (hwnd, msg, wParam, lParam);
    }

    return 0;
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
    std::string Size;
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
                return (Int2Str(src / 1073741824) + "GB");
            }
            else
            {
                return (Int2Str(src / 1048576) + "MB");
            }
        }
        else
        {
            return (Int2Str(src / 1024) + "KB");
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

    std::fstream fp;
    // Otwarcie pliku: pobranie nazwy z kontrolki eInputEdit, otwarcie pliku do odczytu i binarnie
    fp.open(std::string(GetControlText(eInputEdit)).c_str(), std::ios::in | std::ios::binary);
    if (fp.good() != true)
    {
        MessageBox(NULL, "B³¹d: nie mo¿na otworzyæ pliku!", "Duplicate Lines Finder", MB_OK | MB_ICONERROR);
    }
    else
    {
        long int size;
        char *buf;

        fp.seekg( 0, std::ios_base::end ); // Obliczanie rozmiaru pliku
        size = fp.tellg();
        fp.seekg( 0, std::ios_base::beg );

        Stats.Size = SetSize(size);

        buf = new char[size+1];
        for (int i = 0; i < size; i++)
        {
            fp.read(&buf[i], 1); // Odczytywanie danych bajt po bajcie
        }
        fp.close();

        std::ofstream OutFp;
        // Otwarcie pliku: pobranie nazwy z kontrolki eOutputEdit, otwarcie pliku do zapisu i binarnie
        OutFp.open (std::string(GetControlText(eOutputEdit)).c_str(), std::ios::out | std::ios::binary);

        if (OutFp.good() != true)
        {
            MessageBox (NULL, "B³¹d: nie mo¿na otworzyæ pliku!", "Duplicate Lines Finder", MB_OK | MB_ICONERROR);
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
            //SendMessage(pbMainProgressBar, PBM_SETPOS, 100, 0);
            for (unsigned long int i = 0; i <= strbuf.length(); i++)
            {
                //SendMessage(pbMainProgressBar, PBM_SETPOS, round((double)i / strbuf.length() * 100), 0);
                /*int omg=round((double)i / strbuf.length() * 100);
                char wtf[10];
                sprintf (wtf, "%d", omg);
                std::string lawl;
                lawl.assign(wtf);
                lawl = lawl + "%";

                SetWindowText(pbMainProgressBar, lawl.c_str());*/
                if (buf[i] == '\n' || i == strbuf.length())
                {
                    IntThisLine = i; // Ustaw zmiennej IntThisLine jego pozycjê

                    if (FirstIteration == true)
                    {
                        sThisLine = strbuf.substr (IntPrevLine, IntThisLine-IntPrevLine); // Przypisz zmiennej ThisLine aktualn¹ liniê
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
                    else
                    {
                        sThisLine = strbuf.substr (IntPrevLine+1, IntThisLine-IntPrevLine-1); // Przypisz zmiennej ThisLine aktualn¹ liniê
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
                            OutFp << '\n' + sThisLine;
                        }
                    }


                    IntPrevLine = i; // Ustaw pozycjê poprzedniej linii na pozycjê znaku nowej linii
                }
            }
            //Stats.Lines = MainVector.size();


            delete[] buf;
            buf = NULL;
            OutFp.close();
        }
    }
    Stats.Time = GetTickCount() - Time1;
}
