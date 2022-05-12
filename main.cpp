#include <iostream>
#include<iomanip>
#include<ctime>
#include<fstream>
#include<stdlib.h>

int boardB[25][25];
int boardW[25][25];
int countStep = 0;
clock_t timeStart, timeEnd;

static int moveX[8] = { 1,1,2,2,-1,-1,-2,-2 };
static int moveY[8] = { 2,-2,1,-1,2,-2,1,-1 };

using namespace std;

bool isEmpty(int x, int y, int board[][25], int m)
{
    if ((x >= 0 && x < m) && (y >= 0 && y < m) && board[x][y] < 0)
        return true;
    return false;
}

bool BackTracking(int x, int y, int count, int m)
{
    if (count > m * m)
        return true;
    ++countStep;
    int nextX, nextY;
    for (int i = 0; i < 8; ++i)
    {
        nextX = x + moveX[i];
        nextY = y + moveY[i];
        if (isEmpty(nextX, nextY, boardB, m))
        {
            boardB[nextX][nextY] = count;
            if (BackTracking(nextX, nextY, count + 1, m))
                return true;
            else
                boardB[nextX][nextY] = -1;
        }
    }
    return false;
}

int getDegree(int x, int y, int m)
{
    int count = 0, nextX, nextY;
    for (int i = 0; i < 8; ++i)
    {
        nextX = x + moveX[i];
        nextY = y + moveY[i];
        if (isEmpty(nextX, nextY, boardW, m))
            ++count;
    }

    return count;
}

bool Heuristic(int& x, int& y, int m)
{
    ++countStep;
    int minDegreeIndex = -1, d, minDegree = 9, nextX, nextY;
    int start = rand() % 8;
    for (int count = 0; count < 8; ++count)
    {
        int i = (start + count) % 8;
        nextX = x + moveX[i];
        nextY = y + moveY[i];

        if ((isEmpty(nextX, nextY, boardW, m)) && (d = getDegree(nextX, nextY, m)) < minDegree)
        {
            minDegreeIndex = i;
            minDegree = d;
        }
    }

    if (minDegreeIndex == -1)
        return false;

    nextX = x + moveX[minDegreeIndex];
    nextY = y + moveY[minDegreeIndex];

    boardW[nextX][nextY] = boardW[x][y] + 1;

    x = nextX;
    y = nextY;

    return true;
}

void printOutput(int board[][25], int x, int y, int m, int choice)
{
    ofstream fo;
    if (choice == 1)
        fo.open("20127490_backtrack.txt", ios::out | ios::trunc);
    else
        fo.open("20127490_heuristic.txt", ios::out | ios::trunc);
    fo << x << " " << y << " " << m << '\n';
    fo << countStep << '\n';
    fo << fixed << setprecision(3) << double(timeEnd - timeStart) / double(CLOCKS_PER_SEC / 1000) << '\n';
    for (int i = 0; i < m; ++i)
    {
        for (int j = 0; j < m; ++j)
            fo << setw(3) << board[i][j] << " ";
        fo << endl;
    }
    fo.close();
    return;
}

bool isChecking(int x, int y, int m)
{
    int cx = x, cy = y;
    --x, --y;
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < m; ++j)
            boardB[i][j] = -1;

    boardB[y][x] = 1;

    timeStart = clock();
    BackTracking(y, x, 2, m);
    timeEnd = clock();

    printOutput(boardB, cx, cy, m, 1);

    countStep = 0;
    for (int i = 0; i < m; i++)
        for (int j = 0; j < m; j++)
            boardW[i][j] = -1;

    boardW[y][x] = 1;

    timeStart = clock();
    for (int i = 0; i < m * m - 1; ++i)
        Heuristic(y, x, m);
    timeEnd = clock();

    printOutput(boardW, cx, cy, m, 2);
    return true;
}

int main(int argc, char* argv[])
{
    srand(time(NULL));
    int x, y, m;
    x = atoi(argv[2]);
    y = atoi(argv[4]);
    m = atoi(argv[6]);

    isChecking(x, y, m);
    return 0;
}
