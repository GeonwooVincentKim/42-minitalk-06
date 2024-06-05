/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: geonwkim <geonwkim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 14:33:59 by geonwkim          #+#    #+#             */
/*   Updated: 2024/06/05 14:34:30 by geonwkim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "libft/libft.h"

// When a client and server send messages to each other, 
// the role of 'receive' is to output the message that 
// the client has already sent to the server, and the client 
// receives it back from the server. 

// In other words, 'receive' is used to verify that the client's message
// has been sent to the server and successfully output.

// __attribute__((destructor))
// static void destructor() {
//     system("leaks -q client");
// }

static void	action(int sig)
{
	static int	received = 0;

	if (sig == SIGUSR1)
		++received;
	else
	{
		ft_putnbr_fd(received, 1);
		ft_putchar_fd('\n', 1);
		exit(0);
	}
}

static void	check_status(int pid)
{
	int	i;

	i = 8;
	while (i--)
	{
		kill(pid, SIGUSR1);
		usleep(100);
	}
}

// i = 8 -> 1 bytes
// Send the message from client to server
// If is Sigusr2, then you check the string has the Null-Character
// and send it to the Server

// Otherwise, you send the character one by one and send it to the Server
// (Sigusr1)
static void	mt_kill(int pid, char *str)
{
	int		i;
	char	c;

	while (*str)
	{
		i = 8;
		c = *str++;
		while (i--)
		{
			if (c >> i & 1)
			{
				if (kill(pid, SIGUSR2) == -1)
					exit(1);
			}
			else
			{
				if (kill(pid, SIGUSR1) == -1)
					exit(1);
			}
			usleep(100);
		}
	}
	check_status(pid);
}

// printf("PID %ld\n", pid);
void	manage_error(char **argv, long pid)
{
	if (pid > 2147483647 || ft_strlen(argv[1]) > 10)
		exit(write(1, "ERROR: PID invalid!\n", 20));
	if (pid == 0)
		exit(write(1, "ERROR: PID 0\n", 13));
	if (pid == 1)
		exit(write(1, "ERROR: PID 1\n", 13));
	ft_putstr_fd("Sent    : ", 1);
	ft_putnbr_fd(ft_strlen(argv[2]), 1);
	ft_putchar_fd('\n', 1);
	ft_putstr_fd("Received: ", 1);
}

int	main(int argc, char **argv)
{
	int		i;
	long	pid;

	i = 0;
	pid = 0;
	if (argc != 3 || !ft_strlen(argv[2]))
		exit(write(1, "Error: Wrong format!", 20));
	while (argv[1][i] != '\0')
	{
		if (argv[1][i] < '0' || argv[1][i] > '9')
			exit(write(1, "ERROR: PID invalid!\n", 20));
		i++;
	}
	pid = (long)ft_atol(argv[1]);
	manage_error(argv, pid);
	signal(SIGUSR1, action);
	signal(SIGUSR2, action);
	mt_kill(ft_atol(argv[1]), argv[2]);
	while (1)
		pause();
	return (0);
}
