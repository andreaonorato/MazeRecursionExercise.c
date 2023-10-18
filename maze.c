#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

#define EMPTY ' '
#define START '@'
#define STOP  '#'
#define PATH  '$'

const int xOff[4] = {0, 1, 0, -1}; // Offset for moving in x-direction
const int yOff[4] = {-1, 0, 1, 0}; // Offset for moving in y-direction

/* Function prototypes */
void displayMaze(char **maze, int numRows);
int findShortestPath(char **mazeCurr, int currentStep, char **mazeBest, int bestStep, int numRows, int numCols, int row, int col);
FILE *openFile(char *filename, char *mode);
void *allocateMemory(int size);
char *copyString(char *source);

int main(int argc, char *argv[]) {
  int numRows = -1, numCols = -1, i, j, startingRow, startingCol, step;
  char **mazeCurr, **mazeBest;
  char line[MAX];
  FILE *file;

  if (argc < 2) {
    printf("Error: Missing parameter.\n");
    printf("Usage: %s <maze_file>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  // Open the maze file for reading
  file = openFile(argv[1], "r");
  fgets(line, MAX, file);
  sscanf(line, "%d %d", &numRows, &numCols);
  mazeCurr = (char **)allocateMemory(numRows * sizeof(char *));
  mazeBest = (char **)allocateMemory(numRows * sizeof(char *));

  // Read and initialize the maze
  for (i = 0; i < numRows; i++) {
    fgets(line, MAX, file);
    mazeCurr[i] = copyString(line);
    mazeBest[i] = copyString(line);
    for (j = 0; j < numCols; j++) {
      if (mazeCurr[i][j] == START) {
        mazeCurr[i][j] = EMPTY;
        startingRow = i;
        startingCol = j;
      }
    }
  }

  if (startingRow < 0 || startingCol < 0) {
    printf("Error: No starting position found!\n");
    exit(EXIT_FAILURE);
  }

  // Find the shortest path
  step = findShortestPath(mazeCurr, 0, mazeBest, numRows * numCols, numRows, numCols, startingRow, startingCol);

  // Display the solution if found
  if (step > 0) {
    printf("Solution:\n");
    mazeBest[startingRow][startingCol] = START;
    displayMaze(mazeBest, numRows);
  } else {
    printf("No solution found!\n");
  }

  // Clean up allocated memory
  for (i = 0; i < numRows; i++) {
    free(mazeCurr[i]);
    free(mazeBest[i]);
  }
  free(mazeCurr);
  free(mazeBest);
  return EXIT_SUCCESS;
}

// Recursive function to find the shortest path
int findShortestPath(char **mazeCurr, int currentStep, char **mazeBest, int bestStep, int numRows, int numCols, int row, int col) {
  int k, r, c;

  // Avoid further recursion if a solution is already found
  if (currentStep >= bestStep) {
    return bestStep;
  }

  // Check if the current position is the exit
  if (mazeCurr[row][col] == STOP) {
    if (currentStep < bestStep) {
      bestStep = currentStep;
      // Copy the best solution to mazeBest
      for (r = 0; r < numRows; r++) {
        for (c = 0; c < numCols; c++) {
          mazeBest[r][c] = mazeCurr[r][c];
        }
      }
    }
    return bestStep;
  }

  // Check if the current position is not empty
  if (mazeCurr[row][col] != EMPTY) {
    return bestStep;
  }

  // Mark the current position as visited
  mazeCurr[row][col] = PATH;

  // Explore four possible directions
  for (k = 0; k < 4; k++) {
    r = row + xOff[k];
    c = col + yOff[k];
    if (r >= 0 && r < numRows && c >= 0 && c < numCols) {
      bestStep = findShortestPath(mazeCurr, currentStep + 1, mazeBest, bestStep, numRows, numCols, r, c);
    }
  }

  // Restore the current position to empty
  mazeCurr[row][col] = EMPTY;

  return bestStep;
}

// Function to display the maze
void displayMaze(char **maze, int numRows) {
  int i;

  for (i = 0; i < numRows; i++) {
    printf("%s", maze[i]);
  }
}

// Utility function to open a file
FILE *openFile(char *filename, char *mode) {
  FILE *file = fopen(filename, mode);
  if (file == NULL) {
    printf("Error opening file %s.\n", filename);
    exit(EXIT_FAILURE);
  }
  return file;
}

// Utility function to allocate memory with error handling
void *allocateMemory(int size) {
  void *ptr = malloc(size);
  if (ptr == NULL) {
    printf("Memory allocation error.\n");
    exit(EXIT_FAILURE);
  }
  return ptr;
}

// Utility function to duplicate a string with error handling
char *copyString(char *source) {
  char *destination = strdup(source);
  if (destination == NULL) {
    printf("Memory allocation error.\n");
    exit(EXIT_FAILURE);
  }
  return destination;
}
