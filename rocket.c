/// @file rocket.c
/// @author CSCI 243 Instructors
/// @author Aneesh Bukya
///
/// Implementation of a rocket thread.


#include <curses.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "support.h"
#include "spaceshot.h"
#include "rocket.h"

///
/// This file contains declarations and code for the rocket thread.
///

/*
** Variables and types used by the rocket thread.
*/

/*
** Public interface functions
*/

/// Startup function for a rocket
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

void *rocket( void *conf ) {

    // make the configuration information pointer useful
    tinfo_t *me = (tinfo_t *) conf;
    int xmax = getmaxx(stdscr) - 1;  //stores max x value 
    int ymax = getmaxy(stdscr) - 1; // stores max y value 
    int xval; // stores the current x value 
    int yval;  // stores the current y value 
    int ch;  // stores the char pressed by user 
    while (quit!=1) { // until we dont have to quit 
        xval = me->col;  // assigns row and col 
        yval = me->row; 
        ch = getch();
        switch (ch) {
            case KEY_LEFT: // left key pressed 
                me->col--;
                if (me->col<=0){
                    me->col= 0; 
                }
                break;
            case KEY_RIGHT: // right key pressed 
                me->col++; 
                if(me->col>=xmax){
                    me->col = xmax; 
                }
                break;
            case KEY_UP: // up key pressed 
                if (me->row == 1){ // should break out and print that game won 
                    me->row--;
                    quit = 1; 
                    won = 1; 
                    continue;
                }else if (me->row-1 != 0){
                    me->row--; 
                }
                break;
            case KEY_DOWN: // down key pressed 
                me->row++;
                if(me->row>=ymax){
                    me->row = ymax; 
                }
                break;
            case 'c':// c button pressed
            case 'C':
                me->col = xmax / 2;
                break;
            case 'b':
            case 'B':// b button pressed 
                me->row = ymax - 1;
                break; 
            case 'q':
            case 'Q':// q button pressed 
                if (colcheck ==0){
                quit = 1;
                quit_button = 1; 
                }
                break;
            default:
                break;
        }
     pthread_mutex_lock(&screen_lock); // locks mutex 
     refresh(); 
     chtype c = mvwinch(stdscr, me->row, me->col); // stores current char 
     char ch = (char)(c & A_CHARTEXT); // isolates the chtype to char for comaprsion 
        if (ch == '*') { // if collision 
            quit = 1;
            colcheck = 1; 
            pthread_mutex_unlock(&screen_lock);
            collision(yval,xval);
        }else{ // moveto next location 
            mvaddch(yval,xval,' ');
            refresh();
            mvaddch(me->row, me->col,'A');
            pthread_mutex_unlock(&screen_lock);
        }
        refresh(); 
    }
    clear();
    refresh(); 
    size_t rowval = me->row ;
    pthread_exit((void *) rowval); 
    // returns the highest row value hit 
    return NULL;
}

