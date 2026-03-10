#include <stdio.h> 
#include <string.h>

#define ROWS    11 // Grid ROWS length
#define COLS    19 // Grid COLSumn length

void render(const int grid[ROWS][COLS]);
void init_grid(int grid[ROWS][COLS]);

int main() {
    int grid[ROWS][COLS];
    init_grid(grid);
    render(grid);
}

void init_grid(int grid[ROWS][COLS]) {
    size_t size = ROWS * COLS * sizeof(int);
    memset(grid, '#', size);

    for (int i = 1; i < ROWS-1; i++) {
        for (int j = 1; j < COLS-1; j++) {
           grid[i][j] = ' '; 
        }    
    }

    /* Set the center cursor */
    grid[(ROWS)/2][(COLS)/2] = '+';
}

void render(const int grid[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            printf("%c", grid[i][j]);
        }    
        printf("\n");
    }
}
