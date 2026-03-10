#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <time.h> // nanosleep()

#define ROWS    11 // Grid ROWS length
#define COLS    19 // Grid COLSumn length


void init_grid(int grid[ROWS][COLS]);
struct termios* set_terminal_settings(void);
void user_input(void);
void clear_screen(void); 
void render(const int grid[ROWS][COLS]);

int main() {
    int grid[ROWS][COLS];
    struct timespec* time = {0, 500000000};
    struct timespec* rem;
    
    init_grid(grid);
    struct termios* original_settings = set_terminal_settings();    

    while (1) {
        user_input();
        clear_screen();
        render(grid);
        nanosleep(time, rem); 
    }
    
    tcsetattr(STDOUT_FILENO, TCSANOW, original_settings);
    return 0;
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

struct termios* set_terminal_settings() {
    struct termios* original_settings = (struct termios*)malloc(sizeof(struct termios));  

    /* Save previous settings */
    tcgetattr(STDOUT_FILENO, original_settings);

    struct termios new_settings = *original_settings; 
    new_settings.c_lflag &= ~ECHO; 

    /* Set new settings */ 
    tcsetattr(STDOUT_FILENO, TCSANOW, &new_settings);

    return original_settings;
}

void user_input() {
    /* Read a character */

    /* Do some action based on the character: WASD, c, q */
}

void clear_screen(void) {
    printf("\x1B[2J");
}

void render(const int grid[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            printf("%c", grid[i][j]);
        }    
        printf("\n");
    }
}
