/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkeec <krishnakeec@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 14:14:26 by kkeec             #+#    #+#             */
/*   Updated: 2025/02/20 14:28:16 by kkeec            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

static t_server	g_server = {0, 0, 0};

void	handler(int sig, siginfo_t *info, void *context)
{
	(void)context;
	
	if (g_server.current_client_pid == 0
		|| g_server.current_client_pid == info->si_pid)
	{
		if (g_server.current_client_pid == 0)
			g_server.current_client_pid = info->si_pid;
		if (sig == SIGUSR1)
			g_server.received_char |= (1 << (7 - g_server.bit_count));
		g_server.bit_count++;
		if (g_server.bit_count == 8)
		{
			if (g_server.received_char == '\0')
			{
				write(1, "\nMessage received successfully!\n", 32);
				ft_putnbr(info->si_pid);
				ft_putstr("Server PID is :");
				ft_putnbr(getpid());
				write(1, "\n", 1);
				kill(g_server.current_client_pid, SIGUSR2);
				g_server.bit_count = 0;
				g_server.received_char = 0;
				g_server.current_client_pid = 0;
				return ;
			}
			else
				write(1, &g_server.received_char, 1);
			g_server.bit_count = 0;
			g_server.received_char = 0;
		}
		kill(g_server.current_client_pid, SIGUSR1);
	}
	else
		kill(info->si_pid, SIGUSR2);
}

int	main(void)
{
	int					pidval;
	struct sigaction	sa;

	pidval = (int)getpid();
	ft_putstr("Server PID is :");
	ft_putnbr(pidval);
	write(1, "\n", 1);
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = handler;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGUSR1, &sa, NULL) == -1
		|| sigaction(SIGUSR2, &sa, NULL) == -1)
	{
		write(2, "Error registering signal handler\n", 33);
		return (EXIT_FAILURE);
	}
	while (1)
		pause();
	return (EXIT_SUCCESS);
}
