#include "get_next_line.h"

void *ft_memcpy(void *dst, void const *src, size_t n) {
  void *const head = dst;

  if (!dst && !src)
    return head;
  while (n--)
    *(unsigned char *)dst++ = *(unsigned char *)src++;
  return head;
}

void *ft_memchr(char const *ptr, int c, size_t n) {
  unsigned char *head;

  head = (unsigned char *)ptr;
  while (n--)
    if (*head++ == (unsigned char)c)
      return --head;
  return NULL;
}

void list_clear(t_list **list, void (*delete)(void *)) {
  t_list *next;

  if (!list || !delete)
    return ;
  while (*list) {
    next = (*list)->next;
    delete((*list)->content);
    free(*list);
    *list = next;
  }
  *list = NULL;
}

t_list *list_new(void *content, size_t size) {
  t_list *const list = (t_list *)malloc(sizeof(t_list));

  if (!list)
    return NULL;
  *list = (t_list){ content, size, NULL };
  return list;
}

t_list *list_push(t_list **list, t_list *node) {
  t_list *head = *list;

  if (!node)
    return head;
  if (!head) {
    *list = node;
    return node;
  }
  while (head->next)
    head = head->next;
  head->next = node;
  return *list;
}
