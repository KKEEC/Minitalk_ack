#include "utils.h"
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

static unsigned char received_char = 0;
static int bit_count = 0;
static pid_t current_client_pid = 0;

void handler(int sig, siginfo_t *info, void *context) 
{
    (void)context;

    if (current_client_pid == 0) {
        // The first signal we get will assign the client PID
        current_client_pid = info->si_pid;
    }

    // Handle the incoming bit
    if (sig == SIGUSR1) {
        received_char |= (1 << (7 - bit_count));  // Set bit
    }
    bit_count++;

    // Check if a full character has been received
    if (bit_count == 8) {
        if (received_char == '\0') {  // End of message
            write(1, "\nMessage received successfully!\n", 32);
            
            ft_putstr("Server PID is :");
            ft_putnbr(getpid());
            write(1, "\n", 1);
            // ✅ Send final ACK to let client exit
            kill(current_client_pid, SIGUSR2);
            
            // Reset for next message
            bit_count = 0;
            received_char = 0;
            current_client_pid = 0;  // Reset the client PID to be ready for the next message
            return;
        } else {
            write(1, &received_char, 1);
        }

        // Reset variables for the next character
        bit_count = 0;
        received_char = 0;
    }

    // Send regular acknowledgment
    kill(current_client_pid, SIGUSR1);
}

int main(void) {
    int pidval = (int)getpid();
    ft_putstr("Server PID is :");
    ft_putnbr(pidval);
    write(1, "\n", 1);

    // Set up the signal handler for SIGUSR1 and SIGUSR2
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = handler;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGUSR1, &sa, NULL) == -1 || sigaction(SIGUSR2, &sa, NULL) == -1) {
        write(2, "Error registering signal handler\n", 33);
        return EXIT_FAILURE;
    }

    // The server will keep running indefinitely, ready to receive messages
    while (1) {
        pause();  // Wait for signals
    }

    return EXIT_SUCCESS;
}
