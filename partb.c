#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdlib.h>

/**
 * This program takes two command-line arguments: an input file and an output file.
 * It redirects the STDOUT of the process to the output file, writes a header line,
 * and then executes the program from Part A.
 * 
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 * @return int Returns 0 on success, or 1 on failure.
 */
int main(int argc, char* argv[]) {
    // Check if the correct number of arguments is provided
    if (argc != 3) {
        fprintf(stderr, "USAGE: partb FILEIN FILEOUT\n");
        return 1;
    }

    // Extract the input and output filenames from the arguments
    char* input_filename = argv[1];
    char* output_filename = argv[2];

    // Open the output file with write mode, create options, and 0770 permissions
    int fd = open(output_filename, O_WRONLY | O_CREAT | O_TRUNC, 0770);
    if (fd == -1) {
        perror("Error opening output file");
        return 1;
    }

    // Redirect STDOUT to the output file
    if (dup2(fd, STDOUT_FILENO) == -1) {
        perror("Error redirecting STDOUT");
        close(fd);
        return 1;
    }

    // Close the original file descriptor as it's no longer needed
    close(fd);

    // Print the header line to the output file and flush the buffer
    printf("Category,Count\n");
    fflush(stdout);

    // Prepare arguments for execv
    char* parta_args[] = {"./parta", input_filename, NULL};

    // Execute the program from Part A
    int eret = execv("./parta", parta_args);
    if (eret == -1) {
        perror("Error executing ./parta");
        return 1;
    }

    return 0;
}
