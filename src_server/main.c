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

static void		ftp_clean_process(int sig_number)
{
	wait4(-1, 0, WNOHANG, 0);
}

static void		ftp_init_config(t_srv_ftp *srv_ftp)
{
	srv_ftp->config.mode_ftp = MODE_FTP_DEFAULT;
	srv_ftp->config.type = TYPE_DEFAULT;
	srv_ftp->config.path_srv = ftp_srv_fs_get_path();
	srv_ftp->config.login = NULL;
	srv_ftp->config.is_logged = FALSE;
}

int				main(int ac, char **av)
{
	t_srv_ftp	srv_ftp;
	int			port;

	if (ac != 2)
		usage(av[0]);
	signal(SIGCHLD, &ftp_clean_process);
	srv_ftp.sock_data = -1;
	srv_ftp.cs_data = -1;
	srv_ftp.port = ft_atoi(av[1]);
	ftp_init_config(&srv_ftp);
	ftp_create_socket(&srv_ftp);
	return (0);
}
