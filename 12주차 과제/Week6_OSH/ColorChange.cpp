#include "ColorChange.h"

void ColorChange::changeColorFunc(COLORREF* paintColor, int wmId)
{
    switch (wmId)
    {
        // red
    case ID_32775:
    case ID_32780:
        *paintColor = RGB(255, 0, 0);
        break;

        // green
    case ID_32776:
    case ID_32781:
        *paintColor = RGB(0, 255, 0);
        break;

        // blue
    case ID_32779:
    case ID_32782:
        *paintColor = RGB(0, 0, 255);
        break;

    case ID_32784:
    case ID_32783:
        *paintColor = RGB(0, 0, 0);
        break;
    
    case ID_32785:
        *paintColor = RGB(255, 255, 255);
        break;
    }
}