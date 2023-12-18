# Asteroids

Our simple Asteroids game pits a lone rocket ship against a swarm of asteroids. The goal of the program is to move the rocket from the bottom of the terminal window to the top without having it collide with any of the asteroids that are constantly moving across the upper part of the window. Asteroids are represented with the ASCII '*' character.

Each asteroid and the rocket are simulated using individual POSIX threads (Pthreads). Asteroids move horizontally across the window at specific positions; the rocket begins at the bottom of the window and moves in response to keyboard input from the user playing the game. The rocket is represented with the ASCII 'A' character.

Should the rocket and an asteroid attempt to occupy the same character position on the screen at the same time, a collision occurs. When this happens, the rocket explodes in a fiery display. (Fortunately, as the rocket is being controlled remotely via keyboard input, no lives will be lost should this happen.) A support function has been supplied to provide the necessary special effects for this.
