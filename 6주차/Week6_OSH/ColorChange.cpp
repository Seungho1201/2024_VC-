#include "ColorChange.h"

void ColorChange::changeColorFunc(COLORREF* paintColor, int wmId)
{
    switch (wmId)
    {
        // red
    case ID_32775:
        *paintColor = RGB(255, 0, 0);
        break;

        // green
    case ID_32776:
        *paintColor = RGB(0, 255, 0);
        break;

        // blue
    case ID_32779:
        *paintColor = RGB(0, 0, 255);
        break;
    }
}