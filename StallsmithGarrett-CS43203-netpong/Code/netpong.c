#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>

// Function to set terminal attributes for non-blocking input
void setNonBlockingMode() {
    struct termios ttystate;
    tcgetattr(STDIN_FILENO, &ttystate);
    // Turn off canonical mode and echo
    ttystate.c_lflag &= ~(ICANON | ECHO);
    ttystate.c_cc[VMIN] = 0;
    ttystate.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
}

// Gameplay constants
// Change these to change aspects of the game
#define TOPEDGE 0
#define LEFTEDGE 0
#define RIGHTEDGE 40
#define BOTTOMEDGE 15
#define PADDLE_SIZE 5

// Vectorization of the movement
// Considering magnitude and direction of x, y coordinates separately
struct ball_vector {
    int x;
    int y;
    int x_mag;
    int y_mag;
};

struct paddle {
    int y;
};

// Visual representation of walls
void draw_walls(char side) {
    char horizontal_wall_piece[] = "-";
    char vertical_wall_piece[] = "|";

    for(int y = TOPEDGE; y <= BOTTOMEDGE; y++) {
        if(side == 'L') {
            move(y, RIGHTEDGE);
        } else if(side == 'R')  {
            move(y, LEFTEDGE);
        }
        addstr(vertical_wall_piece);
    }

    for(int x = LEFTEDGE; x <= RIGHTEDGE; x++) {
        move(TOPEDGE, x);
        addstr(horizontal_wall_piece);
        move(BOTTOMEDGE, x);
        addstr(horizontal_wall_piece);
    }
}

// Visutal representation of paddle
void draw_paddle(struct paddle *paddle, char side) {
    for(int i = paddle->y; i < PADDLE_SIZE + paddle->y; i++) {
        if(side == 'L') {
            move(i, LEFTEDGE);

        } else if(side == 'R') {
            move(i, RIGHTEDGE);

        }
        addstr("#");
    }
}

// Visual representation of ball
void draw_ball(struct ball_vector *ball) {
    move(ball->y, ball->x);
    addstr("O");
}

// Tells the user how many serves they have left
void draw_serves(int serves) {
    move(BOTTOMEDGE + 3, RIGHTEDGE/4);
    char c_serves[10];
    sprintf(c_serves, "%d", serves);

    char serves_msg[] = "Serves remaining: ";
    addstr(serves_msg);
    addstr(c_serves);
}

// Makes sure the paddle stays in the game
// Prevents user from moving the paddle too far above or below the game
void paddle_bounds(struct paddle *paddle) {
    if(paddle->y < TOPEDGE + 1) {
        paddle->y = TOPEDGE + 1;
    }
    if(paddle->y > BOTTOMEDGE - PADDLE_SIZE) {
        paddle->y = BOTTOMEDGE - PADDLE_SIZE;
    }
}

// Apply magnitude to ball coordinates
void move_ball(struct ball_vector *ball, char side) {

    // If the ball hits a wall invert the direction
    if(ball->y < TOPEDGE + 2) {
        ball->y_mag = 0 - ball->y_mag;
    }
    if(ball->y > BOTTOMEDGE - 2) {
        ball->y_mag = 0 - ball->y_mag;
    }
    if(ball->x < LEFTEDGE + 1 && side == 'R') {
        ball->x_mag = 0 - ball->x_mag;
    } else if (ball->x > RIGHTEDGE - 1 && side == 'L') {
        ball->x_mag = 0 - ball->x_mag;
    }

    ball->x = ball->x + ball->x_mag;
    ball->y = ball->y + ball->y_mag;

}

// Moves the paddle using j and k keys
void move_paddle(struct paddle *paddle, char c) {
    if (c == 'j') {
        paddle->y++;
    }
    else if (c == 'k') {
        paddle->y--;
    }

}

// Check if ball and paddle hit, if not return -1
// -1 results in lost serve
// Otherwise, ball and paddle hit, invert ball direction
int check_collision(struct ball_vector *ball, struct paddle *paddle, char side) {
    if(side == 'R') {
        if(ball->x > RIGHTEDGE - 1) {
            int difference = ball->y - paddle->y;
            if(difference > PADDLE_SIZE + 1 || difference < 0) {
                return -1;
            } else {
                ball->x_mag = 0 - ball->x_mag;
            }
        }
    } else if(side == 'L') {
        if(ball->x < LEFTEDGE + 1) {
            int difference = ball->y - paddle->y;
            if(difference > PADDLE_SIZE + 1 || difference < 0) {
                return -1;
            } else {
                ball->x_mag = 0 - ball->x_mag;
            }
        }

    }

    return 1;
}

// Reseve the ball after a loss state
void reserve(struct ball_vector *ball, char side) {
    ball->y = TOPEDGE + rand()%BOTTOMEDGE;

    int init_x_mag;

    if(side == 'L') {
        ball->x = LEFTEDGE + 2;

        init_x_mag = 0 + rand()%3;
        while (init_x_mag >= 0) {
            init_x_mag = 0 + rand()%3;
        }
        
    } else if (side == 'R') {

        ball->x = RIGHTEDGE - 2;
        init_x_mag = 0 - rand()%3;
        while (init_x_mag >= 0) {
            init_x_mag = 0 - rand()%3;
        }
        
    }

    int init_y_mag = rand()%3;
    while (init_y_mag <= 0) {
        init_y_mag = rand()%3;
    }

    ball->x_mag = init_x_mag;
    ball->y_mag = init_y_mag;

}

int main(int argc, char* argv[]) {
    if(argc < 2) {
        perror("Pick a side of the court to play on.\nL for left\nR for right\n");
        return 0;
    }

    char side = *argv[1];

    initscr();
    srand(time(NULL));

    // Allows us to receive user input
    setNonBlockingMode();

    // Initialize vectors to random speed between 1-6
    int init_x_mag = 0 - rand()%7;
    while (init_x_mag >= 0) {
        init_x_mag = 0 - rand()%7;
    }
    
    int init_y_mag = rand()%3;
    while (init_y_mag <= 0) {
        init_y_mag = rand()%3;
    }

    int serves = 3;

    // This keeps track of user input
    char c;

    struct ball_vector ball = {RIGHTEDGE - 2, TOPEDGE + rand()%BOTTOMEDGE, init_x_mag, init_y_mag};
    struct paddle paddle = {1};

    while(c != 'q' && serves > 0) {
        if (read(STDIN_FILENO, &c, 1) == 1) {
            move_paddle(&paddle, c);
        }

        paddle_bounds(&paddle);
        
        move_ball(&ball, side);

        if(check_collision(&ball, &paddle, side) == -1) {
            reserve(&ball, side);
            serves--;
        }

        draw_serves(serves);

        draw_walls(side);
        draw_paddle(&paddle, side);
        draw_ball(&ball);

        refresh();

        // Game updates every 1/5 second
        usleep(200000);
        clear();

    }

    // Gracefully exit and return to normal terminal
    endwin();


    return 0;
}
