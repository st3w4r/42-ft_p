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
		if (ft_strcmp(args[0], g_cmd_nvt_list[i].name) == 0)
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
		args = ft_strsplit(buf, ' ');
		ftp_srv_pi_search_builtins(srv_ftp, args);
		// ftp_fork_process(args[0], args);
		ft_arrfree(&args);
		// send(srv_ftp->cs, NULL, r, 0);
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
	struct sockaddr_in	csin;

	srv_ftp->sock = ftp_create_server(srv_ftp->port);
	while (42)
	{
		srv_ftp->cs = accept(srv_ftp->sock, (struct sockaddr*)&csin,  &cslen);
		// ftp_redirect_fd(cs, STDOUT_FILENO);
		// ftp_redirect_fd(cs, STDERR_FILENO);
		pid = fork();
		if (pid > 0)
		{
			close(srv_ftp->cs);
		}
		else if (pid == 0)
		{
			// ftp_recv_on_socket(srv_ftp);
			ftp_read_on_socket(srv_ftp);
			close(srv_ftp->sock);
			exit(0);
		}
		// send(cs, "ok1234567890", 7, 0);
	}
}
