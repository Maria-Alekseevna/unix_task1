#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>

#define DEFAULT_BLOCK 4096

void die(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int is_all_zero(char *buf, ssize_t size) {
    for (ssize_t i = 0; i < size; i++) {
        if (buf[i] != 0)
            return 0;
    }
    return 1;
}

int main(int argc, char *argv[]) {
    int opt;
    size_t block_size = DEFAULT_BLOCK;

    while ((opt = getopt(argc, argv, "b:")) != -1) {
        switch (opt) {
            case 'b':
                block_size = atoi(optarg);
                if (block_size == 0) {
                    fprintf(stderr, "Invalid block size\n");
                    exit(EXIT_FAILURE);
                }
                break;
            default:
                fprintf(stderr, "Usage: %s [-b blocksize] [input] output\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    int input_fd = STDIN_FILENO;
    int output_fd;

    if (argc - optind == 1) {

        output_fd = open(argv[optind], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (output_fd < 0) die("open output");
    } else if (argc - optind == 2) {

        input_fd = open(argv[optind], O_RDONLY);
        if (input_fd < 0) die("open input");

        output_fd = open(argv[optind + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (output_fd < 0) die("open output");
    } else {
        fprintf(stderr, "Usage: %s [-b blocksize] [input] output\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char (*buffer) = malloc(block_size);
    if (!buffer) die("malloc");

    ssize_t bytes_read;
    off_t total_size = 0;

    while ((bytes_read = read(input_fd, buffer, block_size)) > 0) {
        if (is_all_zero(buffer, bytes_read)) {
            if (lseek(output_fd, bytes_read, SEEK_CUR) == -1)
                die("lseek");
        } else {
            ssize_t written = write(output_fd, buffer, bytes_read);
            if (written != bytes_read)
                die("write");
        }
        total_size += bytes_read;
    }

    if (bytes_read < 0)
        die("read");

    if (ftruncate(output_fd, total_size) == -1)
        die("ftruncate");

    close(input_fd);
    close(output_fd);
    free(buffer);

    return 0;
}