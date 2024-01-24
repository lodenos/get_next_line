#ifndef GET_NEXT_LINE
#define GET_NEXT_LINE

#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>

#define MAX_FD 4096

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 10
#endif

typedef struct s_gnl_list {
  char buffer[BUFFER_SIZE];
  size_t index;
  size_t size;
  struct s_gnl_list *next;
} t_gnl_list;

typedef struct s_ptr_char {
  char *data;
  size_t index;
  size_t size;
} t_ptr_char;

void *ft_memchr(char const *, int, size_t);
void *ft_memcpy(void *, void const *, size_t);
char *get_next_line(int fd);
void gnl_list_clear(t_gnl_list **);
t_gnl_list *gnl_list_push(t_gnl_list **, t_gnl_list *);

#endif
