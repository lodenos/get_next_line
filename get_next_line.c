#include "get_next_line.h"

static t_gnl_list *gnl_list_push(t_gnl_list **list, t_gnl_list *node) {
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

static size_t size_line_feed(t_gnl_list *chunks) {
  size_t size;
  char *match;

  size = 0;
  while (chunks) {
    match = ft_memchr(chunks->buffer + chunks->index, '\n',
      chunks->size - chunks->index);
    if (match)
      return size + match - (chunks->buffer + chunks->index) + 1;
    size += chunks->size;
    chunks = chunks->next;
  }
  return size;
}

static t_gnl_list *dump_line_fill(t_gnl_list *chunk, char *buffer,
    size_t *index) {
  char *match = ft_memchr(chunk->buffer + chunk->index, '\n',
      chunk->size - chunk->index);

  if (match) {
    ft_memcpy(buffer + *index, chunk->buffer + chunk->index,
      match - (chunk->buffer + chunk->index) + 1);
    chunk->index += match - (chunk->buffer + chunk->index) + 1;
    if (chunk->index >= chunk->size) {
      *index = SIZE_MAX;
      return chunk;
    }
    return NULL;
  }
  ft_memcpy(buffer + *index, chunk->buffer + chunk->index,
    chunk->size - chunk->index);
  *index += chunk->size - chunk->index;
  return chunk;
}

static char *dump_line_feed(t_gnl_list **chunks) {
  t_gnl_list *chunk;
  char *buffer;
  size_t size;

  if (!*chunks)
    return NULL;
  size = size_line_feed(*chunks);
  buffer = (char *)malloc(size + 1);
  if (!buffer)
    return NULL;
  buffer[size] = 0;
  size = 0;
  while (*chunks) {
    chunk = dump_line_fill(*chunks, buffer, &size);
    if (!chunk)
      break ;
    *chunks = (*chunks)->next;
    free(chunk);
    if (size == SIZE_MAX)
      break ;
  }
  return buffer;
}

char *get_next_line(int fd) {
  static t_gnl_list *chunks = NULL;
  t_gnl_list *node;
  ssize_t size;

  if (BUFFER_SIZE < 1 || fd < 0 || fd > MAX_FD)
    return NULL;
  while (true) {
    node = (t_gnl_list *)malloc(sizeof(t_gnl_list));
    if (!node)
      return NULL;
    *node = (t_gnl_list){ 0 };
    size = read(fd, node->buffer, BUFFER_SIZE);
    if (size < 1) {
      free(node);
      break ;
    }
    node->size = size;
    gnl_list_push(&chunks, node);
    if (ft_memchr(node->buffer, '\n', node->size))
      break ;
  }
  return dump_line_feed(&chunks);
}
