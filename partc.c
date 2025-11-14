#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

/**
 * This program uses a pipe to connect
 * a parent process (executing parta) and a child process (executing sort).
 * The parent process writes data to the pipe, and the child process reads
 * from the pipe and sorts the data.
 * 
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments. The first argument should be
 *             the name of the input file.
 * @return int Returns 0 on success, or 1 if there is an error.
 */
int main(int argc, char* argv[]) {
    // Check for command-line arguments
    if (argc != 2) {
        fprintf(stderr, "USAGE: partc FILEIN\n");
        return 1;
    }

    // Create the pipe
    int pipe_fds[2];
    if (pipe(pipe_fds) == -1) {
        perror("pipe");
        return 1;
    }

    // Create the child process
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    }

    if (pid > 0) {
        // Parent process
        close(pipe_fds[0]); // Close the read end of the pipe

        // Redirect STDOUT to the pipe's write end
        if (dup2(pipe_fds[1], STDOUT_FILENO) == -1) {
            perror("dup2");
            return 1;
        }
        close(pipe_fds[1]); // Close the original write end

        // Execute parta with the name of the input file
        execlp("./parta", "./parta", argv[1], NULL);
        perror("execlp"); // If execlp fails
        return 1;
    } else {
        // Child process
        close(pipe_fds[1]); // Close the write end of the pipe

        // Redirect STDIN to the pipe's read end
        if (dup2(pipe_fds[0], STDIN_FILENO) == -1) {
            perror("dup2");
            return 1;
        }
        close(pipe_fds[0]); // Close the original read end

        // Execute sort with the specified arguments
        execlp("sort", "sort", "-t,", "-k2", "-n", NULL);
        perror("execlp"); // If execlp fails
        return 1;
    }

    return 0;
}
