#include "avl_tree.h"

Item* BinarySearch(Item products[], int n, int id);
// Binary search that counts number of array accesses/comparisons.
// If 'accesses' is not NULL it will be incremented on each probe.
Item* BinarySearchWithCount(Item products[], int n, int id, int *accesses);