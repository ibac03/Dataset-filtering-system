/** @file music_manager.c
 *  @brief A small program to analyze songs data.
 *  @author Mike Z.
 *  @author Felipe R.
 *  @author Hausi M.
 *  @author Juan G.
 *  @author Angadh S.
 *  @author Bharath Irukulapati
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h" // Include the header file for linked list operations

#define MAX_LINE_LEN 256 // Define the maximum length of a line

// Function Prototypes
void free_song_data(song_data* song); // Frees the memory allocated for a song_data struct
void deallocate_memory(node_t*); // Frees the entire linked list and its song data
void display_songs_ordered(node_t*, int, char*, FILE*); // Displays songs in a specific order
node_t* process_song_details(char*); // Processes a single line of song data to create a song node
void analyze_songs_by_artist(const char*, char*, char*, int, char*); // Filters and displays songs by artist
void analyze_songs_by_year(const char*, char*, char*, int, char*); // Filters and displays songs by year
void process_arguments_and_filter_songs(args argument); // Processes arguments and filters songs accordingly
args parse_arguments(int argc, char *argv[]); // Parses command-line arguments into a structured form
void display_songs_by_streams(node_t* list, int limit, FILE* output_file); // Displays songs ordered by stream count
void display_songs_by_spotify_playlists(node_t* list, int limit, FILE* output_file); // Displays songs ordered by Spotify playlist count
void display_songs_by_apple_playlists(node_t* list, int limit, FILE* output_file); // Displays songs ordered by Apple playlist count

// Frees the memory allocated for a song_data struct
void free_song_data(song_data* song) {
    if (song != NULL) {
        free(song->artist_name);
        free(song->track_name);
        // Free any additional dynamically allocated fields within song_data here
        free(song);
    }
}

// Frees the entire linked list and its song data
void deallocate_memory(node_t* list) {
    while (list != NULL) {
        node_t *temp = list; // Temporary pointer for the current node
        list = list->next; // Move to the next node

        // Now, free the song_data using the new function
        free_song_data(temp->song);

        // Finally, free the list node itself
        free(temp);
    }
}

// Displays songs ordered by stream count
void display_songs_by_streams(node_t* list, int limit, FILE* output_file) {
    int counter = 0;
	
	if(limit!=0){
	
    while (list != NULL && counter < limit) {
        fprintf(output_file, "%d-%d-%d,%s,%s,%ld\n",
                list->song->year, list->song->month, list->song->day, list->song->track_name, list->song->artist_name, list->song->streams);
        list = list->next; // Move to the next node
        counter++; // Increment the counter
    }
	
	}else {
		
		while (list != NULL) {
        fprintf(output_file, "%d-%d-%d,%s,%s,%ld\n",
                list->song->year, list->song->month, list->song->day, list->song->track_name, list->song->artist_name, list->song->streams);
        list = list->next; // Move to the next node
	}
	}
}

// Displays songs ordered by Spotify playlist count
void display_songs_by_spotify_playlists(node_t* list, int limit, FILE* output_file) {
    int counter = 0;
    while (list != NULL && counter < limit) {
        fprintf(output_file, "%d-%d-%d,%s,%s,%d\n",
                list->song->year, list->song->month, list->song->day, list->song->track_name, list->song->artist_name, list->song->spotify);
        list = list->next; // Move to the next node
        counter++; // Increment the counter
    }
}

// Displays songs ordered by Apple playlist count
void display_songs_by_apple_playlists(node_t* list, int limit, FILE* output_file) {
    int counter = 0;
    while (list != NULL && counter < limit) {
        fprintf(output_file, "%d-%d-%d,%s,%s,%d\n",
                list->song->year, list->song->month, list->song->day, list->song->track_name, list->song->artist_name, list->song->apple);
        list = list->next; // Move to the next node
        counter++; // Increment the counter
    }
}

// Displays songs in a specific order
void display_songs_ordered(node_t* list, int limit, char* order_by, FILE* output_file) {
    // Check the order_by criteria and call the appropriate function
    if (strcmp(order_by, "STREAMS") == 0) {
        display_songs_by_streams(list, limit, output_file);
    } else if (strcmp(order_by, "NO_SPOTIFY_PLAYLISTS") == 0) {
        display_songs_by_spotify_playlists(list, limit, output_file);
    } else {
		display_songs_by_apple_playlists(list, limit, output_file);
    }
}

// Processes a single line of song data to create a song node
node_t* process_song_details(char* line){
    // Allocate memory for a new song_data structure
    song_data* song = (song_data*)malloc(sizeof(song_data));
    if (!song) {
        perror("Failed to allocate song_data"); // Print an error message if allocation fails
        return NULL;
    }
    // Extract and duplicate the track name
    char* temp = strtok(line, ",");
    song->track_name = strdup(temp ? temp : " ");
    // Extract and duplicate the artist name
    temp = strtok(NULL, ",");
    song->artist_name = strdup(temp ? temp : " ");
    
    // Skip two fields to reach the year
    for (int i = 0; i < 2; i++) temp = strtok(NULL, ",");
    
    // Extract year, month, day, spotify count, streams, and apple playlist count
    song->year = temp ? atoi(temp) : 0;
    temp = strtok(NULL, ",");
    song->month = temp ? atoi(temp) : 0;
    temp = strtok(NULL, ",");
    song->day = temp ? atoi(temp) : 0;
    temp = strtok(NULL, ",");
    song->spotify = temp ? atoi(temp) : 0;
    temp = strtok(NULL, ",");
	char *endptr;
    song->streams = temp ? strtol(temp, &endptr, 10) : 0;
    temp = strtok(NULL, ",");
    song->apple = temp ? atoi(temp) : 0;
    // Create and return a new node with the processed song data
    return new_node(song);
}

// Filters and displays songs by artist
void analyze_songs_by_artist(const char* filename, char* order_by, char* artist_name, int limit, char* order){
    FILE *input_file, *output_file; // File pointers for reading and writing
    char *line = NULL; // Buffer for reading lines from the file
    node_t *list = NULL; // Linked list for storing processed songs
    line = (char *)malloc(MAX_LINE_LEN * sizeof(char)); // Allocate memory for the line buffer

    if (!line) {
        fprintf(stderr, "Failed to allocate memory\n"); // Print an error if memory allocation fails
        return;
    }

    input_file = fopen(filename, "r"); // Open the input file for reading
    output_file = fopen("output.csv", "w"); // Open (or create) the output file for writing
    // Write the CSV header to the output file
    fprintf(output_file, "released,track_name,artist(s)_name,streams\n");

    // Read each line from the input file until EOF
    while (fgets(line, MAX_LINE_LEN, input_file) != NULL) {
        // Process the current line to create a song node
        node_t *song_info = process_song_details(line);
        // If the artist name matches, add the song to the list
        if (strstr(song_info->song->artist_name, artist_name) != NULL) {
            // Decide the order for adding based on the 'order' argument
            if(strcmp(order, "DES") == 0){
                // Add in reverse order
                list = add_rev_order(list, song_info, order_by);
            } else {
                // Add in normal order
                list = add_inorder(list, song_info, order_by);
            }
        }
    }
    // Display the ordered list of songs
    display_songs_ordered(list, limit, order_by, output_file);
    // Deallocate memory used by the list and its contents
    deallocate_memory(list);
    free(line); // Free the line buffer
    fclose(input_file); // Close the input file
    fclose(output_file); // Close the output file
}

// Filters and displays songs by year
void analyze_songs_by_year(const char* filename, char* order_by, char* year, int limit, char* order){
    FILE *input_file, *output_file; // File pointers for reading and writing
    char *line = NULL; // Buffer for reading lines from the file
    node_t *list = NULL; // Linked list for storing processed songs
    line = (char *)malloc(MAX_LINE_LEN * sizeof(char)); // Allocate memory for the line buffer

    if (!line) {
        fprintf(stderr, "Failed to allocate memory\n"); // Print an error if memory allocation fails
        return;
    }

    input_file = fopen(filename, "r"); // Open the input file for reading
    output_file = fopen("output.csv", "w"); // Open (or create) the output file for writing

    // Decide the CSV header based on the 'order_by' criteria
    if(strcmp(order_by, "NO_SPOTIFY_PLAYLISTS")==0){
        // Header for Spotify playlists count
        fprintf(output_file, "released,track_name,artist(s)_name,in_spotify_playlists\n");
    }
    else if(strcmp(order_by, "NO_APPLE_PLAYLISTS")==0){
        // Header for Apple playlists count
        fprintf(output_file, "released,track_name,artist(s)_name,in_apple_playlists\n");
    }
    else{
        // Default header (used for Apple playlists count here but can be adjusted)
        fprintf(output_file, "released,track_name,artist(s)_name,in_apple_playlists\n");
    }

    // Read each line from the input file until EOF
    while (fgets(line, MAX_LINE_LEN, input_file) != NULL) {
        // Process the current line to create a song node
        node_t *song_info = process_song_details(line);
        // Convert the year string to an integer for comparison
        int year_released = atoi(year);
        // If the song's year matches the specified year, add it to the list
        if (song_info->song->year == year_released) {
            // Decide the order for adding based on the 'order' argument
            if(strcmp(order, "DES") == 0){
                // Add in reverse order
                list = add_rev_order(list, song_info, order_by);
            } else {
                // Add in normal order
                list = add_inorder(list, song_info, order_by);
            }
        }
    }
    // Display the ordered list of songs
    display_songs_ordered(list, limit, order_by, output_file);
    // Deallocate memory used by the list and its contents
    deallocate_memory(list);
    free(line); // Free the line buffer
    fclose(input_file); // Close the input file
    fclose(output_file); // Close the output file
}

// Parses command-line arguments into a structured form
args parse_arguments(int argc, char *argv[]) {
    args argument; // Structure to hold parsed arguments

    // Initialize with default values
    argument.data = NULL;
    argument.filter = NULL;
    argument.value = NULL;
    argument.order_by = NULL;
    argument.order = NULL;
    argument.limit = 0; // Default limit

    //Check if the minimum number of arguments is provided
    if (argc < 6) {
        fprintf(stderr, "Insufficient arguments provided.\n");
        exit(1); // Exit if not enough arguments are provided
     }

    // Parse each argument and store in the 'argument' structure
    
	char *temp;
    temp = strtok(argv[1], "=");
    argument.data = strtok(NULL, "=");

    temp = strtok(argv[2], "=");
    argument.filter = strtok(NULL, "=");

    temp = strtok(argv[3], "=");
    argument.value = strtok(NULL, "=");

    temp = strtok(argv[4], "=");
    argument.order_by = strtok(NULL, "=");

    temp = strtok(argv[5], "=");
    argument.order = strtok(NULL, "=");
	
	argument.limit = 0;
    // If a limit argument is provided, parse and store it
    if (argc == 7) {
        temp = strtok(argv[6], "=");
        argument.limit = atoi(strtok(NULL, "="));
    }

    return argument; // Return the populated 'argument' structure
}

// Processes arguments and filters songs accordingly
void process_arguments_and_filter_songs(args argument) {
    // Determine the filter type and call the appropriate analysis function
    if (strcmp(argument.filter, "YEAR") == 0) {
        // Filter and display songs by year
        analyze_songs_by_year(argument.data, argument.order_by, argument.value, argument.limit, argument.order);
    } else {
        // Filter and display songs by artist
        analyze_songs_by_artist(argument.data, argument.order_by, argument.value, argument.limit, argument.order);
    }
}

// Entry point of the program
int main(int argc, char *argv[]) {
    // Parse the command-line arguments
    args argument = parse_arguments(argc, argv);

    // Process the arguments to filter and display songs accordingly
    process_arguments_and_filter_songs(argument);

    return 0; // Indicate successful execution
}


