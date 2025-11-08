#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>

#define WIDTH 400
#define HEIGHT 400
#define TITLE "Balls and their admirers"
#define BALL_COUNT 100
#define FPS 60
#define VEL_MAX 5
#define RADIUS_MAX 20
#define RADIUS_MIN 5

Color COLORS[] = {
    LIGHTGRAY, GRAY, DARKGRAY, YELLOW, GOLD, ORANGE, PINK,
    RED, MAROON, GREEN, LIME, DARKGREEN, SKYBLUE, BLUE,
    DARKBLUE, PURPLE, VIOLET, DARKPURPLE, BEIGE, BROWN, DARKBROWN,
};

typedef struct Ball {
    int posx;
    int posy;
    int velx;
    int vely;
    int radius;
    Color color;
    struct Ball *follows;
} Ball;

Ball balls[BALL_COUNT];

Ball *init_ball_random(Ball *p) {
        p->posx = rand() % WIDTH;
        p->posy = rand() % HEIGHT;
        p->velx = rand() % VEL_MAX;
        p->vely = rand() % VEL_MAX;
        p->radius = rand() % (RADIUS_MAX - RADIUS_MIN + 1) + RADIUS_MIN;
        p->color = COLORS[rand() % 21];
        p->follows = NULL;
        return p;
}

void init_balls_random() { // This is our main function for generating the balls.
    srand(time(NULL)); // We need to seed it with time, for it to be random. 

    Ball *place = malloc(sizeof(Ball)); 

    for (int i; i<BALL_COUNT; i++) { // We loop through every single variable in the structure (0-99)
        place = &balls[i]; //And first we assign the structure variables memory adress (ie. 0, 1, 2 and so forth) to a placeholder variable
        *place = *init_ball_random(place); // The we generate all of the values for a balls structure (ie. posx, posy, velx, and so forth) and assigb it to the pointer of the placeholder variable, which again holds the memory adress of the structure. 

        while(1){ // We need to assign the ball to follow each other. So we create a loop, the assigns one of the other balls to the current ball. 
        place = &balls[rand() % BALL_COUNT]; // Generate a random number. 
        if (place != &balls[i]) {balls[i].follows = place; break;} // If the number isn't itself, then assign it and break the while loop.
        }
    }
}

Ball *draw_ball(Ball *p) {
  DrawCircle(p->posx, p->posy, p->radius, p->color);
  return p;
}

// Updates the positions of balls according to their velocities
Ball *update_pos(Ball *p) {
  p->posx = (WIDTH + p->posx + p->velx) %
            WIDTH; // `WIDTH +` because C uses truncated division
  p->posy = (HEIGHT + p->posy + p->vely) % HEIGHT;
  return p;
}

// Updates the velocity of a ball so that it follows the leading ball
Ball *update_vel_for_following(Ball *p) {
  int errx = p->follows->posx - p->posx;
  int erry = p->follows->posy - p->posy;
  p->velx = errx > 0 ? 1 : -1;
  p->vely = erry > 0 ? 1 : -1;
  return p;
}

// Update all elements
void update_elements() {
  for (size_t i = 0; i < BALL_COUNT; ++i) {
    draw_ball(update_pos(update_vel_for_following(&balls[i])));
  }
}

int main() {
  InitWindow(WIDTH, HEIGHT, TITLE);
  SetTargetFPS(FPS);

  init_balls_random();

  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    BeginDrawing();
    update_elements();
    ClearBackground(RAYWHITE);
    EndDrawing();
  }
}