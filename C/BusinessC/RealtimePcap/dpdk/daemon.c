#include "daemon.h"

static volatile sig_atomic_t sigusr1_flag = 0;

/**
 * \brief Signal handler used to take the parent process out of stand-by
 */
static void signal_handler_usr1 (int signo)
{
    (void)signo;
    sigusr1_flag = 1;
}

/**
 * \brief Tell the parent process the child is ready
 *
 * \param pid pid of the parent process to signal
 */
static void tell_waiting_parent (pid_t pid)
{
    kill(pid, SIGUSR1);
}

static void wait_for_child(pid_t pid)
{
    int status;
    // trace("Daemon: Parent waiting for child to be ready...");
    /* Wait until child signals is ready */
    while (sigusr1_flag == 0) {
        if (waitpid(pid, &status, WNOHANG)) {
            /* Check if the child is still there, otherwise the parent should exit */
            if (WIFEXITED(status) || WIFSIGNALED(status)) {
                // fatal("Child died unexpectedly");
                exit(EXIT_FAILURE);
            }
        }
        /* sigsuspend(); */
        sleep(1);
    }
}

static void SetupLogging (void)
{
    /* Redirect stdin, stdout, stderr to /dev/null  */
    int fd = open("/dev/null", O_RDWR);
    if (fd < 0)
        return;
    (void)dup2(fd, 0); 
    (void)dup2(fd, 1); 
    (void)dup2(fd, 2); 
    close(fd);
}

void daemonize(){
	pid_t pid, sid;

    /* Register the signal handler */
    signal(SIGUSR1, signal_handler_usr1);

    /** \todo We should check if wie allow more than 1 instance
              to run simultaneously. Maybe change the behaviour
              through conf file */

    /* Creates a new process */
    pid = fork();

    if (pid < 0) {
        /* Fork error */
        // fatal("Error forking the process");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        /* Child continues here */
        sid = setsid();
        if (sid < 0) {
            // fatal("Error creating new session");
            exit(EXIT_FAILURE);
        }

        SetupLogging();

        /* Child is ready, tell its parent */
        tell_waiting_parent(getppid());

        /* Daemon is up and running */
        // trace("Daemon is running");
        return;
    }
    /* Parent continues here, waiting for child to be ready */
    // trace("Parent is waiting for child to be ready");
    wait_for_child(pid);

    /* Parent exits */
    // trace("Child is ready, parent exiting");
    exit(EXIT_SUCCESS);
}
