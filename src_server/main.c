/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/16 13:30:32 by ybarbier          #+#    #+#             */
/*   Updated: 2015/05/16 13:30:33 by ybarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_srv.h"

static void		usage(char *str)
{
	ft_putstr("Usage: ");
	ft_putstr(str);
	ft_putstr(" <port>\n");
	exit(1);
}

int				main(int ac, char **av)
{
	t_srv_ftp srv_ftp;
	int		port;

	if (ac != 2)
		usage(av[0]);
	srv_ftp.sock_data = -1;
	srv_ftp.cs_data = -1;
	srv_ftp.mode_ftp = MODE_FTP_DEFAULT;
	srv_ftp.type = TYPE_DEFAULT;
	srv_ftp.port = ft_atoi(av[1]);
	ftp_create_socket(&srv_ftp);
	return (0);
}
