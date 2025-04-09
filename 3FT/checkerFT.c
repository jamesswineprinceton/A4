/*--------------------------------------------------------------------*/
/* checkerFT.c                                                        */
/* Author: James Swinehart and Benny Wertheimer                       */
/*--------------------------------------------------------------------*/

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "checkerFT.h"
#include "dynarray.h"
#include "path.h"

/* see checkerFT.h for specification */
boolean CheckerFT_Node_isValid(Node_T oNNode) {
   Node_T oNParent;
   Path_T oPNPath;
   Path_T oPPPath;

   /* Sample check: a NULL pointer is not a valid node */
   if(oNNode == NULL) {
      fprintf(stderr, "A node is a NULL pointer\n");
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
*/
static boolean CheckerFT_treeCheck(Node_T oNNode) {
   size_t ulIndex;

   if(oNNode!= NULL) {
      
      /* Sample check on each node: node must be valid */
      /* If not, pass that failure back up immediately */
      if(!CheckerFT_Node_isValid(oNNode))
         return FALSE;
      
      /* Recur on every child of oNNode */
      for(ulIndex = 0; ulIndex < Node_getNumChildren(oNNode); ulIndex++)
      {
         Node_T oNChild = NULL;
         int iStatus = Node_getChild(oNNode, ulIndex, &oNChild);

         if(iStatus != SUCCESS) {
            fprintf(stderr, 
         "getNumChildren claims more children than getChild returns\n");
            return FALSE;
         }

         /* Check: Children must be in lexographic path name order. 
            Paths can not have duplicate names. */
         if (ulIndex > 0) {
            Node_T oNPrevChild = NULL;
            Node_getChild(oNNode, ulIndex - 1, &oNPrevChild);

         if(Path_comparePath(Node_getPath(oNChild), 
                             Node_getPath(oNPrevChild)) < 0) {
            fprintf(stderr, 
                       "Paths are not arranged in lexographic order\n");
            return FALSE;
         }

         if(Path_comparePath(Node_getPath(oNChild), 
                             Node_getPath(oNPrevChild)) == 0) {
            fprintf(stderr, "Tree has a duplicate path\n");
            return FALSE;
         }
         }
         
         /* if recurring down one subtree results in a failed check
            farther down, passes the failure back up immediately */
         if(!CheckerFT_treeCheck(oNChild)) return FALSE;
      }
   } 
   return TRUE;
}

/* Performs a pre-order traversal of the tree rooted at oNNode. Counts
   each node within DT to ensure it can be matched with the DT's count
   later. Returns the number of nodes within DT */
static size_t CheckerFT_countCheck(Node_T oNNode) {
   size_t ulRealCount = 0;
   size_t ulIndex;

   if(oNNode != NULL) {
       /* Count oNNode */
       ulRealCount = 1;
       
       /* Recur on every child of oNNode */
       for(ulIndex = 0; ulIndex < Node_getNumChildren(oNNode); 
           ulIndex++) {
         Node_T oNChild = NULL;
         int iStatus = Node_getChild(oNNode, ulIndex, &oNChild);

         if(iStatus != SUCCESS) {
            fprintf(stderr, 
         "getNumChildren claims more children than getChild returns\n");
            return 0;
           }

           /* Sum all children found from recurring */
           ulRealCount += CheckerFT_countCheck(oNChild);
       }
   }
   return ulRealCount;
}

/* see checkerFT.h for specification */
boolean CheckerFT_isValid(boolean bIsInitialized, Node_T oNRoot,
                          size_t ulCount) {
   size_t ulRealCount;

   /* Sample check on a top-level data structure invariant:
      if the DT is not initialized, its count should be 0. */
   if(!bIsInitialized) {
      if(ulCount != 0) {
         fprintf(stderr, "Not initialized, but count is not 0\n");
         return FALSE;
      }
   }

   /* Check: if the DT is not initialized, its root should be NULL. */
   if(!bIsInitialized) {
      if(oNRoot != NULL) {
         fprintf(stderr, "Not initialized, but root is not NULL\n");
         return FALSE;
      }
   }

   /* Check: if the DT's count is 0, its root should be NULL. */
   if(ulCount == 0) {
      if(oNRoot != NULL) {
         fprintf(stderr, "Count is 0, but root is not NULL\n");
         return FALSE;
      }
   }

   /* Check: if the DT's root is NULL, its count should be 0. */
   if(oNRoot == NULL) {
      if(ulCount != 0) {
         fprintf(stderr, "Root is NULL, but count is not 0\n");
         return FALSE;
      }
   }

   /* Check: The DT's count should match its number of nodes. */
   ulRealCount = CheckerFT_countCheck(oNRoot);
   if (ulRealCount != ulCount) {
      fprintf(stderr, 
         "DT count is %lu, but number of nodes in DT is %lu\n",
      ulCount, ulRealCount);
      return FALSE;
   }
   
   return CheckerFT_treeCheck(oNRoot);
}