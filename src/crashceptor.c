//----------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <limits.h>

//----------------------------------------------------------------------------

#ifndef ORIG_BIN_DIR
#define ORIG_BIN_DIR "/var/lib/crashceptor"
#endif

#ifndef LOG_DIR
#define LOG_DIR "/var/log/crashceptor"
#endif

//----------------------------------------------------------------------------

char *path_basename(char *progname)
{
  char *basename = strrchr(progname, '/');
  return basename != NULL ? basename : progname;
}

int path_join(char *base_path, char *progname, char *buffer, size_t bufsize)
{
  int size = snprintf(buffer, bufsize, "%s/%s",
                      base_path, path_basename(progname));
  if (size >= bufsize) {
    errno = ENAMETOOLONG;
    return -1;
  }

  return 0;
}

//----------------------------------------------------------------------------

FILE* open_log_file(char *progname)
{
  char template[PATH_MAX];
  char log_file[PATH_MAX];
  // NOTE: if this one is too long, path_join() will fail as well (its
  // resulting path is even longer)
  snprintf(log_file, sizeof(log_file), "log.%s.%d.XXXXXX", 
           path_basename(progname), getpid());

  if (path_join(LOG_DIR, log_file, template, sizeof(template)) != 0)
    // errno already set
    return NULL;

  int fd = mkstemp(template);
  if (fd < 0)
    // errno already set
    return NULL;

  return fdopen(fd, "w");
}

//----------------------------------------------------------------------------

pid_t execute(char *progname, char **argv, FILE **log_handle)
{
  char exec_path[PATH_MAX];
  if (path_join(ORIG_BIN_DIR, progname, exec_path, sizeof(exec_path)) != 0)
    // errno already set
    return -1;

  if (access(exec_path, X_OK) != 0)
    // errno already set
    return -1;

  FILE *handle = open_log_file(progname);
  if (handle == NULL)
    // errno already set
    return -1;

  pid_t child = fork();
  if (child < 0) {
    int old_errno = errno;
    fprintf(handle, "## @%d fork() failed: errno=%d (%s)\n",
            time(NULL), errno, strerror(errno));
    fclose(handle);
    errno = old_errno;
    // errno already set
    return -1;
  }

  if (child > 0) {
    *log_handle = handle;
    // parent process
    return child;
  }

  // XXX: child process

  dup2(fileno(handle), 1); // STDOUT
  dup2(fileno(handle), 2); // STDERR
  fprintf(handle, "## @%d pid=%d executing %s\n",
          time(NULL), getpid(), progname);
  fclose(handle);

  execv(exec_path, argv);
  fprintf(stderr, "## @%d execv() failed: errno=%d (%s); aborting\n",
          time(NULL), errno, strerror(errno));
  abort();
}

//----------------------------------------------------------------------------

int main(int argc, char **argv)
{
  FILE *log_handle = NULL;
  pid_t process = execute(argv[0], argv, &log_handle);

  if (process < 0) {
    fprintf(stderr, "executing %s failed: %s\n", argv[0], strerror(errno));
    abort();
  }

  int status;
  waitpid(process, &status, 0);

  if (WIFEXITED(status)) { // regular exit
    fprintf(log_handle, "## @%d exit %d\n", time(NULL), WEXITSTATUS(status));
    fclose(log_handle);
    return WEXITSTATUS(status);
  } else { // signal
    fprintf(log_handle, "## @%d signal %d\n", time(NULL), WTERMSIG(status));
    fclose(log_handle);
    raise(WTERMSIG(status));
  }

  return 255; // never reached
}

//----------------------------------------------------------------------------
// vim:ft=c
