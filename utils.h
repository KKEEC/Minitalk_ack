/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkc <kkc@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 19:14:08 by kkc               #+#    #+#             */
/*   Updated: 2025/02/27 19:22:45 by kkc              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include <stdio.h>
# include <unistd.h>
# include <signal.h>
# include <stdlib.h>

typedef struct s_server
{
	unsigned char	received_char;
	int				bit_count;
	pid_t			current_client_pid;
}	t_server;
int		ft_atoi(char *str);
void	ft_putstr(char *str);
void	ft_putnbr(int nbr);
size_t	ft_strlen(char *str);

#endif
