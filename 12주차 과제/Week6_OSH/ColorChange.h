#pragma once
#include <windows.h>  /// CreateWindow에 필요한 헤더
#include "framework.h"
#include "Resource.h"

class ColorChange {

public:
    void changeColorFunc(COLORREF* paintColor, int wmId);
};