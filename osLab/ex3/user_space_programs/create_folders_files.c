#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

int main() {
    // Base path
    const char *base_path = "/mnt";

    // Folder and file structure
    const char *folders[] = {"folder1", "folder2", "folder3"};
    const char *files[][2] = {
        {"file1.txt", "file2.txt"},
        {"file3.txt", "file4.txt"},
        {"file5.txt", "file6.txt"}
    };

    // Check if /mnt exists
    struct stat st;
    if (stat(base_path, &st) != 0 || !S_ISDIR(st.st_mode)) {
        fprintf(stderr, "Error: %s does not exist or is not a directory.\n", base_path);
        return 1;
    }

    // Iterate through folders and files
    for (size_t i = 0; i < sizeof(folders) / sizeof(folders[0]); i++) {
        char folder_path[256];
        snprintf(folder_path, sizeof(folder_path), "%s/%s", base_path, folders[i]);

        // Create folder
        if (mkdir(folder_path, 0755) != 0 && errno != EEXIST) {
            fprintf(stderr, "Error creating folder %s: %s\n", folder_path, strerror(errno));
            continue;
        }

        // Create files in the folder
        for (size_t j = 0; j < 2; j++) {
            char file_path[256];
            snprintf(file_path, sizeof(file_path), "%s/%s", folder_path, files[i][j]);

            FILE *fp = fopen(file_path, "w");
            if (fp == NULL) {
                fprintf(stderr, "Error creating file %s: %s\n", file_path, strerror(errno));
                continue;
            }

            fprintf(fp, "This is %s inside %s\n", files[i][j], folders[i]);
            fclose(fp);
            printf("Created: %s\n", file_path);
        }
    }

    printf("All folders and files created successfully!\n");
    return 0;
}

