#include "get_next_line_bonus.h"

static char *create_buffer_line_feed(t_gnl_list *chunks, t_ptr_char *buffer) {
  t_ptr_char head;
  char *match;

  *buffer = (t_ptr_char) { 0 };
  while (chunks) {
    head.data = chunks->buffer + chunks->index;
    head.size = chunks->size - chunks->index;
    match = ft_memchr(head.data, '\n', head.size);
    if (match) {
      buffer->size += match - head.data + 1;
      break ;
    }
    buffer->size += head.size;
    chunks = chunks->next;
  }
  buffer->size += 1;
  buffer->data = malloc(buffer->size);
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

static char *dump_line_feed(t_gnl_list **chunks, bool dump) {
  t_ptr_char buffer;
  t_gnl_list *chunk;

  if (dump || !*chunks || !create_buffer_line_feed(*chunks, &buffer)) {
    gnl_list_clear(chunks);
    return NULL;
  }
  while (*chunks) {
    chunk = dump_line_fill(*chunks, &buffer);
    if (!chunk)
      break ;
    *chunks = (*chunks)->next;
    free(chunk);
    if (buffer.index == SIZE_MAX)
      break ;
  }
  if (!buffer.data)
    gnl_list_clear(chunks);
  return buffer.data;
}

char *get_next_line(int fd) {
  static t_gnl_list *chunks[MAX_FD] = { 0 };
  t_gnl_list *node;
  ssize_t read_size;

  if (BUFFER_SIZE < 1 || fd < 0 || fd > MAX_FD)
    return NULL;
  node = (t_gnl_list *)malloc(sizeof(t_gnl_list));
  if (!node) 
    return dump_line_feed(&chunks[fd], true);
  *node = (t_gnl_list){ 0 };
  read_size = read(fd, node->buffer, BUFFER_SIZE);
  if (read_size < 1) {
    free(node);
    if (read_size == -1)
      return dump_line_feed(&chunks[fd], true);
    return dump_line_feed(&chunks[fd], false);
  }
  node->size = read_size;
  gnl_list_push(&chunks[fd], node);
  if (ft_memchr(node->buffer, '\n', node->size))
    return dump_line_feed(&chunks[fd], false);
  return get_next_line(fd);
}
