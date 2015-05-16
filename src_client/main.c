/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/16 13:36:01 by ybarbier          #+#    #+#             */
/*   Updated: 2015/05/16 13:36:02 by ybarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_cli.h"

void	usage(char *str)
{
	ft_putstr("Usage: ");
	ft_putstr(str);
	ft_putstr(" <addr> <port>\n");
	exit(1);
}

int		main(int ac, char **av)
{
	int port;
	int sock;
	char buf[1024];
	int r;

	if (ac != 3)
		usage(av[0]);
	port = ft_atoi(av[2]);
	sock = ftp_create_client(av[1], port);

	while ((r = read(STDIN_FILENO, buf, 1023)) > 0)
	{
		// write(STDIN_FILENO, "FT_P $> ", 8);
		buf[r] = '\0';
		write(sock, buf, r);
	}
	close(sock);
}
