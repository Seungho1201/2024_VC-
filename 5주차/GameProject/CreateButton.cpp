/**
@file button.cpp
@brief ��ư ���� ���� Ŭ���� �ܺ� ���� ����
*/
#include "CreateButton.h"


/*
@fn  MakeButton::MakeButton(int x, int y, int width, int height, int func, LPCWSTR text)
@brief ��ư Ŭ���� ������
@param x ��ư�� x ��ǥ
@param y ��ư�� y��ǥ
@param width ��ư�� ��
@param height ��ư�� �ʺ�
@param func COMMAND���� �۵��� ��� ���
@param text ��ư�� �׷����� �ؽ�Ʈ
*/
MakeButton::MakeButton(int x, int y, int width, int height, int func, LPCWSTR text)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->func = func;
    this->text = text;
    this->hButton = nullptr;
}


/// <summary>
/// ��ư ������ �ΰ����� ������
/// 1. �ؽ�Ʈ�� ��ư ���� - ���� �� X
/// 2. ���� ��� ����� ���� - ������ ���� �� 1��
/// 
/// ���� �������� �߰��ϰų� �����ϰ��� ������ ���� �� ������ 
/// �������� ��Ʈ�� Ȯ����(bmp)�� �����̿��� �ϰ� "���ҽ� ����" �ȿ� �����ؾ� ��
/// �������� Ref_GuestBook.rc ���Ͽ��� ���� �Ѵ� (ex: IDI_ERASE_ICON    ICON    "..\..\icon\Pen.ico")
/// rc ���Ͽ��� ���� �� Resource.h ���Ͽ��� ����� ������ �Ѵ� (ex: #define IDI_ERASE_ICON     130)
/// ���� ������ �̹����� ���� �ν��Ͻ��� mkButton(int path) �޼����� ���ڷ� Resource.h ���Ͽ��� ������ ����� �ִ´�.
/// </summary>

/**
@fn  MakeButton::mkButton()
@brief ��ư Ŭ������ ��ư ���� �޼���
@param g_Hwnd ������ �ڵ�
*/
HWND MakeButton::mkButton(HWND g_Hwnd)
{
    this->hButton = CreateWindow(
        L"BUTTON",                                                          // ��ư Ŭ���� �̸�
        text,                                                               // ��ư �ؽ�Ʈ
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_FLAT,       // ��ư ��Ÿ��
        x,                                                                  // ��ư�� x ��ġ
        y,                                                                  // ��ư�� y ��ġ
        width,                                                              // ��ư�� ��
        height,                                                             // ��ư�� ����
        g_Hwnd,                                                             // �θ� ������ �ڵ�
        (HMENU)func,                                                        // ��ư�� ID
        (HINSTANCE)GetWindowLongPtr(g_Hwnd, GWLP_HINSTANCE),                // �ν��Ͻ� �ڵ�
        NULL                                                                // �߰� �Ű�����
    );

    // ������ ��ư�� �ڵ��� ��ȿ���� Ȯ��
    if (this->hButton != NULL)
    {
        // ������ ��ư�� ���� �׵θ� ���� �޽��� ����
        SendMessage(this->hButton, WM_UPDATEUISTATE, MAKELONG(UIS_SET, UISF_HIDEFOCUS), 0);
    }

    return this->hButton;
}



