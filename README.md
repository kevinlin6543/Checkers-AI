# ECE 469 Artificial Intelligence

## Project 1: Checkers AI with Alpha-Beta pruning

## Design Guidelines
The AI was design using alpha beta pruning and iterative deepening search. The checker board is displayed along with all legal moves for the turn. Additionally, the depth of search is displayed with the time it takes to reach it.

## What each file does?
The board.cpp class has member functions for the majority of the design. Legal moves, printing the board, making moves and several other helper functions. The game.cpp defines the AI part with alpha beta along with reading the user inputs. The last file, ai.cpp is simply the main function that starts the program.

## Dependencies
```
g++
make
```
## Loading a game state

### Position values

```
'r' = Player 1 Regular Piece
'b' = Player 2 Regular Piece
'R' = Player 1 King Piece
'B' = Player 2 King Piece
'e' = Empty square
```
### Sample board modeled after sampleCheckers1.txt

```
e e e e
R e e e
B b R e
e e e e
B b B e
e e e e
r B b e
e e R e
r
```
### Description
The board is modeled in 8x4 pattern since each row can only have four legal spots. The last character describe whose turn it currently is.

