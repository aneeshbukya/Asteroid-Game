/**
*@file spaceshot.c 
*@author Aneesh Bukya
*
*/
#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include <curses.h>
#include "rocket.h"
#include "asteroid.h"
#include "spaceshot.h"

// Global variables
pthread_mutex_t screen_lock;
int quit =0; 
int quit_button = 0; 
int colcheck = 0; 
int won =0; 

int main( int argc, char* argv[] ) {

    srand(41);
    // prepare the Curses environment for output and input
    initscr();      // initialize the Curses window
    cbreak();       // set no waiting for Enter key
    noecho();       // set no echoing of user input
    pthread_mutex_init(&screen_lock, NULL);
    // get the dimensions of the window
    int nrows = getmaxy( stdscr );
    int ncols = getmaxx( stdscr );

    // additional preparation for doing input
    intrflush( stdscr, false ); // don't flush output on interrupt
    keypad( stdscr, true );     // enable the keypad
    // clear the screen, and "home" the cursor
    clear();
    int opt;
    int *rowval; 
    int num_asteroids = 5;
    int max_asteroids = (nrows - 2) / 2;
    if (nrows < MIN_ROWS || nrows > MAX_ROWS || ncols < MIN_COLS || ncols > MAX_COLS) {// Check if window dimensions fit within 
        endwin();
        pthread_mutex_destroy(&screen_lock);
        fprintf(stderr, "This window's dimensions are %dx%d.\n", ncols, nrows);
        fprintf(stderr, "Please use a window that is at least %dx%d and\n", MIN_COLS, MIN_ROWS);
        fprintf(stderr, "no more than %dx%d.\n", MAX_COLS, MAX_ROWS);
        fprintf(stderr, "The number of asteroids must be between 1 and %d.\n", max_asteroids);
        return EXIT_FAILURE;
    }
    while ((opt = getopt(argc, argv, "hn:")) != -1) {
        switch (opt) {
            case 'h':
                endwin();
                pthread_mutex_destroy(&screen_lock);
                fprintf(stderr, "usage: spaceshot [-h] [-n num]\n");
                return EXIT_SUCCESS;
            case 'n':
                num_asteroids = atoi(optarg);
                if (num_asteroids < 1 || num_asteroids > max_asteroids) {  // Check if teh numbe rof asteriods is within teh range
                    endwin();
                    pthread_mutex_destroy(&screen_lock);
                    fprintf(stderr, "This window's dimensions are %dx%d.\n", ncols, nrows);
                    fprintf(stderr, "The number of asteroids must be between 1 and %d.\n", max_asteroids);
                    return EXIT_FAILURE; 
                }
                break; 
        }
    }
    // Create rocket thread and rocket info instance. 
    tinfo_t rocket_info; 
    rocket_info.row = nrows - 1;
    rocket_info.col = ncols / 2;
    // Initialize rocket_info
    pthread_t rocket_thread;
    pthread_create(&rocket_thread, NULL, rocket,(void*) &rocket_info);
     // Create asteroid threads and asteroid info 
    pthread_t asteroid_threads[num_asteroids];
    tinfo_t asteroid_info[num_asteroids];
    //loops through and fills in the struct
    for(int i =0; i<num_asteroids;i++){
        asteroid_info[i].id = i; 
        asteroid_info[i].row = 2 * i + 1; 
        asteroid_info[i].col = rand() % (ncols - 1) + 1; 
        if (i%2 ==0){
            asteroid_info[i].dir = Right; 
        }else{
            asteroid_info[i].dir = Left; 
        }
     pthread_create(&asteroid_threads[i], NULL, asteroid,(void *)&asteroid_info[i]); // create a thread for each asteroid
    }
    pthread_join(rocket_thread,(void*)&rowval); // joins the rocket thread 
    for (int i = 0; i < num_asteroids; i++) {
        pthread_join(asteroid_threads[i], NULL); // joins the asteroid threads
    }
    refresh(); 
    if (won == 1){  // if the game was won 
        mvprintw(nrows/2,ncols/2,"Congratulations!  You won!");
    }else if (quit_button==1){ // if the quit button was pressed 
        mvprintw(nrows/2,ncols/2,"The game is over; you quit.");
    }else if (colcheck == 1){ // checks for collisions 
        mvprintw(nrows/2,ncols/2,"The game is over; the rocket made it to row %d.",rowval);
    }else{
        mvprintw(nrows/2,ncols/2,"end of game");
    }
    mvprintw((nrows/2)+1,ncols/2,"Press any key to exit.");// exits 
    refresh();
    while(1){ // waits for user to press any key 
        if (getch() != ERR){
            endwin();
            exit(EXIT_SUCCESS);
        }
    }
    echo();
    pthread_mutex_destroy(&screen_lock); // destroys and returns
    return EXIT_SUCCESS;
}
