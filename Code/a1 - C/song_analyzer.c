/** @file song_analyzer.c
 *  @brief A pipes & filters program that uses conditionals, loops, and string processing tools in C to process song
 *  data and printing it in a different format.
 *  @author Felipe R.
 *  @author Hausi M.
 *  @author Angadh S.
 *  @author Juan G.
 *  @author Bharath Irukulapati.
 *
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * @brief The maximum line length.
 *
 */
#define MAX_LINE_LEN 132

/**
 * Function: main
 * --------------
 * @brief The main function and entry point of the program.
 *
 * @param argc The number of arguments passed to the program.
 * @param argv The list of arguments passed to the program.
 * @return int 0: No errors; 1: Errors produced.
 *
 */
typedef struct {
    char artist[256];
    char song[256];
} Song;

void processArguments(int argc, char *argv[], char **questionNumber, char **dataName) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s --question=<number> --data=<name>\n", argv[0]);
        exit(1);
    }
    *questionNumber = strtok(argv[1], "=");
    *questionNumber = strtok(NULL, "");
    *dataName = strtok(argv[2], "=");
    *dataName = strtok(NULL, "");
}

void processFile(const char *filename, FILE *outputFile, const char *questionNumber) {
    FILE *file_ptr = fopen(filename, "r");
    if (file_ptr == NULL) {
        perror("Failed to open data file");
        exit(1);
    }
    char line[256]; // Buffer for reading lines from the file
    fgets(line, sizeof(line), file_ptr); // Skip the header line
    
    while (fgets(line, sizeof(line), file_ptr) != NULL) {
		// Parsing line for song information
        char *trackName = strtok(line, ",");
        char *artistName = strtok(NULL, ",");
		
		// Skipping unused fields
        strtok(NULL, ","); 
        char *releasedYearStr = strtok(NULL, ",");
        char *inSpotifyPlaylistsStr = strtok(NULL, ",");
        strtok(NULL, ","); 
        char *key = strtok(NULL, ",");
        char *mode = strtok(NULL, "\n"); // Using \n to ensure we capture until the end of the line
		
        // Converting string fields to appropriate data types
        int releasedYear = atoi(releasedYearStr);
        int inSpotifyPlaylists = atoi(inSpotifyPlaylistsStr);

        int includeLine = 0; // Decision flag for including this song in output
        int question = atoi(questionNumber);
		
		// Filtering based on question criteria
        switch (question) {
            case 1:
                if (strcmp(artistName, "Rae Spoon") == 0 && releasedYear < 2020) {
                    includeLine = 1;
                }
                break;
            case 2:
                if (strcmp(artistName, "Tate McRae") == 0 && releasedYear >= 2020) {
                    includeLine = 1;
                }
                break;
            case 3:
                if (strcmp(artistName, "The Weeknd") == 0 && releasedYear < 2020 && strcmp(mode, "Major") == 0) {
                    includeLine = 1;
                }
                break;
            case 4:
                if (releasedYear >= 2020 && inSpotifyPlaylists > 5000 && (strcmp(key, "D") == 0 || strcmp(key, "A") == 0)) {
                    includeLine = 1;
                }
                break;
            case 5:
                if (releasedYear >= 2021 && releasedYear <= 2022 && strstr(artistName, "Drake") != NULL) {
                    includeLine = 1;
                }
                break;
            default:
                fprintf(stderr, "Unknown question number: %s\n", questionNumber);
                break;
        }
		// Writing to output file if criteria met
        if (includeLine) {
            fprintf(outputFile, "%s,%s\n", artistName, trackName);
        }
    }
    fclose(file_ptr);
}

int main(int argc, char *argv[]) {
    char *questionNumber, *dataName;

    // Processing command-line arguments

    processArguments(argc, argv, &questionNumber, &dataName);

    // Opening output file
    FILE *fp = fopen("output.csv", "w");
    if (fp == NULL) {
        perror("Failed to open file for writing");
        return 1;
    }
    fprintf(fp, "Artist(s),Song\n");  // Writing CSV header

    // Processing the data file
    processFile(dataName, fp, questionNumber);

    fclose(fp);// Closing output file
    return 0;
}
