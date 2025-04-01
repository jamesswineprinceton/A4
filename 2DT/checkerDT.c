/*--------------------------------------------------------------------*/
/* checkerDT.c                                                        */
/* Author:                                                            */
/*--------------------------------------------------------------------*/

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "checkerDT.h"
#include "dynarray.h"
#include "path.h"

/* see checkerDT.h for specification */
boolean CheckerDT_Node_isValid(Node_T oNNode) {
   Node_T oNParent;
   Path_T oPNPath;
   Path_T oPPPath;

   /* Sample check: a NULL pointer is not a valid node */
   if(oNNode == NULL) {
      fprintf(stderr, "A node is a NULL pointer\n");
      return FALSE;
   }

   if(Node_getPath(oNNode) == NULL) {
      fprintf(stderr, "Node has no path\n");
      return FALSE;
   }

   /* Sample check: parent's path must be the longest possible
      proper prefix of the node's path */
   oNParent = Node_getParent(oNNode);
   if(oNParent != NULL) {
      oPNPath = Node_getPath(oNNode);
      oPPPath = Node_getPath(oNParent);

      if(Path_getSharedPrefixDepth(oPNPath, oPPPath) !=
         Path_getDepth(oPNPath) - 1) {
         fprintf(stderr, "P-C nodes don't have P-C paths: (%s) (%s)\n",
                 Path_getPathname(oPPPath), Path_getPathname(oPNPath));
         return FALSE;
      }
   }

   return TRUE;
}

/*
   Performs a pre-order traversal of the tree rooted at oNNode.
   Returns FALSE if a broken invariant is found and
   returns TRUE otherwise.

   You may want to change this function's return type or
   parameter list to facilitate constructing your checks.
   If you do, you should update this function comment.
*/
static boolean CheckerDT_treeCheck(Node_T oNNode, DynArray_T oDPathsSeen) {
   size_t ulIndex;
   Path_T oPPath;

   if(oNNode!= NULL && oDPathsSeen != NULL) {

      /* Sample check on each node: node must be valid */
      /* If not, pass that failure back up immediately */
      if(!CheckerDT_Node_isValid(oNNode))
         return FALSE;

      oPPath = Node_getPath(oNNode);

      for (ulIndex = 0; ulIndex < DynArray_getLength(oDPathsSeen) &&
                        DynArray_get(oDPathsSeen, ulIndex) != NULL;
                        ulIndex++) {

         /* Sample check: no two nodes can have the same path */
         if (Path_comparePath(oPPath,
                             DynArray_get(oDPathsSeen, ulIndex)) == 0) {
            fprintf(stderr, "Duplicate path found: %s\n",
                    Path_getPathname(oPPath));
            return FALSE;
         }
      }
      DynArray_add(oDPathsSeen, oPPath);
      
      /* Recur on every child of oNNode */
      for(ulIndex = 0; ulIndex < Node_getNumChildren(oNNode); ulIndex++)
      {
         Node_T oNChild = NULL;
         int iStatus = Node_getChild(oNNode, ulIndex, &oNChild);

         if(iStatus != SUCCESS) {
            fprintf(stderr, "getNumChildren claims more children than getChild returns\n");
            return FALSE;
         }

         /* if recurring down one subtree results in a failed check
            farther down, passes the failure back up immediately */
         if(!CheckerDT_treeCheck(oNChild, oDPathsSeen))
            return FALSE;
      }
   }

      return TRUE;
}


/* see checkerDT.h for specification */
boolean CheckerDT_isValid(boolean bIsInitialized, Node_T oNRoot,
                          size_t ulCount) {
   DynArray_T oDPathsSeen;

   /* Sample check on a top-level data structure invariant:
      if the DT is not initialized, its count should be 0. */
   if(!bIsInitialized) {
      if(ulCount != 0) {
         fprintf(stderr, "Not initialized, but count is not 0\n");
         return FALSE;
      }
   }

   if(!bIsInitialized) {
      if(oNRoot != NULL) {
         fprintf(stderr, "Not initialized, but root is not NULL\n");
         return FALSE;
      }
   }

   if(ulCount == 0) {
      if(oNRoot != 0) {
         fprintf(stderr, "Count is 0, but root is not NULL\n");
         return FALSE;
      }
   }

   if(oNRoot == NULL) {
      if(ulCount != 0) {
         fprintf(stderr, "Root is NULL, but count is not 0\n");
         return FALSE;
      }
   }

   oDPathsSeen = DynArray_new(ulCount);

   /* Now checks invariants recursively at each node from the root. */
   return CheckerDT_treeCheck(oNRoot, oDPathsSeen);
}