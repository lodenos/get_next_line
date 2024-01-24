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

static char *create_buffer_line_feed(t_gnl_list *chunks, t_ptr_char *buffer) {
  t_ptr_char head;
  char *match;

  *buffer = (t_ptr_char) { 0 };
  head.data = chunks->buffer + chunks->index;
  head.size = chunks->size - chunks->index;
  while (chunks) {
    match = ft_memchr(head.data, '\n', head.size);
    if (match) {
      buffer->size += match - head.data + 1;
      break ;
    }
    buffer->size += chunks->size;
    chunks = chunks->next;
  }
  buffer->data = malloc(buffer->size + 1);
  return buffer->data;
}

static t_gnl_list *dump_line_fill(t_gnl_list *chunk, t_ptr_char *buffer) {
  t_ptr_char head;
  char *match;
  size_t size;

  head.data = chunk->buffer + chunk->index;
  head.size = chunk->size - chunk->index;
  match = ft_memchr(head.data, '\n', head.size);
  if (match) {
    size = match - head.data + 1;
    ft_memcpy(buffer->data + buffer->index, head.data, size);
    chunk->index += size;
    buffer->data[buffer->index + size] = 0;
    if (chunk->index >= chunk->size) {
      buffer->index = SIZE_MAX;
      return chunk;
    }
    return NULL;
  }
  ft_memcpy(buffer->data + buffer->index, head.data, head.size);
  buffer->data[buffer->index + head.size] = 0;
  buffer->index += head.size;
  return chunk;
}

static char *dump_line_feed(t_gnl_list **chunks) {
  t_ptr_char buffer;
  t_gnl_list *chunk;

  if (!*chunks || !create_buffer_line_feed(*chunks, &buffer))
    return NULL;
  while (*chunks) {
    chunk = dump_line_fill(*chunks, &buffer);
    if (!chunk)
      break ;
    *chunks = (*chunks)->next;
    free(chunk);
    if (buffer.index == SIZE_MAX)
      break ;
  }
  return buffer.data;
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
