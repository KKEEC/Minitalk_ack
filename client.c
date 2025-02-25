/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkeec <krishnakeec@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 13:21:13 by kkeec             #+#    #+#             */
/*   Updated: 2025/02/20 13:45:04 by kkeec            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

volatile int	ack_received = 0;
volatile int	final_ack = 0;

void	ack_handler(int sig)
{
	if (sig == SIGUSR1)
		ack_received = 1;
	else if (sig == SIGUSR2)
	{
		final_ack = 1;
		ack_received = 1;
	}
}

void	send_bit(int server_pid, int bit)
{
	if (bit == 1)
		kill(server_pid, SIGUSR1);
	else
		kill(server_pid, SIGUSR2);
}

void	send_char(int server_pid, char c)
{
	int	i;
	int	bit;

	i = 0;
	while (i < 8)
	{
		bit = (c >> (7 - i)) & 1;
		ack_received = 0;
		send_bit(server_pid, bit);
		while (!ack_received)
			pause();
		if (final_ack)
			return ;
		usleep(100);
		i++;
	}
}

void	send_message(int server_pid, char *message)
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
	int					server_pid;
	char				*message;
	struct sigaction	sa;

	if (argc != 3)
	{
		ft_putstr("Usage: ./client <pid> <message>\n");
		return (EXIT_FAILURE);
	}
	server_pid = ft_atoi(argv[1]);
	message = argv[2];
	sa.sa_handler = ack_handler;
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGUSR1, &sa, NULL) == -1
		|| sigaction(SIGUSR2, &sa, NULL) == -1)
	{
		perror("sigaction failed");
		return (EXIT_FAILURE);
	}
	send_message(server_pid, message);
	return (EXIT_SUCCESS);
}
