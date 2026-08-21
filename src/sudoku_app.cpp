#include "apps.h"

#include <M5Unified.h>
#include <Preferences.h>
#include <cstring>

#include "native_display.h"

namespace {
constexpr uint8_t kPuzzle[81] = {
    5,3,0,0,7,0,0,0,0,
    6,0,0,1,9,5,0,0,0,
    0,9,8,0,0,0,0,6,0,
    8,0,0,0,6,0,0,0,3,
    4,0,0,8,0,3,0,0,1,
    7,0,0,0,2,0,0,0,6,
    0,6,0,0,0,0,2,8,0,
    0,0,0,4,1,9,0,0,5,
    0,0,0,0,8,0,0,7,9,
};

constexpr uint8_t kSolution[81] = {
    5,3,4,6,7,8,9,1,2,
    6,7,2,1,9,5,3,4,8,
    1,9,8,3,4,2,5,6,7,
    8,5,9,7,6,1,4,2,3,
    4,2,6,8,5,3,7,9,1,
    7,1,3,9,2,4,8,5,6,
    9,6,1,5,3,7,2,8,4,
    2,8,7,4,1,9,6,3,5,
    3,4,5,2,8,6,1,7,9,
};

constexpr int kBoardX = 206;
constexpr int kBoardY = 20;
constexpr int kCell = 40;
uint8_t cells[81];
uint8_t cursor = 0;
bool solved = false;
Preferences prefs;

bool isEditable(uint8_t index) { return kPuzzle[index] == 0; }

void selectEditable(int direction) {
  for (int tries = 0; tries < 81; ++tries) {
    cursor = static_cast<uint8_t>((cursor + 81 + direction) % 81);
    if (isEditable(cursor)) return;
  }
}

bool isConflict(uint8_t index) {
  const uint8_t value = cells[index];
  if (!value) return false;
  const int row = index / 9;
  const int col = index % 9;
  for (int i = 0; i < 9; ++i) {
    const int rowPeer = row * 9 + i;
    const int colPeer = i * 9 + col;
    if ((rowPeer != index && cells[rowPeer] == value) ||
        (colPeer != index && cells[colPeer] == value)) return true;
  }
  const int boxRow = row / 3 * 3;
  const int boxCol = col / 3 * 3;
  for (int r = boxRow; r < boxRow + 3; ++r) {
    for (int c = boxCol; c < boxCol + 3; ++c) {
      const int peer = r * 9 + c;
      if (peer != index && cells[peer] == value) return true;
    }
  }
  return false;
}

bool checkSolved() { return std::memcmp(cells, kSolution, sizeof(cells)) == 0; }

void save() {
  prefs.begin("sudoku", false);
  prefs.putBytes("cells", cells, sizeof(cells));
  prefs.putUChar("cursor", cursor);
  prefs.end();
}

void resetGame() {
  std::memcpy(cells, kPuzzle, sizeof(cells));
  cursor = 0;
  selectEditable(1);
  solved = false;
  save();
}

void load() {
  prefs.begin("sudoku", true);
  if (prefs.getBytesLength("cells") == sizeof(cells)) {
    prefs.getBytes("cells", cells, sizeof(cells));
    cursor = prefs.getUChar("cursor", 0) % 81;
  } else {
    std::memcpy(cells, kPuzzle, sizeof(cells));
  }
  prefs.end();
  for (int i = 0; i < 81; ++i) {
    if (kPuzzle[i] != 0) cells[i] = kPuzzle[i];
    if (cells[i] > 9) cells[i] = 0;
  }
  if (!isEditable(cursor)) selectEditable(1);
  solved = checkSolved();
}

void render() {
  M5.Display.startWrite();
  M5.Display.fillScreen(WHITE);
  M5.Display.setTextDatum(middle_center);
  M5.Display.setTextColor(BLACK, WHITE);
  M5.Display.setFont(&fonts::FreeSansBold18pt7b);
  M5.Display.drawString("SUDOKU", 100, 55);
  M5.Display.setFont(&fonts::FreeSans9pt7b);
  M5.Display.drawString("A / C: move", 100, 125);
  M5.Display.drawString("B: number", 100, 155);
  M5.Display.drawString("Hold B: clear", 100, 185);
  M5.Display.drawString("Hold A: reset", 100, 215);
  if (solved) {
    M5.Display.fillRoundRect(20, 270, 160, 58, 10, GREEN);
    M5.Display.setTextColor(WHITE, GREEN);
    M5.Display.setFont(&fonts::FreeSansBold12pt7b);
    M5.Display.drawString("SOLVED!", 100, 300);
  }

  for (int index = 0; index < 81; ++index) {
    const int row = index / 9;
    const int col = index % 9;
    const int x = kBoardX + col * kCell;
    const int y = kBoardY + row * kCell;
    uint32_t background = WHITE;
    if (index == cursor) background = YELLOW;
    else if ((row / 3 + col / 3) % 2) background = 0xFFDE;
    M5.Display.fillRect(x + 1, y + 1, kCell - 1, kCell - 1, background);
    if (cells[index]) {
      const uint32_t color = isConflict(index) ? RED : (isEditable(index) ? BLUE : BLACK);
      M5.Display.setTextColor(color, background);
      M5.Display.setFont(isEditable(index) ? &fonts::FreeSans18pt7b : &fonts::FreeSansBold18pt7b);
      M5.Display.drawNumber(cells[index], x + kCell / 2, y + kCell / 2 + 1);
    }
  }
  for (int i = 0; i <= 9; ++i) {
    const int width = i % 3 == 0 ? 3 : 1;
    M5.Display.fillRect(kBoardX + i * kCell - width / 2, kBoardY, width, 360, BLACK);
    M5.Display.fillRect(kBoardX, kBoardY + i * kCell - width / 2, 360, width, BLACK);
  }
  M5.Display.endWrite();
}
}

void sudokuSetup() {
  beginNativeDisplay();
  load();
  render();
}

void sudokuLoop() {
  M5.update();
  bool changed = false;
  if (M5.BtnA.wasReleasedAfterHold()) {
    resetGame();
    changed = true;
  } else if (M5.BtnA.wasReleased()) {
    selectEditable(-1);
    changed = true;
  }
  if (M5.BtnB.wasReleasedAfterHold()) {
    cells[cursor] = 0;
    changed = true;
  } else if (M5.BtnB.wasReleased()) {
    cells[cursor] = static_cast<uint8_t>((cells[cursor] + 1) % 10);
    changed = true;
  }
  if (M5.BtnC.wasReleased()) {
    selectEditable(1);
    changed = true;
  }
  if (changed) {
    solved = checkSolved();
    save();
    render();
  }
  delay(20);
}
