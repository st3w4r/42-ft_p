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

static char		*ftp_recv_on_socket_eol(t_srv_ftp *srv_ftp)
{
	int		r;
	char	buf[2];
	char	*cmd;
	t_bool	eol;

	eol = FALSE;
	cmd = ft_strdup("");
	while (!eol)
	{
		if ((r = recv(srv_ftp->cs, buf, 1, 0)) < 0)
		{
			ft_error_str("[ERROR RECEIVE]\n");
			return (NULL);
		}
		buf[r] = '\0';
		if (buf[0] == '\0')
			return (NULL);
		if (buf[0] == '\n')
			eol = TRUE;
		cmd = ft_strjoin_free_l(cmd, buf);
	}
	return (cmd);
}

static void		ftp_recv_on_socket(t_srv_ftp *srv_ftp)
{
	char	*cmd;
	char	**args;
	t_bool	find;

	find = FALSE;
	while (42)
	{
		if (!(cmd = ftp_recv_on_socket_eol(srv_ftp)))
			return ;
		ft_strreplace_char(cmd, '\n', '\0');
		ft_strreplace_char(cmd, '\r', '\0');
		args = ft_strsplit(cmd, ' ');
		ftp_srv_ui_display_cmd(srv_ftp, cmd);
		if ((srv_ftp->config.is_logged == FALSE) &&
			(ft_strcmp(ft_str_toupper(args[0]), "USER") != 0) &&
			(ft_strcmp(ft_str_toupper(args[0]), "PASS") != 0) &&
			(ft_strcmp(ft_str_toupper(args[0]), "QUIT") != 0))
			ftp_srv_pi_send_response(srv_ftp, 503, "Login with USER first.");
		else
		{
			find = ftp_srv_pi_search_builtins(srv_ftp, args);
			if (!find)
				ftp_srv_pi_send_response(srv_ftp, 500, "Unknown command.");
		}
	}
}

static int		ftp_create_server(t_srv_ftp *srv_ftp)
{
	int		sock;

	sock = ftp_srv_use_ipv6(srv_ftp->port);
	if (listen(sock, 42) == -1)
		ft_error_str_exit("Listen error\n");
	return (sock);
}

void			ftp_create_socket(t_srv_ftp *srv_ftp)
{
	pid_t				pid;
	unsigned int		cslen;

	srv_ftp->sock = ftp_create_server(srv_ftp);
	while (42)
	{
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
			ftp_srv_ui_display_cmd(srv_ftp, "[DISCONNECTED]");
			close(srv_ftp->sock);
			exit(0);
		}
	}
}
