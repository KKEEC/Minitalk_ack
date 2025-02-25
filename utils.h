#ifndef UTILS_H
# define UTILS_H

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

typedef struct s_server {
    unsigned char received_char;
    int bit_count;
    pid_t current_client_pid;
} t_server;

int ft_atoi(char *str);
void    ft_putstr(char *str);
void    ft_putnbr(int nbr);
size_t ft_strlen(char *str);
void    *ft_memcpy(void *dest, const void *src, size_t n);
void *ft_realloc(void *ptr, size_t new_size);


#endif