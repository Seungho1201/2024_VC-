#pragma once
#include <windows.h>  /// CreateWindow�� �ʿ��� ���
#include "framework.h"
#include "Resource.h"

class ColorChange {

public:
    void changeColorFunc(COLORREF* paintColor, int wmId);
};