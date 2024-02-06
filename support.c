///
/// @file support.c
///
/// @author CSCI 243 instructors
/// @author Aneesh Bukya
///
/// Support functions for the program
///

#define _DEFAULT_SOURCE

#include <curses.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "spaceshot.h"
#include "asteroid.h"

///
/// This file contains useful support functions for use by the
/// threads in your program.
///
/// If you add or modify parts of this file, be sure to submit it
/// along with your solution.
///

/*
** Variables and types used by all threads.
*/


/*
** Functions used by all threads
*/

/// report a collision
///
/// @param row - row where the collision occurred
/// @param col - column where the collision occurred
///
/// @pre the Curses environment must have been set up already
/// @pre the calling routine must not already hold the screen lock

void collision( int row, int col ) {
    int nrows = getmaxy( stdscr );
    int ncols = getmaxx( stdscr );

    // if we are too close to the left or right edge of the screen,
    // we will "offset" the starburst so that the whole thing is
    // printed.

    if( col < 3 ) {
        // too close to the left edge
        col = 3;
    } else if( col > (ncols-3) ) {
        // too close to the right edge
        col = ncols - 3;
    }

    // similarly, check the top and bottom of the screen
    if( row < 2 ) {
        row = 2;
    } else if( row > (nrows-3) ) {
        row = nrows - 3;
    }

    pthread_mutex_lock( &screen_lock );
    mvprintw( row - 2, col - 2, "\\ | /" );
    mvprintw( row - 1, col - 1, "\\|/" );
    mvprintw( row,     col - 3, "---@---" );
    mvprintw( row + 1, col - 1, "/|\\" );
    mvprintw( row + 2, col - 2, "/ | \\" );
    refresh();
    pthread_mutex_unlock( &screen_lock );
}

/// mysleep() takes a time value in milleseconds and sleeps for that
/// length of time
///
/// We use nanosleep() for this, because sleep() requires an integer
/// time in seconds, and usleep() is not only deprecated, but also
/// requires an integer time in microseconds but cannot sleep longer
/// than one second.
///
/// @param ms - time to sleep, in milliseconds
///
/// @pre this thread should not hold any mutex locks while it is sleeping!

void mysleep( unsigned long ms ) {
    struct timespec tspec;

    tspec.tv_sec = ms / 1000;
    tspec.tv_nsec = (ms % 1000) * 1000000;

    nanosleep( &tspec, NULL );
}
