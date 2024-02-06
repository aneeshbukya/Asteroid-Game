///
/// @file asteroid.c
///
/// @author CSCI 243 instructors
/// @author Aneesh Bukya
///
/// Implementation of an asteroid thread.
///

#define _DEFAULT_SOURCE

#include <curses.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "support.h"
#include "spaceshot.h"
#include "asteroid.h"
///
/// This file contains declarations and code for asteroid threads.
/// Any global variables defined here are shared by all asteroids.
///


/*
** Public interface functions
*/

/// Startup function for an asteroid.
///
/// @param conf - configuration information for this thread
///
/// @pre initscr() has been called to set up ncurses environment
/// @pre srand() has been called to set a random seed
///
/// <br>
/// Notes:
/// <ul>
/// <li>
/// Do not lock the ncurses screen too long.
/// For optimum concurrency, use lots of mutual exclusion or critical sections
/// so that each section is very short.
/// </li>
/// <li>
/// The code in the thread is tricky regarding Curses state changes;
/// some threads may have changed the game state, but Curses
/// has not yet updated the global state. To remedy, insert a refresh()
/// before trying to fetch state that another thread may have changed.
/// </li>
/// </ul>

void *asteroid( void *conf ) {

    // make the configuration information pointer useful
    tinfo_t *me = (tinfo_t *) conf;
    int xmax = getmaxx(stdscr)-1;  // stores max x value 
    int xval; // stores current x and y values 
    int yval; 
    while(quit!=1){ // while we dont have to quit 
        unsigned int time = rand() % (MAX_DELAY - 1) + 1; // time delay as per instructions 
        mysleep(time);  // sleeps for time 
        xval = me->col;
        yval = me->row; 
        switch(me->dir){
            case Right:// right case id +ve 
                me->col++;
                if (me->col >= xmax) { // hits border and turns around 
                me->dir = Left; 
                me->col = xmax;
                continue; 
                }
                break; 
             case Left:// left case id -ve 
                me->col--;
                if (me->col<=0){ //hits border and turns right 
                    me->dir = Right; 
                    me->col =0; 
                }
                break; 
        }
        pthread_mutex_lock(&screen_lock); // locks mutex 
        refresh();
        chtype c = mvwinch(stdscr, me->row, me->col); // stores chtype (current item)
        char ch = (char)(c & A_CHARTEXT); 
        if (ch == 'A'){
            quit = 1;
            colcheck = 1; 
            pthread_mutex_unlock(&screen_lock);
            collision(yval, xval);
        }else{
            mvaddch(yval, xval,' ');
            refresh();
            mvaddch(me->row, me->col,'*');
            pthread_mutex_unlock(&screen_lock); // unlocks 
        }
        refresh(); 
    }
    pthread_exit(NULL); // exits with nothing to return 
}
