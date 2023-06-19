#include "all.h"

#include "../libs/raylib/zig-out/include/raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "../libs/raygui/src/raygui.h"

void DrawEmployeeData(EMPLOYEE *employees, int numEmployees, int startRow, int numRows) {
    int rowHeight = 20, y = 50, i;
    Font customFont = LoadFontEx("Hack-Regular.ttf", 20, NULL, 0);
    Vector2 textPosition = { 50, y };
    DrawTextEx(customFont, "Register", textPosition, 20, 2, BLACK);
    textPosition.x=150;
    DrawTextEx(customFont, "Salary", textPosition, 20, 2, BLACK);
    textPosition.x=300;
    DrawTextEx(customFont, "FName", textPosition, 20, 2, BLACK);
    textPosition.x=500;
    DrawTextEx(customFont, "LName", textPosition, 20, 2, BLACK);
    textPosition.x=650;
    DrawTextEx(customFont, "Code", textPosition, 20, 2, BLACK);
    textPosition.x=800;
    DrawTextEx(customFont, "Rate", textPosition, 20, 2, BLACK);
    DrawLine(48, y + 1.5*rowHeight, 850, y + 1.5*rowHeight, BLACK);
    textPosition.y+=2*rowHeight;
    for (i = startRow; i < startRow + numRows && i < numEmployees; i++) {
        textPosition.x=50;
        DrawTextEx(customFont, TextFormat("%hu", employees[i].mat), textPosition, 20, 2, BLACK);
        textPosition.x=150;
        DrawTextEx(customFont, TextFormat("%lu", employees[i].salary), textPosition, 20, 2, BLACK);
        textPosition.x=300;
        DrawTextEx(customFont, employees[i].namef->ptr, textPosition, 20, 2, BLACK);
        textPosition.x=500;
        DrawTextEx(customFont, employees[i].namel->ptr, textPosition, 20, 2, BLACK);
        textPosition.x=650;
        DrawTextEx(customFont, TextFormat("%hhu", employees[i].region.code), textPosition, 20, 2, BLACK);
        textPosition.x=800;
        DrawTextEx(customFont, TextFormat("%hhu", employees[i].region.rate), textPosition, 20, 2, BLACK);
        textPosition.y += rowHeight;
    }
    UnloadFont(customFont);
}
