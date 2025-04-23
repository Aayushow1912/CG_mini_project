#include <graphics.h>
#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
#include <dos.h>  // For sound, nosound, delay

#define MAX 10
#define CELL 20

// Function Prototypes
void drawMaze();
void movePlayer(int key);
void newMaze();
void initMaze();
void generateMaze(int r, int c);
void removeWall(int r1, int c1, int r2, int c2);
void drawCell(int r, int c);
void sound(unsigned frequency);
void nosound();
void delay(unsigned msec);

int ROWS = 10, COLS = 10;
int playerRow = 0, playerCol = 0;
int moveCount = 0;
char info[100];

struct Cell {
    int top, bottom, left, right, visited;
} maze[MAX][MAX];

void initMaze() {
    int i, j;
    for (i = 0; i < MAX; ++i)
        for (j = 0; j < MAX; ++j) {
            maze[i][j].top = 1;
            maze[i][j].bottom = 1;
            maze[i][j].left = 1;
            maze[i][j].right = 1;
            maze[i][j].visited = 0;
        }
}

void removeWall(int r1, int c1, int r2, int c2) {
    if (r1 == r2) {
        if (c1 > c2) {
            maze[r1][c1].left = 0;
            maze[r2][c2].right = 0;
        } else {
            maze[r1][c1].right = 0;
            maze[r2][c2].left = 0;
        }
    } else {
        if (r1 > r2) {
            maze[r1][c1].top = 0;
            maze[r2][c2].bottom = 0;
        } else {
            maze[r1][c1].bottom = 0;
            maze[r2][c2].top = 0;
        }
    }
}

void generateMaze(int r, int c) {
    int i, j, temp, dirs[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
    maze[r][c].visited = 1;

    for (i = 0; i < 4; ++i) {
        j = random(4);
        temp = dirs[i][0];
        dirs[i][0] = dirs[j][0];
        dirs[j][0] = temp;
        temp = dirs[i][1];
        dirs[i][1] = dirs[j][1];
        dirs[j][1] = temp;
    }

    for (i = 0; i < 4; ++i) {
        int nr = r + dirs[i][0];
        int nc = c + dirs[i][1];
        if (nr >= 0 && nr < ROWS && nc >= 0 && nc < COLS && maze[nr][nc].visited == 0) {
            removeWall(r, c, nr, nc);
            generateMaze(nr, nc);
        }
    }
}

void drawCell(int r, int c) {
    int x = 50 + c * CELL;
    int y = 50 + r * CELL;

    setcolor(WHITE);
    if (maze[r][c].top) line(x, y, x + CELL, y);
    if (maze[r][c].bottom) line(x, y + CELL, x + CELL, y + CELL);
    if (maze[r][c].left) line(x, y, x, y + CELL);
    if (maze[r][c].right) line(x + CELL, y, x + CELL, y + CELL);
}

void drawMaze() {
    int i, j;
    setcolor(YELLOW);
    outtextxy(180, 20, "Maze Game - Use Arrow Keys");

    sprintf(info, "Moves: %d", moveCount);
    outtextxy(180, 35, info);

    for (i = 0; i < ROWS; ++i)
        for (j = 0; j < COLS; ++j)
            drawCell(i, j);

    // Goal Cell
    int gx = 50 + (COLS - 1) * CELL;
    int gy = 50 + (ROWS - 1) * CELL;
    setfillstyle(SOLID_FILL, BLUE);
    bar(gx + 2, gy + 2, gx + CELL - 2, gy + CELL - 2);

    // Player
    int px = 50 + playerCol * CELL;
    int py = 50 + playerRow * CELL;
    setfillstyle(SOLID_FILL, GREEN);
    bar(px + 4, py + 4, px + CELL - 4, py + CELL - 4);
}

void movePlayer(int key) {
    if (key == 72 && playerRow > 0 && maze[playerRow][playerCol].top == 0) {
        playerRow--;
    } else if (key == 80 && playerRow < ROWS - 1 && maze[playerRow][playerCol].bottom == 0) {
        playerRow++;
    } else if (key == 75 && playerCol > 0 && maze[playerRow][playerCol].left == 0) {
        playerCol--;
    } else if (key == 77 && playerCol < COLS - 1 && maze[playerRow][playerCol].right == 0) {
        playerCol++;
    }
    moveCount++;
}

void newMaze() {
    initMaze();
    generateMaze(0, 0);
    playerRow = 0;
    playerCol = 0;
    moveCount = 0;
}

int main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "C:\\Turboc3\\BGI");

    randomize();
    newMaze();

    while (1) {
        cleardevice();
        drawMaze();

        if (kbhit()) {
            int key = getch();
            if (key == 0) key = getch();
            movePlayer(key);
        }

        if (playerRow == ROWS - 1 && playerCol == COLS - 1) {
            setcolor(LIGHTGREEN);
            outtextxy(180, 450, "You reached the goal! Press any key...");
            int f;
            for (f = 300; f <= 800; f += 100) {
                sound(f);
                delay(100);
            }
            nosound();
            getch();
            newMaze();
        }

        delay(50);
    }

    closegraph();
    return 0;
}

