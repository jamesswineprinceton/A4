/*--------------------------------------------------------------------*/
/* nodeFT.h                                                           */
/* Author: Christopher Moretti, James Swinehart, and Benny Wertheimer */
/*--------------------------------------------------------------------*/

#ifndef NODE_INCLUDED
#define NODE_INCLUDED

#include <stddef.h>
#include "a4def.h"
#include "path.h"


/* A Node_T is a node in a Directory Tree */
typedef struct node *Node_T;

/*
  Creates a new node in the Directory Tree, with type bIsDirectory 
  indicating if the node represents a directory or a file, path oPPath,
  parent oNParent, void pointer pvContents, and size_t ulLength. Returns 
  an int SUCCESS status and sets *poNResult to be the new node if 
  successful. Otherwise, sets *poNResult to NULL and returns status:
  * MEMORY_ERROR if memory could not be allocated to complete request
  * CONFLICTING_PATH if oNParent's path is not an ancestor of oPPath
  * NO_SUCH_PATH if oPPath is of depth 0
                 or oNParent's path is not oPPath's direct parent
                 or oNParent is NULL but oPPath is not of depth 1
  * ALREADY_IN_TREE if oNParent already has a child with this path
  * NOT_A_DIRECTORY if oNParent is a file node
*/
int Node_new(boolean bIsDirectory, Path_T oPPath, Node_T oNParent, 
             Node_T *poNResult, void *pvContents, size_t ulLength);

/*
  Destroys and frees all memory allocated for the subtree rooted at
  oNNode, i.e., deletes this node and all its descendents. Returns the
  number of nodes deleted.
*/
size_t Node_free(Node_T oNNode);

/* Returns the path object representing oNNode's absolute path. */
Path_T Node_getPath(Node_T oNNode);

/*
  Returns TRUE if oNParent has a child with path oPPath. Returns
  FALSE if it does not.

  If oNParent has such a child, stores in *pulChildID the child's
  identifier (as used in Node_getChild). If oNParent does not have
  such a child, stores in *pulChildID the identifier that such a
  child _would_ have if inserted.
*/
boolean Node_hasChild(Node_T oNParent, Path_T oPPath,
                         size_t *pulChildID);

/* Returns the number of children that oNParent has. */
size_t Node_getNumChildren(Node_T oNParent);

/*
  Returns an int SUCCESS status and sets *poNResult to be the child
  node of oNParent with identifier ulChildID, if one exists.
  Otherwise, sets *poNResult to NULL and returns status:
  * NO_SUCH_PATH if ulChildID is not a valid child for oNParent
*/
int Node_getChild(Node_T oNParent, size_t ulChildID,
                  Node_T *poNResult);

/*
  Returns a the parent node of oNNode.
  Returns NULL if oNNode is the root and thus has no parent.
*/
Node_T Node_getParent(Node_T oNNode);

/*
  Compares oNFirst and oNSecond lexicographically based on their paths.
  Returns <0, 0, or >0 if onFirst is "less than", "equal to", or
  "greater than" oNSecond, respectively.
*/
int Node_compare(Node_T oNFirst, Node_T oNSecond);

/* 
  Returns TRUE if oNNode is a directory and FALSE is oNNode is a file 
*/
boolean Node_isDirectory(Node_T oNNode);

/* Returns a void pointer to the contents stored at file oNNode */
void *Node_getContents(Node_T oNNode);

/* 
   Inserts a void pointer of pvNewContents and size_t ulLength into a 
   file oNNode. 
   Returns a void pointer of the contents previously stored in oNNode, 
   if any.
*/
void *Node_putContents(Node_T oNNode, void *pvNewContents, 
                       size_t ulLength);

/* Returns the content length of file oNNode. */
size_t Node_getContentsLength(Node_T oNNode);

/*
  Returns a string representation for oNNode, or NULL if
  there is an allocation error.

  Allocates memory for the returned string, which is then owned by
  the caller!
*/
char *Node_toString(Node_T oNNode);

#endif
