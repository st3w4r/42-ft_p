/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_server_pi.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/19 15:35:58 by ybarbier          #+#    #+#             */
/*   Updated: 2015/05/19 15:36:01 by ybarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_srv.h"

void	ftp_srv_pi_send_response(t_srv_ftp *srv_ftp, int code, char *msg)
{
	char *res;

	res = ft_itoa(code);
	res = ft_strjoin_free_l(res, " ");
	res = ft_strjoin_free_l(res, msg);
	res = ft_strjoin_free_l(res, "\r\n");
	send(srv_ftp->cs, res, ft_strlen(res), 0);
}

static t_bool	ftp_srv_pi_search_builtins(t_srv_ftp *srv_ftp, char **args)
{
	t_bool	state;
	int		i;

	i = 0;
	state = FALSE;
	while (args && g_cmd_nvt_list[i].name && state == FALSE)
	{
		if (ft_strcmp(ft_str_toupper(args[0]), g_cmd_nvt_list[i].name) == 0)
		{
			g_cmd_nvt_list[i].builtin(srv_ftp, args);
			state = TRUE;
		}
		++i;
	}
	return (state);
}

static void		ftp_read_on_socket(t_srv_ftp *srv_ftp)
{
	char	*path;
	char	**args;
	char	buf[1024];
	int		r;

	while ((r = read(srv_ftp->cs, buf, 1023)) > 0)
	{
		buf[r] = '\0';
		// puts("EOL");
		// ft_putnbr(r);

		ft_strreplace_char(buf, '\n', '\0');
		ft_strreplace_char(buf, '\r', '\0');
		// ft_strreplace_char(buf, '\n', '\0');
		args = ft_strsplit(buf, ' ');
		// puts(args[0]);
		ftp_srv_pi_search_builtins(srv_ftp, args);
		send(srv_ftp->cs, "\r\n", 2, 0);
		// ftp_fork_process(args[0], args);
		ft_arrfree(&args);
		// send(srv_ftp->cs, NULL, r, 0);
	}
}

static void		ftp_recv_on_socket(t_srv_ftp *srv_ftp)
{
	int		r;
	char	buf[2];
	char	*cmd;
	char	**args;
	t_bool	eol;
	t_bool	find;

	find = FALSE;
	while (42)
	{
		eol = FALSE;
		cmd = ft_strdup("");
		while (!eol)
		{
			if ((r = recv(srv_ftp->cs, buf, 1, 0)) < 0)
			{
				ft_error_str("[ERROR RECEIVE]\n");
				return ;
			}
			buf[r] = '\0';
			if (buf[0] == '\0')
				return ;
			if (buf[0] == '\n')
				eol = TRUE;
			cmd = ft_strjoin_free_l(cmd, buf);
		}
	ft_strreplace_char(cmd, '\n', '\0');
	ft_strreplace_char(cmd, '\r', '\0');
	args = ft_strsplit(cmd, ' ');
	ftp_srv_ui_display_cmd(srv_ftp, cmd);
	find = ftp_srv_pi_search_builtins(srv_ftp, args);
	if (!find)
		ftp_srv_pi_send_response(srv_ftp, 500, "Unknown command.");
	// send(srv_ftp->cs, "\r\n", 2, 0);
	}
}

static int		ftp_create_server(int port)
{
	int					sock;
	struct protoent		*proto;
	struct sockaddr_in	sin;

	proto = getprotobyname("tcp");
	if (proto == 0)
		return (-1);

	sock = socket(PF_INET, SOCK_STREAM, proto->p_proto);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sock, (const struct sockaddr *)&sin, sizeof(sin)) == -1)
		ft_error_str_exit("Bind error\n");
	if (listen(sock, 42) == -1)
		ft_error_str_exit("Listen error\n");
	return(sock);
}

void			ftp_create_socket(t_srv_ftp *srv_ftp)
{
	pid_t				pid;
	// int					sock;
	// int					cs;
	unsigned int		cslen;
	// struct sockaddr_in	csin;

	srv_ftp->sock = ftp_create_server(srv_ftp->port);
	while (42)
	{
		// ftp_redirect_fd(cs, STDOUT_FILENO);
		// ftp_redirect_fd(cs, STDERR_FILENO);
		cslen = sizeof(srv_ftp->csin);
		srv_ftp->cs = accept(srv_ftp->sock, (struct sockaddr*)&(srv_ftp->csin),
							&cslen);
		ftp_srv_ui_display_cmd(srv_ftp, "[CONNECTED]");
		ftp_srv_pi_send_response(srv_ftp, 220, "Server ftp by BY");
		pid = fork();
		if (pid > 0)
		{
			close(srv_ftp->cs);
		}
		else if (pid == 0)
		{
			ftp_recv_on_socket(srv_ftp);
			// ftp_read_on_socket(srv_ftp);
			ftp_srv_ui_display_cmd(srv_ftp, "[DISCONNECTED]");
			close(srv_ftp->sock);
			exit(0);
		}

		// send(cs, "ok1234567890", 7, 0);
	}
}
