#include "binary_search.h"

Item* BinarySearch(Item products[], int n, int id){
    int low = 0;
    int high = n - 1;
    while(low <= high){
        int mid = low + (high - low) / 2;

        // Check if id was find
        if(products[mid].id == id)
            return &products[mid];

        // Based in value split the array to the correct half
        if(products[mid].id < id)
            low = mid + 1;
        else
            high = mid - 1;
    }

    return NULL;
}

Item* BinarySearchWithCount(Item products[], int n, int id, int *accesses){
    int low = 0;
    int high = n - 1;
    while(low <= high){
        int mid = low + (high - low) / 2;
        if (accesses) (*accesses)++; // one probe/comparison of products[mid]

        if(products[mid].id == id)
            return &products[mid];

        if(products[mid].id < id)
            low = mid + 1;
        else
            high = mid - 1;
    }

    return NULL;
}