/*  
 *   Sceptre's ANSI C maze generator
 *   c2015
 *
 *   1. Start from a cell and mark it 'visited'
 *   2. Pick a direction.
 *   3. If the nearest cell in that direction is 'unvisited' or you want to make a loop here, move to it.
 *   4. Mark it 'visited'
 *   5. Move until there is no space left.
 *   6. Pick a random 'visited' cell.
 *   7. Go to step 2 until you're satisfied with the maze.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
 
#define MAZE_W 60
#define MAZE_H 25

#define CELL_RADIUS 2
#define CELL_ROW_W (MAZE_W / CELL_RADIUS - 1)
#define CELL_COLUMN_H (MAZE_H / CELL_RADIUS - 1)

#define MAGIC 666

/* 
 *   dirList corresponds to cardinal directions. 0 = north, 1 = east, 2 = south, 3 = west  
 *   This saves us from making a bunch of similar conditionals later on. 
 */
int dirList[4][2] = {
    {  0, -1 },
    {  1,  0 }, 
    {  0,  1 }, 
    { -1,  0 } 
};
 
int map[MAZE_W][MAZE_H];
int cell[CELL_ROW_W][CELL_COLUMN_H];
  
void makeMaze(void);
void printMaze(void);

int main(void)
{
    srand(time(0));
    makeMaze();
    printMaze();
 
    return 0;
}

/* Start map, all cells are initially walls/blocked.  */
void initGrid(void)
{
    int i, j;
    for (i = 0; i < MAZE_W; i++) {
        for (j = 0; j < MAZE_H; j++){
            map[i][j] = 1;
        }
    }
    memset(cell, 0, sizeof(cell));
}
 
/* Returns 1 (true) if (x, y) isn't out of the map boundaries. */
int isInBounds(int x, int y)
{
    if ( x > 2 && y > 2 && x < MAZE_W - 2 && y < MAZE_H - 2){
        return 1;
    } else {
        return 0;
    }
}
 
/* Links the cells in our map. */
void linkCells(int x0, int y0, int x1, int y1)
{
    int cx = x0; int cy = y0;
    while(cx != x1) {
        if (x0 > x1){
            cx--;
        } else {
            cx++;
        }
        if ( isInBounds(cx, cy) ){
            map[cx][cy] = 0;
        }
    }
    while(cy != y1) {
        if (y0 > y1){
            cy--;
        } else {
            cy++;
        }
        if ( isInBounds(cx, cy) ){
            map[cx][cy] = 0;
        }
    }
}
 
 /* Fills cells in our map at the appropriate size. */
void fillCells(void)
{
    int i, j;
    for (i = 0; i < CELL_ROW_W; i++ ){
        for (j = 0; j < CELL_COLUMN_H; j++){
            if (cell[i][j] == 1){
                map[i * CELL_RADIUS][j * CELL_RADIUS] = 0;
            }
        }
    }
}
 
void makeMaze(void)
{   
    int rx = 0; int ry = 0;
    int dx; int dy;
    int dir = 0;
    int count = 0;
 
    int totalCells = CELL_ROW_W * CELL_COLUMN_H;
 
    initGrid();
 
    /* Replace rx/ry with desired startX/startY. */
    rx = CELL_ROW_W / 2;
    ry = CELL_COLUMN_H / 2;
 
    cell[rx][ry] = 1;
    int visitedCells = 1; 

    while( visitedCells < totalCells ) {
        count++;
        if (count < MAGIC){
            fillCells();
        }
        /* Use Direction Lookup table for more Generic dig function. */
        dir = rand()%4; 
        dx = dirList[dir][0];
        dy = dirList[dir][1];
 
        /* 
         *  Check whether target cell is free.
         *  If it isn't, and rand()%7 == 6, link it anyways.
         *  This creates 'loops' and gives a more cavelike appearance.
         */
        if( isInBounds( rx * CELL_RADIUS + dx, ry * CELL_RADIUS + dy)
                        && cell[rx + dx][ry + dy] == 0
                        || rand()%7 == 6){
            linkCells( rx * CELL_RADIUS, ry * CELL_RADIUS, (rx+dx) * CELL_RADIUS, (ry+dy) * CELL_RADIUS);
            rx += dx;
            ry += dy;
        } else {
            do {
                rx = rand()% CELL_ROW_W;
                ry = rand()% CELL_COLUMN_H;
            } while ( cell[rx][ry] != 1 );
        }
        cell[rx][ry] = 1;
        map[rx * CELL_RADIUS][ry * CELL_RADIUS] = 0;
        visitedCells++;
    }
    fillCells();
}
 
void printMaze(void)
{
    int i, j;
    for (j = 0; j < MAZE_H; j++){
        for (i = 0; i < MAZE_W; i++){
            if (map[i][j] == 1)
                printf("#");
            else if (map[i][j] == 0)
                printf(".");
        }
        printf("\n");
    }
    printf("VOILA!");
}
