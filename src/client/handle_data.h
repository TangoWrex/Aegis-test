#ifndef HOLESUM_H		// KLM: Generally you want this to parallel file name or an abbreviation of (i.e. HOLESUM_FUNCS_H)
#define HOLESUM_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <getopt.h>

/** 
 * @brief read_file confirms the file is valid and returns the file pointer
 * @param int argc - number of arguments
 * @param char **argv - array of arguments
 * @return file pointer
 */
FILE *read_file(int argc, char *argv[]);

/** 
 * @brief validates the file 
 * @param fp file to be validated
 * @return true if valid alse false
 */
bool is_valid_file(FILE * fp);

#endif
