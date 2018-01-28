#include "test_suite.h"
#include "rb_node.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_WORD 32

void writeInorder(struct rb_node *tree, FILE *out);

int main(int argc, char *argv[]) {
    
    if (argc != 2) {
        puts("usage: hwk2 input_file");
        exit(1);
    }
    
    char filename[80];
    strcpy(filename, "./");
    strcat(filename, argv[1]);
    
    // OPENING FILE
    char buff[MAX_WORD];
    FILE *in = fopen(filename, "r");
    if (in == NULL) {
        puts(filename);
        puts("\nThere was an error opening the file. Exiting now.");
        exit(1);
    }
    
    typedef struct rb_node Tree, Node;
    Tree tree  = {NULL}; //Tree* tree = (Tree *) malloc(sizeof(Tree));
    Node input = {NULL}; //Node* input = (Node *) malloc(sizeof(Node));
    
    /*
     * Right now, input simply points to buff, which
     * is static. This may need to change if input->word
     * must be freed. Otherwise, it is fine as it is
     * since a deep copy on the buffer will be performed.
     * I currently don't see a need to allocate something
     * that will only last for a single loop iteration.
     */
    while (fscanf(in, "%s", buff) != EOF) {
        input.word = (char *) malloc(sizeof(char) * strlen(buff) + 1);
        strcpy(input.word, buff); // would need to free this
        //input.word = buff;
        rb_insert(&tree, &input);
    }
    
    // Test finding the min
    //struct rb_node *min = rb_min(&tree);
    //printf("min : %s\n", min->word);
    
    // Inserting into a tree automatically sorts, so now we can print.
    FILE *out = fopen("./program_output.txt", "w");
    writeInorder(&tree, out);
    
    // Cleanup
    fclose(in);
    fclose(out);
    //destroy(tree);
    puts("The program has finished executing.");
    exit(0);
    //*/
}

/**
 * @brief Writes a tree to a file. Also works for singular
 * nodes.
 *
 * Prints @p so that the contents are alphabetically sorted
 * and the count of each word is displayed alongside the
 * word.
 *
 * @param tree The RB tree to print.
 * @param file An output file.
 */
void writeInorder(struct rb_node *node, FILE *out) {
    if (node->word == NULL) return;
    writeInorder(node->left, out);
    fprintf(out, "%s: %d\n", node->word, node->count);
    writeInorder(node->right, out);
}