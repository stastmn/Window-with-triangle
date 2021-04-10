#pragma once
#include <windows.h>
#include <tchar.h>
#include <d2d1.h>
#include <iostream>

#include "Graphics.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
ATOM TryRegisterWindowFrom(HINSTANCE);
BOOL TryCreateWindowFrom(HINSTANCE, int);
void ShowMessageBox(const char*, const char*);
void RunWindowLoop(MSG*);
HWND CreateWindowFrom(HINSTANCE);
void HandleWindowResize(int, int, HWND, RECT);
void HandleMouseLeftClick(int*, int*, LPARAM);
void HandleMouseIsClickedAndMoving(int*, int*, LPARAM, WPARAM);
int RunApplication(_In_ HINSTANCE,_In_opt_ HINSTANCE,_In_ LPSTR,_In_ int);
