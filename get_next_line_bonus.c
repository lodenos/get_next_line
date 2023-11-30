#include "get_next_line_bonus.h"

static t_list *chunk_join(t_list *chunks) {
  size_t size;
  t_list *tmp;
  void *buffer;
  void *head;

  size = 0;
  tmp = chunks;
  while (tmp) {
    size += tmp->size;
    tmp = tmp->next;
  }
  buffer = malloc(size);
  if (!buffer)
    return NULL;
  head = buffer;
  tmp = chunks;
  while (tmp) {
    ft_memcpy(head, tmp->content, tmp->size);
    head += tmp->size;
    tmp = tmp->next;
  }
  return list_new(buffer, size);
}

static t_list *read_next_chunk(int fd) {
  char *const buffer = (char *)malloc(BUFFER_SIZE);
  ssize_t len_read;

  if (!buffer)
    return NULL;
  len_read = read(fd, buffer, BUFFER_SIZE);
  if (len_read < 1) {
    free(buffer);
    return NULL;
  }
  return list_new(buffer, len_read);
}

static char *get_line_feed(t_list **chunks) {
  char *buffer;
  t_list *node;
  size_t len;

  node = chunk_join(*chunks);
  list_clear(chunks, free);

  if (ft_memchr(node->content, '\n', node->size) == NULL) {
    buffer = (char *)malloc(node->size + 1);
    buffer[node->size] = 0;
    ft_memcpy(buffer, node->content, node->size);
    list_clear(&node, free);
    return buffer;
  }


  char *tail = ft_memchr(node->content, '\n', node->size);
  len = tail - (char *)node->content + 1;

//  printf("> node->size: %zu\n", node->size);
//  printf(">        len: %zu\n", node->size);


  buffer = (char *)malloc(len + 1);
  buffer[len] = 0;
  ft_memcpy(buffer, node->content, len);

  if (len == node->size) {
    list_clear(&node, free);
    return buffer;
  }
//  printf(">        len: %zu\n", len);
//  printf("> node->size: %zu\n", node->size);

  char *new_content = (char *)malloc(node->size - len);
  ft_memcpy(new_content, node->content + len, node->size - len);


  *chunks = list_new(new_content, node->size - len);
  list_clear(&node, free);
  return buffer;
}

char *get_next_line(int fd) {
  static t_list *chunks[4096] = {NULL};
  t_list *node;

  if (fd < 0 || fd > 1024)
    return NULL;
  while (1) {
    node = read_next_chunk(fd);
    if (!node) {
      if (!chunks[fd])
        return NULL;
      return get_line_feed(&chunks[fd]);
    }
    list_push(&chunks[fd], node);
    if (ft_memchr(node->content, '\n', node->size))
      return get_line_feed(&chunks[fd]);
  }
  return NULL;
}
