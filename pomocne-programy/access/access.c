/*
 *	A simple memory access speed test
 *
 *	Written by Martin Mares <mj@ucw.cz> and put into public domain.
 *
 *	Compile-time switches:
 *	  SIZE=xxx	Size of a single item in bytes
 *	  RANDOMIZE	Define for random access, undef for sequential
 *	  MODIFY	Define for read-write accesses, undef for read-only
 *	  MS		Measurement period in ms
 *	  ARRAY		Access items as array (default: linked list)
 *	  HUGE		Allocate huge pages (hugetlbfs required)
 *
 *	Output is in nanoseconds per access.
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <time.h>

struct item {
  struct item *next;
  uint32_t pad[(SIZE-sizeof(struct item *)) / 4];
};

static struct item *items;
static unsigned block_size, n;

#define TICKS_PER_SEC 1000000
static int64_t get_timer(void)
{
  struct timeval t;
  gettimeofday(&t, NULL);
  return 1000000*t.tv_sec + t.tv_usec;
}

#ifdef HUGE

static void alloc_mem(void)
{
#define HUGE_PATH "huge/access"
#define HUGE_PAGE (2*1048576)
  void *pp;
  unlink(HUGE_PATH);
  int fd = open(HUGE_PATH, O_RDWR | O_CREAT, 0644);
  if (fd < 0)
    {
      fprintf(stderr, "Cannot open hugetlbfs at %s: %m\n", HUGE_PATH);
      exit(1);
    }
  if ((pp = mmap(NULL, ((block_size+HUGE_PAGE-1)/HUGE_PAGE)*HUGE_PAGE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED)
    {
      fprintf(stderr, "Cannot mmap hugetlbfs: %m\n");
      exit(1);
    }
  items = pp;
}

#else

static void alloc_mem(void)
{
  void *pp = NULL;
  if (posix_memalign(&pp, 4096, block_size) < 0)
    {
      fprintf(stderr, "Cannot allocate memory: %m\n");
      exit(1);
    }
  items = pp;
}

#endif

static inline void eat(uint32_t x)
{
  // Discard a value, but do not allow optimizing it out
  asm volatile ("" : : "r" (x));
}

static inline void process_item(struct item *x)
{
#ifdef MODIFY
  static uint32_t cnt;
  x->pad[0] = cnt++;
#else
  eat(x->pad[0]);
#endif
}

#ifdef ARRAY

static void fill(void)
{
  memset(items, 0, block_size);
}

static void loop(void)
{
#ifdef RANDOMIZE
  if (n & (n-1))
    {
      fprintf(stderr, "Block size must be a power of two!\n");
      exit(1);
    }
  unsigned i=0;
  do
    {
      process_item(&items[i]);
      i = (i+259309) & (n-1);
    }
  while (i);
#else
  struct item *stop = items + n;
  for (struct item *a = items; a < stop; a++)
    process_item(a);
#endif
}

#else

static void fill(void)
{
  memset(items, 0, block_size);
#ifdef RANDOMIZE
  for (unsigned i=0; i<n; i++)
    {
      unsigned j=(i+259309) % n;
      if (j)
	items[i].next = &items[j];
    }
#else
  for (unsigned i=0; i<n-1; i++)
    items[i].next = &items[i+1];
#endif
}

static void loop(void)
{
  for (struct item *a = items; a; a=a->next)
    process_item(a);
}

#endif

int main(int argc, char **argv)
{
  if (argc != 2)
    {
      fprintf(stderr, "Usage: access <size-in-KB>\n");
      return 1;
    }
  block_size = atoi(argv[1]) * 1024;
  n = block_size / SIZE;

  alloc_mem();
  fill();

  unsigned attempts = 1;
  for (;;)
    {
      int64_t t0 = get_timer();

      for (unsigned i=0; i<attempts; i++)
	loop();

      int64_t t1 = get_timer();
      int64_t t = t1-t0;
      if (t >= TICKS_PER_SEC/1000*MS)
	{
	  printf("%.3f %f\n", (double)t/attempts/n/TICKS_PER_SEC*1e9, (double)(2*attempts-1)*n);
	  break;
	}
      attempts *= 2;
    }

  return 0;
}
