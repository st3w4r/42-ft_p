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
/*
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

		// i = 0;
		// while (buf[i])
		// {
		// 	write(1, buf, 1024);
		// 	i += 100;
		// }
	}

	// r = read(sock, buf, 1024);
		// read = FALSE;
	// write(1, buf, r);
}*/

void		ftp_loop(t_cli_ftp *cli_ftp)
{
	char	*msg;
	char	*line;
	char	**args;
	int		r;
	t_bool	sended;

	sended = TRUE;
	g_need_read = TRUE;
	while (42)
	{
		if (sended && g_need_read)
		{
			msg = ftp_cli_pi_recive_data(cli_ftp->sock_ctl);
			// ft_putstr(msg);
			ftp_receive_msg(msg);
			free(msg);
		}
		sended = FALSE;
		g_need_read = TRUE;
		ftp_display_prompt();
		if ((r = ft_get_next_line(0, &line)) == 0)
		{
			ft_putendl("");
			ftp_cli_builtin_quit(cli_ftp, NULL);
			break ;
		}
		// ftp_cli_pi_cmd(cli_ftp, line);
		args = ft_strsplit(line, ' ');
		sended = ftp_cli_pi_search_builtins(cli_ftp, args);
		FREE_ARR(args);
		// ftp_cli_pi_write(cli_ftp, line);
		// write(sock, line, ft_strlen(line));
		free(line);
	}
}

int				main(int ac, char **av)
{
	t_cli_ftp		cli_ftp;
	struct hostent	*host;
	struct in_addr	*addr;
	// int			port;
	// int			sock;

	if (ac != 3)
		usage(av[0]);
	if ((host = gethostbyname(av[1])) == NULL)
		ft_error_str_exit("Error get IP.\n");
	addr = (struct in_addr*)host->h_addr_list[0];
	cli_ftp.port_ctl = ft_atoi(av[2]);
	cli_ftp.addr_ctl = ft_strdup(inet_ntoa(*addr));
	cli_ftp.sock_ctl = ftp_cli_pi_create(&cli_ftp);
	ftp_loop(&cli_ftp);
	close(cli_ftp.sock_ctl);
	return (0);
}
