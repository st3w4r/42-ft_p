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
			flags = ft_strdup("-la");
		else
			flags = ft_strdup("-l");
	else
		flags = ft_strdup("-l");
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

void		ftp_srv_builtin_ls(t_srv_ftp *srv_ftp, char **args)
{
	char	**new_args;

	if (srv_ftp->sock_data != -1)
	{
		ftp_srv_dtp_accept_connection(srv_ftp);
		ftp_srv_pi_send_response(srv_ftp, 150,
			"Here comes the directory listing.");
		new_args = ftp_srv_builtin_ls_init(srv_ftp, args);
		ftp_redirect_fd(srv_ftp->cs_data, STDOUT_FILENO);
		ftp_redirect_fd(srv_ftp->cs_data, STDERR_FILENO);
		ftp_fork_process(new_args);
		ftp_redirect_fd(STDIN_FILENO, STDOUT_FILENO);
		ftp_redirect_fd(STDIN_FILENO, STDERR_FILENO);
		ftp_srv_dtp_close_channel(srv_ftp);
		ftp_srv_pi_send_response(srv_ftp, 226, "Directory send OK");
		FREE_ARR(new_args);
	}
	else
		ftp_srv_pi_send_response(srv_ftp, 425, "Use PORT or PASV first.");
}
