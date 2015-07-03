/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_client_builtins_local.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/07/03 11:03:45 by ybarbier          #+#    #+#             */
/*   Updated: 2015/07/03 11:03:46 by ybarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_cli.h"

void	ftp_cli_builtin_lcd(t_cli_ftp *cli_ftp, char **args)
{
	char *path;

	(void)cli_ftp;
	if (chdir(args[1]) == 0)
	{
		path = ftp_cli_fs_get_path();
		ft_putstr("Local directory now: ");
		ft_putendl(path);
		free(path);
	}
	g_need_read = FALSE;
}

void	ftp_cli_builtin_lpwd(t_cli_ftp *cli_ftp, char **args)
{
	char *path;

	(void)cli_ftp;
	(void)args;
	path = ftp_cli_fs_get_path();
	ft_putstr("Local directory: ");
	ft_putendl(path);
	free(path);
	g_need_read = FALSE;
}

void	ftp_cli_builtin_lls(t_cli_ftp *cli_ftp, char **args)
{
	(void)cli_ftp;
	free(args[0]);
	args[0] = ft_strdup("/bin/ls");
	ftp_fork_process(args);
	g_need_read = FALSE;
}
