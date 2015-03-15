#include <stdio.h>

#include "config.h"
#include "fts_.h"
#include "progname.h"
#include "error.h"
#include "errno.h"
#include "string.h"
#include "fcntl.h"
#include "unistd.h"

int main()
{
  set_program_name("testgnulib");

  // This code demonstrates a bug in fdopendir on Mingw.
  {
    int fd = open("emptydir", O_RDONLY | O_DIRECTORY | O_NOCTTY | O_NONBLOCK);
    if (fd < 0)
    {
      fprintf(stderr, "open returned %d\n", fd);
      return 1;
    }

    printf("dup(fd) = %d\n", dup(fd));
    fdopendir(fd);
    printf("dup(fd) = %d\n", dup(fd));
  }

  // This code shows how the bug affects fts.
  {
    char filename[] = "emptydir";
    char * fts_args[] = { filename, NULL };
    int fts_opts = FTS_PHYSICAL;
    FTS * fts = fts_open(fts_args, fts_opts, NULL);
    if (!fts)
    {
      fprintf(stderr, "fts_open returned NULL\n");
      return 2;
    }

    while(1)
    {
      FTSENT * ent = fts_read(fts);
      if (!ent) { break; }
      if (ent->fts_info == FTS_ERR)
      {
        printf("%s: error: %s\n", ent->fts_path, strerror(ent->fts_errno));
      }
      else
      {
        printf("%s\n", ent->fts_path);
      }
    }
  }
  return 0;
}
