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

static void		usage(char *str)
{
	ft_putstr("Usage: ");
	ft_putstr(str);
	ft_putstr(" <addr> <port>\n");
	exit(1);
}

static void		ftp_loop(int sock)
{
	char *line;
	int r;

	while (42)
	{
		ftp_display_prompt();
		if ((r = ft_get_next_line(0, &line)) == 0)
			break ;
		write(sock, line, ft_strlen(line));
		free(line);
	}
	ft_putendl("Exit");
}

int				main(int ac, char **av)
{
	int port;
	int sock;

	if (ac != 3)
		usage(av[0]);
	port = ft_atoi(av[2]);
	sock = ftp_create_client(av[1], port);
	ftp_loop(sock);
	close(sock);
	return (0);
}
