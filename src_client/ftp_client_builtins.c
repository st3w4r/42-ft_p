/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_client_builtins.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/22 14:09:28 by ybarbier          #+#    #+#             */
/*   Updated: 2015/05/22 14:09:29 by ybarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_cli.h"

void	ftp_cli_builtin_cd(t_cli_ftp *cli_ftp, char **args)
{
	t_cmd_nvt cmd;

	cmd.name = "CWD";
	cmd.args = ++args;
	ftp_cli_pi_send_cmd(cli_ftp, cmd);
}

void	ftp_cli_builtin_pwd(t_cli_ftp *cli_ftp, char **args)
{
	t_cmd_nvt cmd;

	cmd.name = "PWD";
	cmd.args = ++args;
	ftp_cli_pi_send_cmd(cli_ftp, cmd);
}

void	ftp_cli_builtin_ls(t_cli_ftp *cli_ftp, char **args)
{
	t_cmd_nvt cmd;

	cmd.name = "LIST";
	cmd.args = ++args;
	ftp_cli_pi_send_cmd(cli_ftp, cmd);
}

void	ftp_cli_builtin_get(t_cli_ftp *cli_ftp, char **args)
{
	t_cmd_nvt cmd;

	cmd.name = "RETR";
	cmd.args = ++args;
	ftp_cli_pi_send_cmd(cli_ftp, cmd);
}

void	ftp_cli_builtin_put(t_cli_ftp *cli_ftp, char **args)
{
	t_cmd_nvt cmd;

	cmd.name = "STOR";
	cmd.args = ++args;
	ftp_cli_pi_send_cmd(cli_ftp, cmd);
}

void	ftp_cli_builtin_quit(t_cli_ftp *cli_ftp, char **args)
{
	t_cmd_nvt cmd;

	cmd.name = "QUIT";
	cmd.args = ++args;
	ftp_cli_pi_send_cmd(cli_ftp, cmd);
}
