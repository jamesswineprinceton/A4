/*--------------------------------------------------------------------*/
/* bdt_client.c                                                       */
/* Author: Christopher Moretti                                        */
/*--------------------------------------------------------------------*/

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bdt.h"

/* Tests the BDT implementation with an assortment of checks.
   Prints the status of the data structure along the way to stderr.
   Returns 0. */
int main(void) {
  char* temp;

  /* Before the data structure is initialized:
     * insert, rm, and destroy should each return INITIALIZATION_ERROR
     * contains should return FALSE
     * toString should return NULL
  */
  assert(BDT_insert("1root/2child/3grandchild") == INITIALIZATION_ERROR);
  assert(BDT_contains("1root/2child/3grandchild") == FALSE);
  assert(BDT_rm("1root/2child/3grandchild") == INITIALIZATION_ERROR);
  assert((temp = BDT_toString()) == NULL);
  assert(BDT_destroy() == INITIALIZATION_ERROR);

  /* After initialization, the data structure is empty, so
     contains should still return FALSE for any non-NULL string,
     and toString should return the empty string.
  */
  assert(BDT_init() == SUCCESS);
  assert(BDT_contains("") == FALSE);
  assert(BDT_contains("1root") == FALSE);
  assert((temp = BDT_toString()) != NULL);
  assert(!strcmp(temp,""));
  free(temp);

  /* A valid path must not:
     * be the empty string
     * start with a '/'
     * end with a '/'
     * have consecutive '/' delimiters.
  */
  assert(BDT_insert("") == BAD_PATH);
  assert(BDT_insert("/1root/2child") == BAD_PATH);
  assert(BDT_insert("1root/2child/") == BAD_PATH);
  assert(BDT_insert("1root//2child") == BAD_PATH);

  /* After insertion, the data structure should contain every prefix
     of the inserted path, toString should return a string with these
     prefixes, trying to insert it again should return
     ALREADY_IN_TREE, and trying to insert some other root should
     return CONFLICTING_PATH. */
  assert(BDT_insert("1root") == SUCCESS);
  assert(BDT_insert("1root/2child/3grandchild") == SUCCESS);
  assert(BDT_contains("1root") == TRUE);
  assert(BDT_contains("1root/2child") == TRUE);
  assert(BDT_contains("1root/2child/3grandchild") == TRUE);
  assert(BDT_contains("anotherRoot") == FALSE);
  assert(BDT_insert("anotherRoot") == CONFLICTING_PATH);
  assert(BDT_contains("anotherRoot") == FALSE);
  assert(BDT_contains("1root/2second") == FALSE);
  assert(BDT_insert("1root/2child/3grandchild") == ALREADY_IN_TREE);
  assert(BDT_insert("anotherRoot/2nope/3noteven") == CONFLICTING_PATH);
}
