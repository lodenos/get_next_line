#include "get_next_line.h"

void *ft_memchr(char const *ptr, int c, size_t n) {
  unsigned char *head;

  head = (unsigned char *)ptr;
  while (n--)
    if (*head++ == (unsigned char)c)
      return --head;
  return NULL;
}

void *ft_memcpy(void *dst, void const *src, size_t n) {
  void *const head = dst;

  if (!dst && !src)
    return head;
  while (n--)
    *(unsigned char *)dst++ = *(unsigned char *)src++;
  return head;
}

void gnl_list_clear(t_gnl_list **list) {
  t_gnl_list *indirect;

  while (*list) {
    indirect = (*list)->next;
    free(*list);
    *list = indirect;
  }
  *list = NULL;
}

t_gnl_list *gnl_list_push(t_gnl_list **list, t_gnl_list *node) {
  t_gnl_list *head = *list;

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
