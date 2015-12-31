#pragma once

#include "resource.h"
#include <stdlib.h>
#include <time.h>
#include <windowsx.h>

#define MAX_LOADSTRING 100

HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

#define NUMTHREADS 2

static long iNumOfRestBall = 0;					// So bong con lai
static unsigned long iNumOfBall = 0;			// A number of Balls
static unsigned int iCurrentTeam = 0;			// Current Team
static unsigned long iTimers = 0;				// Total of milisecond
static long long ThoiGianConLai = 0;			// Thoi gian con lai
static unsigned int SoLanGiuKho[NUMTHREADS];	// So lan giu kho
static unsigned int iNumOfGetBall[NUMTHREADS];	// Tong so bong lay duoc
static HANDLE mutex;							// Mutex
static TCHAR* teamName[NUMTHREADS];				// The name of team
static HANDLE threadList[NUMTHREADS];			// List of thread

// The data which are given to threads
typedef struct ThreadData
{
	DWORD32 index;
} TData, *PTData;

PTData data = new TData[NUMTHREADS];

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	DlgProc(HWND, UINT, WPARAM, LPARAM);

DWORD WINAPI ThreadFunc1(LPVOID data)
{
	PTData para = (PTData)data;
	teamName[para->index] = L"Team 1";
	while (true)
	{
		if (WaitForSingleObject(mutex, INFINITE) == 0)
		{
			iCurrentTeam = para->index;
			Sleep(1000);
			iNumOfRestBall -= 2;
			iNumOfGetBall[para->index] += 2;
			SoLanGiuKho[para->index]++;
			ReleaseMutex(mutex);
		}
	}
}

DWORD WINAPI ThreadFunc2(LPVOID data)
{
	PTData para = (PTData)data;

	teamName[para->index] = L"Team 2";

	while (true)
	{
		if (WaitForSingleObject(mutex, 3000) == 0)
		{
			iCurrentTeam = para->index;
			if (iNumOfRestBall <= 3)
			{
				iNumOfGetBall[para->index] += iNumOfRestBall;
				iNumOfRestBall = 0;
				Sleep(3000);
				return true;
			}

			if (ThoiGianConLai <= 0 || iNumOfRestBall <= 0)
				return true;

			if ((ThoiGianConLai / 1000 / 4) < (iNumOfRestBall / 6))
			{
				iNumOfGetBall[para->index] += 3;
				iNumOfRestBall -= 3;
				Sleep(3000);
			}
			else if (iNumOfBall % 4 == 0)
			{
				iNumOfGetBall[para->index] += 3;
				iNumOfRestBall -= 3;
				Sleep(3000);
			}
			else
			{
				int du = iNumOfBall % 4;
				iNumOfGetBall[para->index] += du;
				iNumOfRestBall -= du;
				Sleep(1000);
			}
			SoLanGiuKho[para->index] ++;

			ReleaseMutex(mutex);
		}
	}
	return true;
}

void OnInitDialog(HWND hDlg)
{
	srand(time(NULL));
	iNumOfRestBall = iNumOfBall = rand() % 31 + 30;
	iTimers = ThoiGianConLai = (rand() % 31 + 30) * 1000;
	mutex = CreateMutex(0, 0, 0);

	ZeroMemory(&SoLanGiuKho, sizeof(int));
	ZeroMemory(&iNumOfGetBall, sizeof (int));

	for (int i = 0; i < NUMTHREADS; ++i)
	{
		data[i].index = i;
	}
}

void OnStart(HWND hDlg)
{
	SetTimer(hDlg, 1, 200, NULL);
	
	// Set the first run
	int id = rand() % 2;
	threadList[id] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadFunc1, data, 0, 0);
	threadList[1 - id] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadFunc2, data + 1, 0, 0);
}
