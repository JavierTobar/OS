#ifndef SHELLMEMORY_H
#define SHELLMEMORY_H


/*
  This function takes a key and a value and adds it into memory
*/
int shell_memory_set(const char *key, const char *value);

/*
  This function searches for a key and returns it's value 
  if the key doesn't exist then we return NULL
*/
const char *shell_memory_get(const char *key);

/*
  This function initializes our memory to NULL
*/
void shell_memory_initialize();

/*
  This function frees all memory
*/
void shell_memory_destroy();

#endif
