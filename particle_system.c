#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define MAX_PARTICLES 100
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 24
#define GRAVITY 0.098
#define DAMPING 0.99
#define EXPLOSION_FORCE 5.0

typedef struct {
    double x, y;
    double vx, vy;
    double ax, ay;
    int lifetime;
    int max_lifetime;
    char symbol;
    int active;
} Particle;

typedef struct {
    Particle particles[MAX_PARTICLES];
    char screen[SCREEN_HEIGHT][SCREEN_WIDTH];
    int particle_count;
} ParticleSystem;


void init_system(ParticleSystem *ps) {
    ps->particle_count = 0;
    for (int i = 0; i < MAX_PARTICLES; i++) {
        ps->particles[i].active = 0;
    }
}

void clear_screen(ParticleSystem *ps) {
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            ps->screen[y][x] = ' ';
        }
    }
}

void create_explosion(ParticleSystem *ps, double x, double y, int count) {
    for (int i = 0; i < count && ps->particle_count < MAX_PARTICLES; i++) {

        int idx = -1;
        for (int j = 0; j < MAX_PARTICLES; j++) {
            if (!ps->particles[j].active) {
                idx = j;
                break;
            }
        }

        if (idx == -1) continue;

        Particle *p = &ps->particles[idx];
        double angle = (rand() % 360) * M_PI / 180.0;
        double speed = (rand() % 100) / 100.0 * EXPLOSION_FORCE;

        p->x = x;
        p->y = y;
        p->vx = cos(angle) * speed;
        p->vy = sin(angle) * speed;
        p->ax = 0;
        p->ay = GRAVITY;
        p->lifetime = 50 + (rand() % 50);
        p->max_lifetime = p->lifetime;
        p->active = 1;
        char symbols[] = {'*', '+', 'o', '.', 'x', '#'};
        p->symbol = symbols[rand() % 6];

        ps->particle_count++;
    }
}

void create_fountain(ParticleSystem *ps, double x, double y) {
    int count = 3;
    for (int i = 0; i < count && ps->particle_count < MAX_PARTICLES; i++) {
        int idx = -1;
        for (int j = 0; j < MAX_PARTICLES; j++) {
            if (!ps->particles[j].active) {
                idx = j;
                break;
            }
        }

        if (idx == -1) continue;

        Particle *p = &ps->particles[idx];


        double angle = -M_PI/2 + ((rand() % 60) - 30) * M_PI / 180.0;
        double speed = 3.0 + (rand() % 100) / 100.0;

        p->x = x;
        p->y = y;
        p->vx = cos(angle) * speed;
        p->vy = sin(angle) * speed;
        p->ax = 0;
        p->ay = GRAVITY;
        p->lifetime = 40 + (rand() % 30);
        p->max_lifetime = p->lifetime;
        p->active = 1;
        p->symbol = '*';

        ps->particle_count++;
    }
}

void update_particles(ParticleSystem *ps) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        Particle *p = &ps->particles[i];

        if (!p->active) continue;


        p->vx += p->ax;
        p->vy += p->ay;


        p->vx *= DAMPING;
        p->vy *= DAMPING;

        p->x += p->vx;
        p->y += p->vy;


        if (p->x < 0) {
            p->x = 0;
            p->vx = -p->vx * 0.8;
        }
        if (p->x >= SCREEN_WIDTH) {
            p->x = SCREEN_WIDTH - 1;
            p->vx = -p->vx * 0.8;
        }


        if (p->y >= SCREEN_HEIGHT) {
            p->y = SCREEN_HEIGHT - 1;
            p->vy = -p->vy * 0.6;
            p->vx *= 0.8;
        }


        p->lifetime--;
        if (p->lifetime <= 0) {
            p->active = 0;
            ps->particle_count--;
        }
    }
}

void render_particles(ParticleSystem *ps) {
    clear_screen(ps);

    for (int i = 0; i < MAX_PARTICLES; i++) {
        Particle *p = &ps->particles[i];

        if (!p->active) continue;

        int screen_x = (int)p->x;
        int screen_y = (int)p->y;

        if (screen_x >= 0 && screen_x < SCREEN_WIDTH &&
            screen_y >= 0 && screen_y < SCREEN_HEIGHT) {


            char display_char = p->symbol;
            if (p->lifetime < p->max_lifetime / 4) {
                display_char = '.';
            }

            ps->screen[screen_y][screen_x] = display_char;
        }
    }
}

void display_screen(ParticleSystem *ps) {

    printf("\033[H\033[J");


    for (int x = 0; x < SCREEN_WIDTH + 2; x++) printf("=");
    printf("\n");


    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        printf("|");
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            putchar(ps->screen[y][x]);
        }
        printf("|\n");
    }


    for (int x = 0; x < SCREEN_WIDTH + 2; x++) printf("=");
    printf("\n");

    printf("Active particles: %d | Press Ctrl+C to exit\n", ps->particle_count);
    fflush(stdout);
}

int main() {
    ParticleSystem ps;
    init_system(&ps);

    srand(time(NULL));

    int frame = 0;
    int mode = 0;

    printf("Particle System Simulator\n");
    printf("Watch the particles dance!\n");
    sleep(2);

    while (1) {

        if (frame % 30 == 0) {
            if (mode == 0) {

                double x = rand() % SCREEN_WIDTH;
                double y = rand() % (SCREEN_HEIGHT / 2);
                create_explosion(&ps, x, y, 15 + rand() % 20);
            }


            if (frame % 120 == 0) {
                mode = 1 - mode;
            }
        }


        if (mode == 1 && frame % 3 == 0) {
            create_fountain(&ps, SCREEN_WIDTH / 2, SCREEN_HEIGHT - 1);
        }

        update_particles(&ps);
        render_particles(&ps);
        display_screen(&ps);

        usleep(50000);
        frame++;


        if (frame > 10000) frame = 0;
    }

    return 0;
}
