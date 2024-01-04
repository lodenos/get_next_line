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
