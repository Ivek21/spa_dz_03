#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

#ifdef _WIN32
#include <windows.h>
void setCursor(int row, int col) {
    COORD coord;
    coord.X = (SHORT)col;
    coord.Y = (SHORT)row;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void hideCursor() {
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 1;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
}
void showCursor() {
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 1;
    info.bVisible = TRUE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
}
void clearScreen() { system("cls"); }
#else
void setCursor(int row, int col) { printf("\033[%d;%dH", row + 1, col + 1); }
void hideCursor() { printf("\033[?25l"); fflush(stdout); }
void showCursor() { printf("\033[?25h"); fflush(stdout); }
void clearScreen() { printf("\033[2J\033[H"); fflush(stdout); }
#endif

const int ROWS = 20;
const int COLS = 40;

void drawGrid(int pR, int pC, int sR, int sC, int eR, int eC, bool arrived) {
    for (int r = 0; r < ROWS; r++) {
        setCursor(r, 0);
        for (int c = 0; c < COLS; c++) {
            if (r == pR && c == pC) {
                std::cout << "x ";
            }
            else if (r == eR && c == eC && !arrived) {
                std::cout << "B ";
            }
            else if (r == sR && c == sC && (r != pR || c != pC)) {
                std::cout << "A ";
            }
            else {
                std::cout << "- ";
            }
        }
    }
    std::cout.flush();
}

int main() {
    int sR, sC, eR, eC;
    std::cout << "A redak: "; std::cin >> sR;
    std::cout << "A stupac: "; std::cin >> sC;
    std::cout << "B redak: "; std::cin >> eR;
    std::cout << "B stupac: "; std::cin >> eC;
    sR--; sC--; eR--; eC--;

    if (sR < 0 || sR >= ROWS || sC < 0 || sC >= COLS ||
        eR < 0 || eR >= ROWS || eC < 0 || eC >= COLS) {
        showCursor();
        std::cout << "Neispravne koordinate!\n";
        return 1;
    }

    std::vector<std::pair<int, int>> path;
    int r = sR, c = sC;
    path.push_back(std::make_pair(r, c));
    while (c != eC) {
        c += (eC > c) ? 1 : -1;
        path.push_back(std::make_pair(r, c));
    }
    while (r != eR) {
        r += (eR > r) ? 1 : -1;
        path.push_back(std::make_pair(r, c));
    }

    clearScreen();
    hideCursor();

    for (int i = 0; i < (int)path.size() - 1; i++) {
        drawGrid(path[i].first, path[i].second, sR, sC, eR, eC, false);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    drawGrid(eR, eC, sR, sC, eR, eC, true);
    setCursor(ROWS + 1, 0);

    return 0;
}