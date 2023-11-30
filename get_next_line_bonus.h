#ifndef GET_NEXT_LINE
#define GET_NEXT_LINE

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 5
#endif

#include <stdio.h>

typedef struct s_list {
  void *content;
  size_t size;
  struct s_list *next;
} t_list;

void *ft_memcpy(void *dst, void const *src, size_t n);
void *ft_memchr(char const *ptr, int c, size_t n);

void list_clear(t_list **list, void (*del)(void *));
t_list *list_new(void *content, size_t size);
t_list *list_push(t_list **list, t_list *node);

char *get_next_line(int fd);

#endif
