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
    ttystate.c_lflag &= ~(ICANON | ECHO); // Turn off canonical mode and echo
    ttystate.c_cc[VMIN] = 0;
    ttystate.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
}
#define TOPEDGE 0
#define LEFTEDGE 0
#define RIGHTEDGE 40
#define BOTTOMEDGE 21
#define PADDLE_SIZE 6
#define BALL_MAGNITUDE 1

struct ball_vector {
    int x;
    int y;
    int x_mag;
    int y_mag;
};

struct paddle {
    int y;
};



void draw_walls() {
    char horizontal_wall_piece[] = "-";
    char vertical_wall_piece[] = "|";

    for(int y = TOPEDGE; y <= BOTTOMEDGE; y++) {
        move(y, LEFTEDGE);
        addstr(vertical_wall_piece);
    }

    for(int x = LEFTEDGE; x <= RIGHTEDGE; x++) {
        move(TOPEDGE, x);
        addstr(horizontal_wall_piece);
        move(BOTTOMEDGE, x);
        addstr(horizontal_wall_piece);
    }
}

void draw_paddle(struct paddle *paddle) {
    for(int i = paddle->y; i < PADDLE_SIZE + paddle->y; i++) {
        move(i, RIGHTEDGE);
        addstr("#");
    }
}

void draw_ball(struct ball_vector *ball) {
    move(ball->y, ball->x);
    addstr("O");
}

void paddle_bounds(struct paddle *paddle) {
    if(paddle->y < TOPEDGE + 1) {
        paddle->y = TOPEDGE + 1;
    }
    if(paddle->y > BOTTOMEDGE - PADDLE_SIZE) {
        paddle->y = BOTTOMEDGE - PADDLE_SIZE;
    }
}


void move_ball(struct ball_vector *ball) {
    ball->x = ball->x + ball->x_mag;
    ball->y = ball->y + ball->y_mag;

    if(ball->y < TOPEDGE + 2) {
        ball->y_mag = 0 - ball->y_mag;
    }
    if(ball->y > BOTTOMEDGE - 2) {
        ball->y_mag = 0 - ball->y_mag;
    }
    if(ball->x < LEFTEDGE + 2) {
        ball->x_mag = 0 - ball->x_mag;
    }
}

void move_paddle(struct paddle *paddle, char c) {
    if (c == 'j') {
        paddle->y++;
    }
    else if (c == 'k') {
        paddle->y--;
    }

}

int check_collision(struct ball_vector *ball, struct paddle *paddle) {
    if(ball->x > RIGHTEDGE - 2) {
        int difference = ball->y - paddle->y;
        if(difference > PADDLE_SIZE + 1 || difference < 0) {
            return -1;
        } else {
            ball->x_mag = 0 - ball->x_mag;
        }
    }
    return 1;
}

int main() {
    initscr();
    srand(time(NULL));
    setNonBlockingMode();

    int init_x_mag = 0 - rand()%3;
    while (init_x_mag >= 0) {
        init_x_mag = 0 - rand()%3;
    }
    
    int init_y_mag = rand()%3;
    while (init_y_mag <= 0) {
        init_y_mag = rand()%3;
    }

    char c;

    struct ball_vector ball = {RIGHTEDGE, TOPEDGE + rand()%BOTTOMEDGE, init_x_mag, init_y_mag};
    struct paddle paddle = {1};

    while(c != 'q') {
        if (read(STDIN_FILENO, &c, 1) == 1) {
            move_paddle(&paddle, c);
        }

        paddle_bounds(&paddle);
        
        move_ball(&ball);

        if(check_collision(&ball, &paddle) == -1) {
            c = 'q';
        }

        draw_walls();
        draw_paddle(&paddle);
        draw_ball(&ball);

        refresh();
        usleep(200000);
        clear();

    }


    endwin();

    printf("Ball pos: %i\nPaddle pos: %i\nDifference (more than 6 or less than 0 -> loss): %i\n", ball.y, paddle.y, ball.y-paddle.y);

    return 0;
}