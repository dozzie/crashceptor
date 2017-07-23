Process crash interceptor
=========================

Crashceptor is a tool written to leave a trace when some daemon crashes or
terminates unexpectedly.

Crashceptor is deployed for a daemon by renaming the original binary and
leaving in its place symlink to `crashceptor`. Crashceptor calls the original
binary with all passed command line arguments, redirecting command's *STDOUT*
and *STDERR* to a log file. Crashceptor then waits for daemon to terminate and
records its exit code or termination signal in the log file.

*NOTE*: Crashceptor was written as a simple diagnostic tool for the author's
environment (Debian network), thus there are currently no plans to make it
more generic or write much user documentation.

Known limitations and problems
------------------------------

* since crashceptor uses `wait(2)` syscall, daemon that detaches from the
  terminal cannot be tracked
* currently all daemon's *STDOUT* and *STDERR* lands in log file, which is
  undesirable for daemons with additional administrative commands
* any pidfile daemon creates points to the daemon's process, but a pidfile
  created by tools like `start-stop-daemon(8)` points to crashceptor
  process
* if the daemon spawns subprocesses and they don't exit with the daemon, log
  file can be left opened and grow in size even after daemon terminates

Contact and License
-------------------

Crashceptor is written by Stanislaw Klekot <dozzie at jarowit.net>.
The primary distribution point is <http://dozzie.jarowit.net/>.

Crashceptor is distributed under 3-clause BSD license. See `COPYING` file for
details.

