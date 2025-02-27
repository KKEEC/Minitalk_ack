/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkeec <krishnakeec@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 13:21:13 by kkeec             #+#    #+#             */
/*   Updated: 2025/02/27 17:21:08 by kkc              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

volatile int	g_ack_received = 0;

void	ack_handler(int sig)
{
	if (sig == SIGUSR1)
		g_ack_received = 1;
	else if (sig == SIGUSR2)
	{
		ft_putstr("Message sent sucessfully\n");
		exit(EXIT_SUCCESS);
	}
}

void	send_bit(int server_pid, int bit)
{
	if (bit == 1)
		kill(server_pid, SIGUSR1);
	else
		kill(server_pid, SIGUSR2);
}

void	send_char(int server_pid, unsigned char c)
{
	int	i;
	int	bit;
	int	wait_time;

	wait_time = 100;
	i = 0;
	while (i < 8)
	{
		bit = (c >> (7 - i)) & 1;
		g_ack_received = 0;
		send_bit(server_pid, bit);
		while (!g_ack_received)
		{
			usleep(500);
			wait_time += 100;
			if (wait_time == 100000)
			{
				ft_putstr("Server Not responding!!\nCheck PID\n");
				exit(EXIT_FAILURE);
			}
		}
		i++;
	}
}

void	send_message(int server_pid, unsigned char *message)
{
	int	i;

	i = 0;
	while (message[i] != '\0')
	{
		send_char(server_pid, message[i]);
		i++;
	}
	send_char(server_pid, '\0');
	exit(EXIT_SUCCESS);
}

int	main(int argc, char *argv[])
{
	pid_t				server_pid;
	struct sigaction	sa;

	if (argc != 3)
	{
		ft_putstr("Usage: ./client <pid> <message>\n");
		return (EXIT_FAILURE);
	}
	server_pid = (pid_t)ft_atoi(argv[1]);
	sa.sa_handler = ack_handler;
	sa.sa_flags = SA_RESTART;
	if (sigemptyset(&sa.sa_mask) == -1)
	{
		ft_putstr("Server Not responding!!\nCheck PID\n");
		return (EXIT_FAILURE);
	}
	if (sigaction(SIGUSR1, &sa, NULL) == -1
		|| sigaction(SIGUSR2, &sa, NULL) == -1)
	{
		perror("sigaction failed");
		return (EXIT_FAILURE);
	}
	send_message(server_pid, (unsigned char *)argv[2]);
	return (EXIT_SUCCESS);
}
