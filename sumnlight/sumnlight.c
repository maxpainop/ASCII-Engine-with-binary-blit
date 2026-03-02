// sumnlight.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <time.h>

typedef struct systeminfo {
    int frame;
    int currentframecount;  
    int pixelcount;
    COORD winsize;
    LARGE_INTEGER frequency, start_time, end_time;
    char* canvas;
    COORD pos;
    int written;
};


HANDLE hconsole;

CONSOLE_SCREEN_BUFFER_INFO csbi;
CONSOLE_CURSOR_INFO cursinfo;
boolean g_EXITSTATUS = FALSE;
char* anim_frame= NULL;
struct systeminfo default_sysinfo;


void importframes() {

    FILE* f = fopen("asciitest.txt", "rb"); // must use binary mode
    if (f == NULL) {
        printf("Error: Could not find file!\n");
        return;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    anim_frame = (char*)malloc(size);
    fread(anim_frame, sizeof(char), size, f);

    fclose(f);
}

void initialization() {
default_sysinfo.frame, default_sysinfo.currentframecount = 0;
//initialize handle to console
hconsole = GetStdHandle(STD_OUTPUT_HANDLE);
GetConsoleScreenBufferInfo(hconsole, &csbi);

//initialize and set console window size
default_sysinfo.winsize.X = csbi.srWindow.Right - csbi.srWindow.Left + 1;
default_sysinfo.winsize.Y = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
SetConsoleScreenBufferSize(hconsole, default_sysinfo.winsize);



// strip down console window
HWND hwnd = GetConsoleWindow();
HMENU hMenu = GetSystemMenu(hwnd, FALSE);
long style = GetWindowLong(hwnd, GWL_STYLE);
style &= ~WS_MAXIMIZEBOX;
style &= ~WS_THICKFRAME;
SetWindowLong(hwnd, GWL_STYLE, style);
SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
DeleteMenu(hMenu, SC_SIZE, MF_BYCOMMAND);
DeleteMenu(hMenu, SC_MAXIMIZE, MF_BYCOMMAND);


//disable cursor visibility
cursinfo.dwSize = 1;
cursinfo.bVisible = FALSE;
SetConsoleCursorInfo(hconsole, &cursinfo);

//initialize canvas
default_sysinfo.pixelcount = default_sysinfo.winsize.X * default_sysinfo.winsize.Y;
default_sysinfo.canvas = (char*)malloc(default_sysinfo.pixelcount);

//get cpu frequency
QueryPerformanceFrequency(&default_sysinfo.frequency);


//import frame
importframes();

}

void drawcanvas() {



    memset(default_sysinfo.canvas, ' ', default_sysinfo.pixelcount);
    memcpy(default_sysinfo.canvas, anim_frame, default_sysinfo.pixelcount);
    default_sysinfo.canvas[default_sysinfo.winsize.X / 2] = (char)((default_sysinfo.currentframecount / 100) % 10 + '0');
    default_sysinfo.canvas[(default_sysinfo.winsize.X / 2)+1] = (char)((default_sysinfo.currentframecount / 10) % 10 + '0');
    default_sysinfo.canvas[(default_sysinfo.winsize.X / 2)+2] = (char)((default_sysinfo.currentframecount) % 10 + '0');
    WriteConsoleOutputCharacterA(
        hconsole,                  
        default_sysinfo.canvas,    
        default_sysinfo.pixelcount,
        default_sysinfo.pos,       
        &default_sysinfo.written   
    );

}

int main()
{

    initialization();

    while (!g_EXITSTATUS) {
        

        QueryPerformanceCounter(&default_sysinfo.start_time);


        drawcanvas();
        default_sysinfo.currentframecount++;

        QueryPerformanceCounter(&default_sysinfo.end_time);

        double time_taken_sec = (double)(default_sysinfo.end_time.QuadPart - default_sysinfo.start_time.QuadPart) / default_sysinfo.frequency.QuadPart;
        if (time_taken_sec < 0.0166666666666667) {
            double time_to_sleep_sec = 0.0166666666666667 - time_taken_sec;
            DWORD sleep_ms = (DWORD)(time_to_sleep_sec * 1000.0);
            if (sleep_ms > 0) Sleep(sleep_ms);
        }
        if (default_sysinfo.currentframecount >= 999) default_sysinfo.currentframecount = 0;
    }

}

