/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkc <kkc@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 17:21:41 by kkc               #+#    #+#             */
/*   Updated: 2025/02/27 19:28:22 by kkc              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

void	handler(int sig, siginfo_t *info, void *context)
{
	static t_server	g_val = {0};

	(void)context;
	g_val.current_client_pid = info->si_pid;
	if (sig == SIGUSR1)
		g_val.received_char |= (1 << (7 - g_val.bit_count));
	g_val.bit_count++;
	if (g_val.bit_count == 8)
	{
		if (g_val.received_char == '\0')
		{
			write(1, "\nMessage received successfully!\n", 32);
			kill(g_val.current_client_pid, SIGUSR2);
			g_val.bit_count = 0;
			g_val.received_char = 0;
			g_val.current_client_pid = 0;
			return ;
		}
		else
			write(1, &g_val.received_char, 1);
		g_val.bit_count = 0;
		g_val.received_char = 0;
	}
	kill(g_val.current_client_pid, SIGUSR1);
}

int	main(void)
{
	int					pidval;
	struct sigaction	sa;

	pidval = (int)getpid();
	ft_putstr("Server PID is : ");
	ft_putnbr(pidval);
	write(1, "\n", 1);
	sa.sa_flags = SA_RESTART | SA_SIGINFO;
	sa.sa_sigaction = handler;
	if (sigemptyset(&sa.sa_mask) == -1)
	{
		ft_putstr("Server Not responding!!\nCheck PID\n");
		return (EXIT_FAILURE);
	}
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
