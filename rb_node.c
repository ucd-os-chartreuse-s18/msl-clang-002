/**
 * @file rb_node.c
 * @author Matthew Moltzau, Michael Hedrick
 * @date 28 Jan 2018
 * @brief Implementation file for a red-black tree for counting words.
 */

#include "rb_node.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct rb_node Node;
typedef struct rb_node Tree;

/* Static sentinel structure for root and leaves cuts the required storage in half. */
static struct rb_node RB_NULL; // members statically initialized to zero, so color is RB_BLACK

/**
 * @brief Search for a node in the tree.
 *
 * Search for the @p node in the RB tree @p tree. Only the
 * key member of @p node is significant. In this case, this
 * is
 * @code{.h}
 *  char *word;
 * @endcode
 *
 * @param tree The RB tree in which to search.
 * @param node A dummy node, containing the key to search for.
 * @return A pointer to the node with that key, or NULL.
 * @note This function assumes that keys are unique.
 */
struct rb_node *
rb_find(const struct rb_node *tree, const struct rb_node *node) {
    
    if (strcmp(node->word, tree->word) < 0) {
        if (tree->left == &RB_NULL) { // NOT FOUND
            return &RB_NULL;
        } else { // continue left
            return rb_find(tree->left, node);
        }
    } else if (strcmp(node->word, tree->word) > 0) {
        if (tree->right == &RB_NULL) { // NOT FOUND
            return &RB_NULL;
        } else { // continue right
            return rb_find(tree->right, node);
        }
    }
    return (struct rb_node *) tree;
}

/**
 * @brief Performs a left rotation.
 *
 * This function is used to restore the properties of the RB
 * tree @ tree, after rb_insert or rb_delete. It pulls the
 * right child of @p node up and makes @node its left child.
 * The original right child's left child becomes the @p node's
 * right child. This function expects that the original right
 * child of @p node is <b>not</b> RB_NULL and that the root
 * of @p tree is RB_NULL. It is the inverse of the function
 * rb_right_rotate.
 *
 * @param tree RB tree on which to perform the rotation.
 * @param node The tree node on which to perform the rotation.
 * @note If the expectations of this function are violated, it silently fails.
 */
void
rb_left_rotate(struct rb_node *tree, struct rb_node *x) {

    /*
    T1, T2 and T3 are subtrees.
                y                               x
               / \     Right Rotation          /  \
              x   T3   – – – – – – – >        T1   y
             / \       < - - - - - - -            / \
            T1  T2     Left Rotation            T2  T3
     https://www.geeksforgeeks.org/avl-tree-set-1-insertion/
     * */
    
    // parameter "tree" is not used and I don't know why it is included
    struct rb_node *y = x->right;
    y->parent = x->parent;
    x->right = y->left;
    y->left = x;
    x->parent = y;
}

/**
 * @brief Performs a right rotation.
 *
 * This function is used to restore the properties of the RB
 * tree <b>tree</b>, after rb_insert or rb_delete. It pulls the
 * left child of @p node up and makes @node its right child.
 * The original left child's right child becomes the @p node's
 * left child. This function expects that the original left
 * child of @p node is <b>not</b> RB_NULL and that the root
 * of @p tree is RB_NULL. It is the inverse of the function
 * rb_left_rotate.
 *
 * @param tree RB tree on which to perform the rotation.
 * @param node The tree node on which to perform the rotation.
 * @note If the expectations of this function are violated, it silently fails.
 */
void
rb_right_rotate(struct rb_node *tree, struct rb_node *y) {
    
    /*
    T1, T2 and T3 are subtrees.
                y                               x
               / \     Right Rotation          /  \
              x   T3   – – – – – – – >        T1   y
             / \       < - - - - - - -            / \
            T1  T2     Left Rotation            T2  T3
     https://www.geeksforgeeks.org/avl-tree-set-1-insertion/
     * */
    
    // parameter "tree" is not used and I don't know why it is included
    struct rb_node *x = y->left;
    x->parent = y->parent;
    y->left = x->right;
    x->right = y;
    y->parent = x;
}

/**
 * @brief Inserts a new node into the tree.
 *
 * This function inserts a new node @p node with a key that
 * doesn't yet exist in @p tree. Duplicates are not allowed.
 * If the same key is encountered, its count is incremented.
 * If the insert is successful, the new node is colored <b>red</b>.
 *
 * @param tree RB tree in which to insert the new node.
 * @param node
 * @return A pointer to the inserted node, or NULL, if duplicate.
 */
//  Should the item simply be linked?
//  Pros:
//      Simplicity?
//  Cons:
//      The main method can change the tree accidentally if the
//      input isn't re-initialized.
//      The allocation and deallocation would be separated by
//      the main method and the tree here. (seems bad)
//      The main method does not have access to RB_NULL, which
//      is integral for the maintenance for this tree.
//	TODO figure out details of return node
//  TODO have item count affect the count of node (0 is 1)
//  TODO allow item to add it's sub-components?
//  TODO add parent nodes
//  const struct must be cast to a non-const
//  DEBUG print node: printf("NODE %p:%s\n", node, node->word);
struct rb_node *
rb_insert(struct rb_node *node, struct rb_node *item) {
    
    /* ROOT CASE
     * node->word == NULL means node is the root
     * and that it hasn't been inserted into yet
     * (it is possible that node->word is garbage,
     * but it is up to the user to assert that's
     * not the case)
     * */
    if (node->word == NULL) {
        /* deep copy that dynamically allocates node->word */
        node->word = (char*) malloc(sizeof(char) * (strlen(item->word) + 1));
        strcpy(node->word, item->word);
        node->count  = 1;
        node->left   = &RB_NULL;
        node->right  = &RB_NULL;
        node->parent = &RB_NULL;
        return node;
    }
    
    /*
     * case 1: word < node->word
     *		the key belongs to the left of the current node
     * case 2: word > node->word
     *		the key belongs to the right of the current node
     * case 3: word == node-> word
     *		the key is equal to the key of the current node
     * */
    if (strcmp(item->word, node->word) < 0) {
        
        /*
         * If there is immediate room on the left, place a node.
         * Otherwise, traverse left.
         * */
        if (node->left == &RB_NULL) {
            
            Node *tmp = malloc(sizeof(Node));
            tmp->left  = (Node *) &RB_NULL;
            tmp->right = (Node *) &RB_NULL;
            
            /* deep copy that dynamically allocates node->word */
            tmp->word = (char*) malloc(sizeof(char) * (strlen(item->word) + 1));
            strcpy(tmp->word, item->word);
            tmp->count = 1;
            node->left = tmp;
            tmp->parent = node;
            
            /*
             * ((Node *) node)->left = item;
             * If we did this, then we'd need to assert that
             * tmp->left and tmp->right are &RB_NULL
             * I could set that here, but what if there is
             * an address connected to another node? I don't
             * want to just cut it off, and the main method
             * does not have access to RB_NULL to let us know,
             * nor do we provide a constructor to make some
             * node be RB_NULL.
             */
            
        } else {
            rb_insert(node->left, item);
        }
    } else if (strcmp(item->word, node->word) > 0) {
        
        /*
         * If there is immediate room on the right, place a node.
         * Otherwise, traverse right.
         * */
        if (node->right == &RB_NULL) {
            
            Node *tmp = malloc(sizeof(Node));
            tmp->left  = &RB_NULL;
            tmp->right = &RB_NULL;
            
            /* deep copy that dynamically allocates node->word */
            tmp->word = (char*) malloc(sizeof(char) * (strlen(item->word) + 1));
            strcpy(tmp->word, item->word);
            tmp->count = 1;
            node->right = tmp;
            tmp->parent = node;
            
        } else {
            rb_insert(node->right, item);
        }
    } else { /* found the same word */
        node->count += 1;
    }
    
    //TODO call fixup method and set color to red
    
    return node;
}

/**
 * @brief Restores RB properties after an insert.
 *
 * This function restores the RB properties of the tree @p tree,
 * after the successful insertion of the new node @node. The new
 * node starts out as <b>red</b>. There are several cases of
 * potential RB property violation and those are all handled by
 * this function.
 *
 * @param tree RB tree to restore.
 * @param node Successfully inserted node.
 */
void
rb_restore_after_insert(struct rb_node *tree, struct rb_node *node) {

}

/**
 * @brief Finds the minimum element of the tree.
 *
 * Finds the minimum element (by key) in the tree rooted
 * at @p tree. This function serves as auxiliary for
 * rb_delete.
 *
 * @param tree The tree in which to find the minimum element.
 * @return A pointer to the minimum element.
 */
// This min was taken from a BS_Tree implementation I previously coded.
// Works, but might need to show how in comments?
struct rb_node *
rb_min(struct rb_node *tree) {
    struct rb_node *tmp;
    tmp = tree;
    if (tree == &RB_NULL) {
        tmp = &RB_NULL;
    } else if (tree->left != &RB_NULL) {
        tmp = rb_min(tree->left);
    } else {
        tmp = tree;
    }
    return tmp;
}


/**
 * @brief Replaces one subtree as a child of its parent with another.
 *
 * This function replaces the subtree rooted at node @p old_root
 * with the subtree rooted at node @p new_root􏰁, node @p old_root’s
 * parent becomes node @p new_root􏰁’s parent, and @p old_root’s
 * parent ends up having 􏰁@p new_root as its appropriate child. This
 * function is auxiliary to rb_delete.
 *
 * @param tree RB tree in which to perform the transplant.
 * @param old_root The subtree root to be replaced.
 * @param new_root The subtree root that replaces old_root.
 * @note The caller is responsible for updating new_root's children.
 */
void
rb_transplant(struct rb_node *tree, struct rb_node *old_root, struct rb_node *new_root) {

}

/**
 * @brief Delete a node from a tree.
 *
 * This function deletes the node @p node from the RB tree
 * @p tree, if the node's key exists in the tree.
 *
 * @param tree RB tree from which to attempt to delete.
 * @param node Node to be deleted.
 * @return A pointer to the deleted node, or NULL, if not found.
 * @note The caller is responsible for freeing the deleted node.
 */
// TODO maintain parent info
// TODO make sure tree->word is freed
// IMPORTANT:
//  rb_delete works to delete everything EXCEPT for the head
//  node. The head cannot be deleted because the address is
//  passed by value. I could fix this if I passed a pointer
//  to the pointer, but I shouldn't change the API.
//  If I can differentiate between the other nodes and the root
//  pointer, maybe I can invoke the rb_delete method again to try
//  to change it indirectly.
struct rb_node *
rb_delete(struct rb_node *tree, struct rb_node *node) {
    
    if (tree == &RB_NULL) {
        return &RB_NULL;
    } else if (strcmp(node->word, tree->word) < 0) {
        tree->left = rb_delete(tree->left, node);
    } else if (strcmp(node->word, tree->word) > 0) {
        tree->right = rb_delete(tree->right, node);
    } else { // The node to delete is found
        
        /* DETECT ROOT */
        //if (tree->parent == &RB_NULL) rb_delete(tree, &RB_NULL);
        
        if (tree->left == &RB_NULL && tree->right == &RB_NULL) { // no children
            free(tree);
            tree = &RB_NULL;
        } else if (tree->left == &RB_NULL) { // one child
            struct rb_node *tmp = tree;
            tree = tree->right;
            free(tmp);
        } else if (tree->right == &RB_NULL) { // one child
            struct rb_node *tmp = tree;
            tree = tree->left;
            free(tmp);
        } else { //both children
            // FROM THE SOURCE I LOOKED AT, tmp->word WOULD HAVE BEEN A CONST REFERENCE
            // I DO NOT CURRENTLY KNOW IF A DEEP COPY IS NEEDED
            struct rb_node *tmp = rb_min(tree->right);
            tree->word = tmp->word;
            tree->count = tmp->count;
            tree->right = rb_delete(tree->right, tmp);
            /*
            Node* tmp = min(node->right);
            node->data = tmp->data;
            node->right = removeNode(node->right, tmp->data); */
        }
    }
    return tree;
}

/**
 * @brief Restores RB properties after a delete.
 *
 * This function restores the RB properties of the tree @p tree
 * after the deletion of node @p orphan's parent. It is run
 * conditionally.
 *
 * @param tree RB tree after a deletion of the orphan's parent.
 * @param orphan The node whose parent was deleted.
 */
void
rb_restore_after_delete(struct rb_node *tree, struct rb_node *orphan) {

}