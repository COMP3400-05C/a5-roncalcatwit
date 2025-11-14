#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

/**
 * Reads a file in chunks of 8 bytes and counts the number of characters
 * in each of the following categories: uppercase letters, lowercase letters,
 * numbers, spaces, and others.
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments. The first argument is the program name,
 *             and the second argument is the name of the file to read.
 * @return Returns 0 on success, 1 if no file is provided, or 2 if the file cannot be opened.
 */
int main(int argc, char* argv[]) {
    // Declare the buffer to use. Do NOT change!
    enum { BUFSIZE = 8 };
    char buffer[BUFSIZE];

    // Check if the correct number of arguments is provided
    if (argc != 2) {
        fprintf(stderr, "USAGE: %s FILEIN\n", argv[0]);
        return 1;
    }

    // Open the file using the open system call
    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "ERROR: %s not found\n", argv[1]);
        return 2;
    }

    // Initialize counters for each category
    int upper_count = 0;
    int lower_count = 0;
    int number_count = 0;
    int space_count = 0;
    int other_count = 0;

    ssize_t bytes_read;
    // Read the file in chunks of BUFSIZE
    while ((bytes_read = read(fd, buffer, BUFSIZE)) > 0) {
        for (ssize_t i = 0; i < bytes_read; i++) {
            if (isupper(buffer[i])) {
                upper_count++;
            } else if (islower(buffer[i])) {
                lower_count++;
            } else if (isdigit(buffer[i])) {
                number_count++;
            } else if (isspace(buffer[i])) {
                space_count++;
            } else {
                other_count++;
            }
        }
    }

    // Close the file descriptor
    close(fd);

    // Print the results
    printf("Upper,%d\n", upper_count);
    printf("Lower,%d\n", lower_count);
    printf("Number,%d\n", number_count);
    printf("Space,%d\n", space_count);
    printf("Other,%d\n", other_count);

    return 0;
}
