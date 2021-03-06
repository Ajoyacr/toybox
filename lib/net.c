#include "toys.h"

int xsocket(int domain, int type, int protocol)
{
  int fd = socket(domain, type, protocol);

  if (fd < 0) perror_exit("socket %x %x", type, protocol);
  return fd;
}

void xsetsockopt(int fd, int level, int opt, void *val, socklen_t len)
{
  if (-1 == setsockopt(fd, level, opt, val, len)) perror_exit("setsockopt");
}

int xconnect(char *host, int port, int family, int socktype, int protocol,
             int flags)
{
  char buf[32];
  struct addrinfo info, *ai;
  int fd;

  memset(&info, 0, sizeof(struct addrinfo));
  info.ai_family = family;
  info.ai_socktype = socktype;
  info.ai_protocol = protocol;
  info.ai_flags = flags;

  sprintf(buf, "%d", port);
  fd = getaddrinfo(host, port ? buf : 0, &info, &ai);

  if (fd || !ai)
    error_exit("Connect '%s:%d': %s", host, port,
      fd ? gai_strerror(fd) : "not found");

  fd = xsocket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
  if (connect(fd, ai->ai_addr, ai->ai_addrlen)) perror_exit("connect");
  freeaddrinfo(ai);

  return fd;
}
