/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_server_builtins_control.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/07/02 22:34:38 by ybarbier          #+#    #+#             */
/*   Updated: 2015/07/02 22:34:38 by ybarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_srv.h"

void	ftp_srv_builtin_user(t_srv_ftp *srv_ftp, char **args)
{
	int		i;

	i = 0;
	while (args && args[1] && g_account[i].username)
	{
		if (ft_strcmp(args[1], g_account[i].username) == 0)
		{
			srv_ftp->config.login = args[1];
			ftp_srv_pi_send_response(srv_ftp, 331, "Please pawword.");
			return ;
		}
		++i;
	}
	srv_ftp->config.login = NULL;
	ftp_srv_pi_send_response(srv_ftp, 530, "Login incorrect.");
}

void	ftp_srv_builtin_pass(t_srv_ftp *srv_ftp, char **args)
{
	int		i;

	i = 0;
	if (srv_ftp->config.login == NULL)
	{
		ftp_srv_pi_send_response(srv_ftp, 503, "Login with USER first.");
		return ;
	}
	while (args && args[1] && g_account[i].password)
	{
		if (ft_strcmp(srv_ftp->config.login, g_account[i].username) == 0 &&
			ft_strcmp(args[1], g_account[i].password) == 0)
		{
			srv_ftp->config.is_logged = TRUE;
			ftp_srv_pi_send_response(srv_ftp, 230, "Login successful.");
			return ;
		}
		++i;
	}
	srv_ftp->config.login = NULL;
	ftp_srv_pi_send_response(srv_ftp, 530, "Password incorrect.");
}

void	ftp_srv_builtin_cd(t_srv_ftp *srv_ftp, char **args)
{
	if (ft_strncmp(args[1], "/", 1) == 0)
	{
		args[1] = ft_strjoin_free_r(srv_ftp->config.path_srv, args[1]);
	}
	if ((ft_arrlen(args) == 2) &&
		(ftp_srv_fs_path_allow(srv_ftp, args[1]) == TRUE) &&
		(chdir(args[1]) == 0))
	{
		ftp_srv_pi_send_response(srv_ftp, 250,
			"Directory successfully changed.");
		return ;
	}
	ftp_srv_pi_send_response(srv_ftp, 550, "Failed to change directory.");
}

void	ftp_srv_builtin_quit(t_srv_ftp *srv_ftp, char **args)
{
	(void)args;
	ftp_srv_pi_send_response(srv_ftp, 221, "Goodbye.");
	close(srv_ftp->cs);
	close(srv_ftp->sock);
}
