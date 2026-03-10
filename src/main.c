#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <time.h> // nanosleep()
#include <fcntl.h>
#include <sys/select.h>

#define ROWS    11 // Grid ROWS length
#define COLS    19 // Grid COLSumn length
#define UP      'w'
#define DOWN    's'
#define LEFT    'a'
#define RIGHT   'd'
#define QUIT    'q'


typedef struct {
    int y;
    int x; 
} coordinates;

void init_grid(char grid[ROWS][COLS]);
struct termios* set_terminal_settings(void);
void set_non_blocking(void);
void set_blocking(void);
void user_input(void);
void clear_screen(void); 
void render(const char grid[ROWS][COLS]);

static coordinates cursor = {ROWS/2, COLS/2};
bool has_quit = false;

int main() {
    char grid[ROWS][COLS];
    struct timespec time = {0, 500000000};
    struct timespec rem;
    
    init_grid(grid);
    struct termios* original_settings = set_terminal_settings();    
    set_non_blocking();
    while (!has_quit) {
        user_input();
        clear_screen();
        render(grid);
        nanosleep(&time, &rem); 
    }
    
    tcsetattr(STDIN_FILENO, TCSANOW, original_settings);
    set_blocking();

    free(original_settings);
    return 0;
}

void init_grid(char grid[ROWS][COLS]) {
    size_t size = ROWS * COLS * sizeof(char);
    memset(grid, '#', size);

    for (int i = 1; i < ROWS-1; i++) {
        for (int j = 1; j < COLS-1; j++) {
           grid[i][j] = ' '; 
        }    
    }

    /* Set the center cursor */
    //grid[(ROWS)/2][(COLS)/2] = '+';
}

struct termios* set_terminal_settings() {
    struct termios* original_settings = (struct termios*)malloc(sizeof(struct termios));  

    /* Save previous settings */
    tcgetattr(STDIN_FILENO, original_settings);

    struct termios new_settings = *original_settings; 
    new_settings.c_lflag &= ~ECHO; 
    new_settings.c_lflag &= ~ICANON;

    /* Set new settings */ 
    tcsetattr(STDIN_FILENO, TCSANOW, &new_settings);

    return original_settings;
}

void set_non_blocking() {
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl F_GETFL failed");
        return;    
    }    
    
    flags |= O_NONBLOCK;
     
    if (fcntl(STDIN_FILENO, F_SETFL, flags) == -1) {
        perror("fcntl F_SETFL failed");
    }
}

void set_blocking() {
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl F_GETFL failed");
        return;    
    }    
    
    flags &= ~O_NONBLOCK;
    
    if (fcntl(STDIN_FILENO, F_SETFL, flags) == -1) {
        perror("fcntl F_SETFL failed");
    }
}

void user_input() {
    fd_set readfds, writefds, exceptfds;
    FD_ZERO(&readfds);
    FD_ZERO(&writefds);
    FD_ZERO(&exceptfds);
    struct timeval timeout = {0};

    FD_SET(STDIN_FILENO, &readfds);
    if (select(STDIN_FILENO + 1, &readfds, &writefds, &exceptfds, &timeout) > 0) {
        int c = getchar();
        
        /* Action */
        switch(c) {
            case UP:
                cursor.y -= 1;
                break;
            case DOWN:
                cursor.y += 1;
                break;
            case LEFT:
                cursor.x -= 1;
                break;
            case RIGHT:
                cursor.x += 1;
                break;
            case QUIT:
                has_quit = true;
                break;
            default:
                break;
        }        
    }
}

void clear_screen(void) {
    printf("\x1B[2J");
}

void render(const char grid[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (i == cursor.y && j == cursor.x) {
                printf("+");
                continue;
            }
    
            printf("%c", grid[i][j]);
        }    
        printf("\n");
    }
}
