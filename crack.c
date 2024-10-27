#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "md5.h"

const int PASS_LEN = 20;        // Maximum any password will be
const int HASH_LEN = 33;        // Length of MD5 hash strings

void trimNL(char *str) {
    char *nl = strchr(str, '\n');
    if (nl) {
        *nl = '\0'; 
    }
}

// tryWord function
char *tryWord(char *plaintext, char *hashFilename) {
    // Hash the plaintext
    char *hash = md5(plaintext, strlen(plaintext));
    FILE *hashFile = fopen(hashFilename, "r");

    char str[HASH_LEN];
    while (fgets(str, HASH_LEN, hashFile)) {      
        trimNL(str);

        if (strcmp(str, hash) == 0) {
            fclose(hashFile);
            return hash;
        }
    }

    fclose(hashFile);
    free(hash);
    return NULL; // No match found
}

// Main Function
int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s hash_file dict_file\n", argv[0]);
        exit(1);
    }

    // Open dictionary file in 'read'
    FILE *dictFile = fopen(argv[2], "r");
    // If can't read file
    if (!dictFile) {
        printf("Can't read file: %s\n", argv[2]);
        exit(1);
    }

    char dictWord[PASS_LEN];
    int num = 0;

    // For each dictionary word, pass it to tryWord
    while (fgets(dictWord, PASS_LEN, dictFile)) {
        // Trim newline
        trimNL(dictWord);

        // Test for a match
        char *found = tryWord(dictWord, argv[1]);
    
        // If we got a match, display the hash and the word
        if (found) {
            printf("%s %s\n", found, dictWord);
            num++;
            free(found); // Freeing the found hash after use
        }
    }
    
    // Close the dictionary file
    fclose(dictFile);
    // Display the number of hashes that were cracked
    printf("Number of hashes cracked: %d\n", num);
    
    return 0; // Successful completion
}
