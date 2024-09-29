/**
@file button.h
@brief ��ư ���� Ŭ���� ��� ���� ����
*/
#pragma once
#include <windows.h>  /// CreateWindow�� �ʿ��� ���
#include "framework.h"
#include "Resource.h"

class MakeButton {
private:
    int x;                /// ��ư�� x ��ġ
    int y;                /// ��ư�� y ��ġ
    int width;            /// ��ư�� ��
    int height;           /// ��ư�� ����
    int func;             /// ��ư�� ID
    LPCWSTR text;         /// ��ư �ؽ�Ʈ
    COLORREF buttonColor; /// ��ư ������
    HWND hButton;


public:

    MakeButton(int x, int y, int width, int height, int func, LPCWSTR text);

    HWND mkButton(HWND g_Hwnd);
    

private:
    
};
