/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_server_builtins_service.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/07/02 19:24:29 by ybarbier          #+#    #+#             */
/*   Updated: 2015/07/02 19:24:30 by ybarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_srv.h"

static char	**ftp_srv_builtin_ls_init(t_srv_ftp *srv_ftp, char **args)
{
	char	**new_args;
	char	*flags;
	char	*path;
	int		pos;

	new_args = (malloc(sizeof(char *) * 4));
	new_args[0] = ft_strdup("/bin/ls");
	if (ft_strchr(args[1], '-') != NULL)
		if (ft_strchr(args[1], 'a') != NULL)
			flags = ft_strdup("-na");
		else
			flags = ft_strdup("-n");
	else
		flags = ft_strdup("-n");
	pos = ft_arrlen(args) - 1;
	if (ftp_srv_fs_path_allow(srv_ftp, args[pos]) == TRUE)
		path = ft_strdup(args[pos]);
	else
		path = ft_strdup(".");
	new_args[1] = flags;
	new_args[2] = path;
	new_args[3] = NULL;
	return (new_args);
}

static void	ftp_srv_builtin_ls_pipe_read(t_srv_ftp *srv_ftp, int pfd)
{
	char	buffer[BUFF_SIZE_READ + 1];
	char	*new_buf;
	int		r;

	while ((r = read(pfd, buffer, BUFF_SIZE_READ)) != 0)
	{
		buffer[r] = 0;
		new_buf = ftp_srv_crlf(buffer, UNIX, DOS);
		ftp_srv_dtp_send_data(srv_ftp, new_buf, ft_strlen(new_buf));
		free(new_buf);
	}
}

static void	ftp_srv_builtin_ls_pipe(t_srv_ftp *srv_ftp, char **args)
{
	int		pfd[2];
	int		pid;

	if (pipe(pfd) == -1)
		ft_error_str("Error pipe failed.\n");
	if ((pid = fork()) < 0)
		ft_error_str("Error fork failed.\n");
	if (pid != 0)
	{
		close(pfd[1]);
		ftp_srv_builtin_ls_pipe_read(srv_ftp, pfd[0]);
		close(pfd[0]);
	}
	else
	{
		close(pfd[0]);
		ftp_redirect_fd(pfd[1], STDOUT_FILENO);
		if (execv(args[0], args) == -1)
			ft_error_str("Exec format error.\n");
		close(pfd[1]);
		exit(0);
	}
}

void		ftp_srv_builtin_ls(t_srv_ftp *srv_ftp, char **args)
{
	char	**new_args;

	if (srv_ftp->sock_data != -1)
	{
		ftp_srv_dtp_accept_connection(srv_ftp);
		ftp_srv_pi_send_response(srv_ftp, 150,
			"Here comes the directory listing.");
		new_args = ftp_srv_builtin_ls_init(srv_ftp, args);
		ftp_srv_builtin_ls_pipe(srv_ftp, new_args);
		ftp_srv_dtp_close_channel(srv_ftp);
		ftp_srv_pi_send_response(srv_ftp, 226, "Directory send OK");
		FREE_ARR(new_args);
	}
	else
		ftp_srv_pi_send_response(srv_ftp, 425, "Use PORT or PASV first.");
}

void		ftp_srv_builtin_noop(t_srv_ftp *srv_ftp, char **args)
{
	(void)args;
	ftp_srv_pi_send_response(srv_ftp, 200, "NOOP ok.");
}
