/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_client_ui.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/19 15:41:06 by ybarbier          #+#    #+#             */
/*   Updated: 2015/05/19 15:41:07 by ybarbier         ###   ########.fr       */
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

static void		ftp_display_prompt(void)
{
	ft_putstr("\033[1;37m");
	ft_putstr("FTP $> ");
	ft_putstr("\033[0m");
}

static void		ftp_loop_write(int sock)
{
	char	buf[2];
	int		r;
	int i;
	t_bool	eol;
	t_bool	cr;

	eol = FALSE;
	cr = FALSE;
	while (!eol)
	{
		if ((r = recv(sock, buf, 1, 0)) < 0)
		{
			ft_error_str("Receive error\n");
			break;
		}
		// ft_putnbr(recv(sock, buf, 1023, 0));

		// ft_putnbr(r);
		// ft_bzero(buf, 2);
		buf[r] = '\0';
		if (buf[0] == '\r')
			cr = TRUE;
		else if (cr && buf[0] == '\n')
			eol = TRUE;
		else if (cr && buf[0] != '\n')
			cr = FALSE;
		ft_putstr(buf);
		/*
		i = 0;
		while (buf[i])
		{
			write(1, buf, 1024);
			i += 100;
		}*/
	}

	// r = read(sock, buf, 1024);
		// read = FALSE;
	// write(1, buf, r);
}

static void		ftp_loop(t_cli_ftp *cli_ftp)
{
	char	*line;
	char	**args;
	int		r;
	t_bool	sended;

	sended = TRUE;
	while (42)
	{
		if (sended)
			ftp_loop_write(cli_ftp->sock);
		sended = FALSE;
		ftp_display_prompt();
		if ((r = ft_get_next_line(0, &line)) == 0)
			break ;
		// ftp_cli_pi_cmd(cli_ftp, line);
		args = ft_strsplit(line, ' ');
		sended = ftp_cli_pi_search_builtins(cli_ftp, args);
		FREE_ARR(args);
		// ftp_cli_pi_write(cli_ftp, line);
		// write(sock, line, ft_strlen(line));
		free(line);
	}
	ft_putendl("Exit");
}

int				main(int ac, char **av)
{
	t_cli_ftp	cli_ftp;
	// int			port;
	// int			sock;

	if (ac != 3)
		usage(av[0]);
	cli_ftp.port = ft_atoi(av[2]);
	cli_ftp.addr = ft_strdup(av[1]);

	cli_ftp.sock = ftp_cli_pi_create(&cli_ftp);
	ftp_loop(&cli_ftp);
	// sock = ftp_create_client(av[1], port);

	// ftp_loop(sock);
	close(cli_ftp.sock);
	return (0);
}
