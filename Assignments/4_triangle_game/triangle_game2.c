#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "triangle_routines.h"

#define NMOVES 36
#define N_MOVE_ELEMENTS 3
#define B_ELEMENTS 15

int moves[NMOVES][N_MOVE_ELEMENTS] =
{
  {0, 1, 3},
  {3, 1, 0},
  {1, 3, 6},
  {6, 3, 1},
  {3, 6, 10},
  {10, 6, 3},
  {2, 4, 7},
  {7, 4, 2},
  {4, 7, 11},
  {11, 7, 4},
  {5, 8, 12},
  {12, 8, 5},
  {0, 2, 5},
  {5, 2, 0},
  {2, 5, 9},
  {9, 5, 2},
  {5, 9, 14},
  {14, 9, 5},
  {1, 4, 8},
  {8, 4, 1},
  {4, 8, 13},
  {13, 8, 4},
  {3, 7, 12},
  {12, 7, 3},
  {3, 4, 5},
  {5, 4, 3},
  {6, 7, 8},
  {8, 7, 6},
  {7, 8, 9},
  {9, 8, 7},
  {10, 11, 12},
  {12, 11, 10},
  {11, 12, 13},
  {13, 12, 11},
  {12, 13, 14},
  {14, 13, 12}
};
/* Return the number of pegs on the board. */
int npegs(int board[]);

/* Return 1 if the move is valid on this board, otherwise return 0. */
int valid_move(int board[], int move[]);

/* Make this move on this board. */
void make_move(int board[], int move[]);

/* Unmake this move on this board. */
void unmake_move(int board[], int move[]);

/*
 * Solve the game starting from this board.  Return 1 if the game can
 * be solved; otherwise return 0.  Do not permanently alter the board passed
 * in. Once a solution is found, print the boards making up the solution in
 * reverse order.
 */
int solve(int board[]);

int main(void)
{
  int solveable;
  int board[N_MOVE_ELEMENTS];
  triangle_input(board);
  solveable = solve(board);
  if (!solveable)
  {
    printf("This board is not solveable.\n");
    exit(0);
  }
  return 0;
}

/* Does: Return the number of pegs on the board.
 * Arguments: One-dimensional array representing board.
 * Returns: The number of pegs on the board.
 */
int npegs(int board[])
{
  int i;
  int count = 0;
  /* Loop through board. Add to count if
   * element at position is 1
   */
  for (i = 0; i < NMOVES; i++)
  {
    if (board[i] == 1)
    {
      count++;
    }
  }
  return count;
}

/* Does: Determines if a move is valid.
 * Arguments: The board as a one-dimensional array and the
 * move to be assessed.
 * Returns: Returns 1 if the move is valid on this board, otherwise
 * returns 0.
 */
int valid_move(int board[], int move[])
{
  int i;
  int j;
  int equal;
  int match_index = 0;
  int move_first;
  int move_second;
  int move_third;
  /* Loop through array of valid moves to find if move[]
   * matches one of them. If it does, check to see if that
   * move is valid given the current board.
   */
  for (i = 0; i < NMOVES; i++)
  {
    equal = 1;
    for (j = 0; j < N_MOVE_ELEMENTS; j++)
    {
      if (move[j] != moves[i][j])
      {
        equal = 0;
      }
    }
    if (equal)
    {
      match_index = i;
      break;
    }
  }

  if (equal)
  {
    move_first = moves[match_index][0];
    move_second = moves[match_index][1];
    move_third = moves[match_index][2];
    if ((board[move_first] = 1) && (board[move_second] == 1)
        && (board[move_third] == 0))
    {
      return 1;
    }
  }

  return 0;
}

/* Does: Makes a move on the board.
 * Arguments: The board as a one-dimensional array and
 * the move to be made.
 * Returns: Void.
 */
void make_move(int board[], int move[])
{
  if (valid_move(board, move))
  {
    board[move[0]] = 0;
    board[move[1]] = 0;
    board[move[2]] = 1;
  }
}

/* Does: Unmakes a move on the board.
 * Arguments: The board as a one-dimensional array and
 * the move to be made.
 * Returns: Void.
 */
void unmake_move(int board[], int move[])
{
  board[move[0]] = 1;
  board[move[1]] = 1;
  board[move[2]] = 0;
}

/* Does: Solves the game starting from this board. Once a solution is found,
 * prints the boards making up the solutions in reverse order. Does not
 * permanently alter the board passed in.
 * Arguments: The starting board.
 * Returns: Returns 1 if the game can be solved; otherwise returns 0.
 */
int solve(int board[])
{
  int i;
  int j;
  int solveable = 0;
  int n_pegs = 0;

  /* Count number of pegs */
  for (i = 0; i < B_ELEMENTS; i++)
  {
    if (board[i] == 1)
    {
      n_pegs++;
    }
  }

  /* Base Case: If only one peg left, it is solveable because
   * it is already solved.
   */
  if (n_pegs == 1 || n_pegs == 0)
  {
    return 1;
  }
  else
  {
    for (j = 0; j < NMOVES; j++)
    {
      if (valid_move(board, moves[j]))
      {
        make_move(board, moves[j]);
        solveable = solve(board);
        unmake_move(board, moves[j]);
        if (solveable)
        {
          triangle_print(board);
          return 1;
        }
      }
    }
  }
  return 0;
}

