/* Name: Noah Patterson
 * user: nspatter
 * Description: This is a traversal type program. After establishing a 2D array of 
 * elevations, traverse left to right (west to east) along the most efficient path.
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

void error_handler (const char *message) {
  printf("%s\n", message);
  exit(1);
}
int main(int argc, char* argv[]){
  if (argc < 2) {
    printf("Usage: East <input file> [debug file]\n");
    return 1;
  }

  FILE *input = fopen(argv[1], "r");
  if (!input) {
    printf("ERROR: failed to open '%s' for input!\n", argv[1]);
    return 1;
  }

  int width;
  int height;

  // Make sure that there are both width and height dimensions
  if (fscanf(input, "%d %d", &width, &height) != 2) {
    error_handler ("ERROR: Failed to read width and height!");
  }

  // establish a 2D array of elevations and parse info
  int array[height][width]; 
  int total_points = height * width;
  int min = INT_MAX; // this allows the actual min to be updated properly
  int max = INT_MIN; // this allows the actual max to be updated properly
  double sum = 0.0;  // this is the sum of elevations

  for (int h = 0; h < height; h++) {
    for (int w = 0; w < width; w++) {
      // check for NAN data
      if (fscanf(input, "%d", &array[h][w]) != 1) {
	error_handler("ERROR: invalid elevation data!");
      }

      sum = sum + array[h][w];

      if(array[h][w] < min) {
	min = array[h][w];
      }

      if(array[h][w] > max) {
	max = array[h][w];
      }
    }
  }

  fclose(input);

  // print the stats
  printf("Data points: %d\n", total_points);
  printf("Avg elevation: %.2f\n", sum/total_points);
  printf("Min elevation: %d\n", min);
  printf("Max elevation: %d\n", max);

  // if debug file is provided, open it
  FILE *debug = NULL;
  if (argc == 3) {
    debug = fopen(argv[2], "w");
    if (!debug) {
      printf("ERROR: failed to open '%s' for output!\n", argv[2]);
      return 1;
    }
  }

  // greedy algorithm
  int min_row = 0;  // this is the best row to progress into
  int min_cost = INT_MAX;  // this is the best elevation cost

  for (int start = 0; start < height; start++) {
    int row = start;
    int total_cost = 0;

    for (int col = 0; col < width - 1; col++) {
      int elevation = array[row][col];
      int next_col = col + 1;
      int next_row = row;
      int min_change = abs(array[row][next_col] - elevation);
      char direction = 'F';  // choosing forward to be default

      // check north
      if (row > 0) {
	int change = abs(array[row - 1][next_col] - elevation);

	if (change < min_change) {
	  min_change = change;
	  next_row = row - 1;
	  direction = 'U';
	}
	
      }
      
      // check south
      if (row < height - 1) {
	int change = abs(array[row + 1][next_col] - elevation);

	if (change < min_change || (change == min_change && direction != 'F')) {
	  min_change = change;
	  next_row = row + 1;
	  direction = 'D';
	}
      }

      if (debug) {
	fprintf(debug, " %d,%d %c,", col, row, direction);
      }

      row = next_row;
      total_cost = total_cost + min_change;
    }

    // run the inner loop one more time, but don't include direction
    int col = width - 1;
    int elevation = array[row][col];
    int next_col = col + 1;
    //    int next_row = row;
    int min_change = abs(array[row][next_col] - elevation);
    char direction = 'F';
    // check north
    if (row > 0) {
      int change = abs(array[row - 1][next_col] - elevation);
      if (change < min_change) {
	min_change = change;
	//	next_row = row - 1;
	direction = 'U';
      }
    }

    // check south
    if (row < height - 1) {
      int change = abs(array[row + 1][next_col] - elevation);
      if (change < min_change || (change == min_change && direction != 'F')) {
	min_change = change;
	//next_row = row + 1;
	direction = 'D';
      }
    }

    if (debug) {
      fprintf(debug, " %d,%d,", col, row);
    }

    if (debug) {
      fprintf(debug, " cost %d\n", total_cost);
    }

    if (total_cost < min_cost) {
      min_cost = total_cost;
      min_row = start;
    }
  }

  // print best path (by greedy)
  printf("Best: row %d, cost %d\n", min_row, min_cost);

  if (debug) {
    fclose(debug);
  }

  return 0;
}
