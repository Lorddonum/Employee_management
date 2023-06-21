void DrawEmployeeData(EMPLOYEE *employees, int numEmployees, int startRow, int numRows) {
    int rowHeight = 20, y = 50, i;
    DrawText("Register", 50, y, 20, BLACK);
    DrawText("Salary", 150, y, 20, BLACK);
    DrawText("FName", 300, y, 20, BLACK);
    DrawText("LName", 500, y, 20, BLACK);
    DrawText("Code", 650, y, 20, BLACK);
    DrawText("Rate", 800, y, 20, BLACK);
    DrawLine(48, y + 1.5*rowHeight, 850, y + 1.5*rowHeight, BLACK);
    y+=2*rowHeight;
    for (i = startRow; i < startRow + numRows && i < numEmployees; i++) {
        DrawText(TextFormat("%d", employees[i].mat), 50, y, 20, BLACK);
        DrawText(TextFormat("%d", employees[i].salary), 150, y, 20, BLACK);
        DrawText(employees[i].namef, 300, y, 20, BLACK);
        DrawText(employees[i].namel, 500, y, 20, BLACK);
        DrawText(TextFormat("%d", employees[i].region.code), 650, y, 20, BLACK);
        DrawText(TextFormat("%d", employees[i].region.rate), 800, y, 20, BLACK);
        y += rowHeight;
    }
}

void DrawInputBox(Rectangle box, char* buffer, int bufferSize, bool active) {
    static float backspaceHoldingTime = 0.0f;
    const float initialBackspaceDelay = 0.5f;
    const float backspaceRepeatDelay = 0.1f;

    int visibleTextStart = 0;
    int textWidth; 

    int maxVisibleCharacters;

    DrawRectangleRec(box, LIGHTGRAY);
    DrawRectangleLinesEx(box, 2, BLACK);

    if (active) {
        int key = GetCharPressed();

        /// Check if more characters have been pressed on the same frame
        while (key > 0) {
            /// Only allow keys in range [32..125]
            if ((key >= 32) && (key <= 125) && (strlen(buffer) < bufferSize - 1)) {
                int len = strlen(buffer);
                buffer[len] = (char)key;
                buffer[len + 1] = '\0';
            }
            key = GetCharPressed();  /// Check next character in the queue
        }

        if (IsKeyDown(KEY_BACKSPACE)) {
            backspaceHoldingTime += GetFrameTime();
        } else {
            backspaceHoldingTime = 0.0f;
        }

        if (backspaceHoldingTime > initialBackspaceDelay) {
            if (((int)(backspaceHoldingTime * 10) % (int)(backspaceRepeatDelay * 10)) == 0) {
                int len = strlen(buffer);
                if (len > 0) {
                    buffer[len - 1] = '\0';
                }
            }
        }
    }

    textWidth = MeasureText(buffer, 20);
    while (textWidth > box.width - 10) {
        visibleTextStart++;
        textWidth = MeasureText(buffer + visibleTextStart, 20);
    }

    maxVisibleCharacters = (box.width - 10) / MeasureText("A", 20);
    char visibleBuffer[maxVisibleCharacters + 1];
    strncpy(visibleBuffer, buffer + visibleTextStart, maxVisibleCharacters);
    visibleBuffer[maxVisibleCharacters] = '\0';

    DrawText(visibleBuffer, box.x + 5, box.y + (box.height / 4), 20, BLACK);

    if (visibleTextStart > 0) {
        DrawRectangleGradientH(box.x+2, box.y+2, 14, box.height-4, Fade(LIGHTGRAY, 0.8f), LIGHTGRAY);
    }
}

void SearchEmployee(EMPLOYEE *employees, int numEmployees, int startRow, int numRows, Rectangle box, char* buffer, int bufferSize, bool active){
    // Draw scrollbar
    Rectangle scrollbar = { 900, 50, 20, numRows * 20 };
    bool scrollbarClicked = false;

    // draw scrollbar and handle input
        if (CheckCollisionPointRec(GetMousePosition(), scrollbar) || scrollbarClicked) {
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
                scrollbarClicked = true;
                startRow = (GetMouseY() - scrollbar.y) * numEmployees / (numRows * 20);
            } else {
                scrollbarClicked = false;
            }
            DrawRectangleRec(scrollbar, DARKGRAY);
        } else {
            DrawRectangleRec(scrollbar, LIGHTGRAY);
        }

    DrawEmployeeData(employees, numEmployees, startRow, numRows);
    DrawInputBox(box, buffer, bufferSize, active);
}

void AddEmpoyee(void){
    return;
}

void RemoveEmployee(void){
    return;
}

int DrawMenu(EMPLOYEE *employees, int numEmployees, int startRow, int numRows, Rectangle box, char* buffer, int bufferSize, bool active) {
    // Define positions, sizes, and colors
    Vector2 titlePosition = {GetScreenWidth() / 2, 50};
    int titleFontSize = 78;
    Color titleColor = DARKGRAY;
    Color shadowColor = BLACK;

    Vector2 lineStart = {0, titlePosition.y + titleFontSize + 10};
    Vector2 lineEnd = {GetScreenWidth(), lineStart.y};
    Color lineColor = DARKGRAY;

    Rectangle buttons[3] = {
        {GetScreenWidth() / 2 - 150, lineEnd.y + 100, 300, 70},
        {GetScreenWidth() / 2 - 150, lineEnd.y + 200, 300, 70},
        {GetScreenWidth() / 2 - 150, lineEnd.y + 300, 300, 70}
    };
    Color buttonColor = LIGHTGRAY;
    const char* buttonText[] = {
        "Search for employee",
        "Add employee",
        "Remove employee"
    };
    int buttonTextFontSize = 20;

    // Draw title with shadow
    DrawTextEx(GetFontDefault(), "Emp Manager", (Vector2){titlePosition.x + 2, titlePosition.y + 2}, titleFontSize, 1, shadowColor);
    DrawTextEx(GetFontDefault(), "Emp Manager", titlePosition, titleFontSize, 1, titleColor);

    // Draw line under title
    DrawLineEx(lineStart, lineEnd, 4, lineColor);

    // Draw rectangles with text inside
    for (int i = 0; i < 3; i++) {
        DrawRectangleRec(buttons[i], buttonColor);
        Vector2 textSize = MeasureTextEx(GetFontDefault(), buttonText[i], buttonTextFontSize, 1);
        Vector2 textPosition = {
            buttons[i].x + (buttons[i].width - textSize.x) / 2,
            buttons[i].y + (buttons[i].height - textSize.y) / 2
        };
        DrawTextEx(GetFontDefault(), buttonText[i], textPosition, buttonTextFontSize, 1, RAYWHITE);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            // Check if the mouse position is inside the rectangle
            if (CheckCollisionPointRec(GetMousePosition(), buttons[i])) {
                return i;
            }
        }
    }
}

