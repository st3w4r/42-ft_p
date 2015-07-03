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

void	ftp_cli_builtin_pwd(t_cli_ftp *cli_ftp, char **args)
{
	t_cmd_nvt cmd;

	cmd.name = "PWD";
	cmd.args = ++args;
	cmd.line_send = ftp_create_cmd_line(cmd.name, cmd.args);
	ftp_cli_pi_send_cmd(cli_ftp, cmd);
	free(cmd.line_send);
}

void	ftp_cli_builtin_size(t_cli_ftp *cli_ftp, char **args)
{
	t_cmd_nvt cmd;

	cmd.name = "SIZE";
	cmd.args = ++args;
	cmd.line_send = ftp_create_cmd_line(cmd.name, cmd.args);
	ftp_cli_pi_send_cmd(cli_ftp, cmd);
	free(cmd.line_send);
}

void	ftp_cli_builtin_mkdir(t_cli_ftp *cli_ftp, char **args)
{
	t_cmd_nvt cmd;

	cmd.name = "MKD";
	cmd.args = ++args;
	cmd.line_send = ftp_create_cmd_line(cmd.name, cmd.args);
	ftp_cli_pi_send_cmd(cli_ftp, cmd);
	free(cmd.line_send);
}

void	ftp_cli_builtin_rmdir(t_cli_ftp *cli_ftp, char **args)
{
	t_cmd_nvt cmd;

	cmd.name = "RMD";
	cmd.args = ++args;
	cmd.line_send = ftp_create_cmd_line(cmd.name, cmd.args);
	ftp_cli_pi_send_cmd(cli_ftp, cmd);
	free(cmd.line_send);
}

void	ftp_cli_builtin_delete(t_cli_ftp *cli_ftp, char **args)
{
	t_cmd_nvt cmd;

	cmd.name = "DELE";
	cmd.args = ++args;
	cmd.line_send = ftp_create_cmd_line(cmd.name, cmd.args);
	ftp_cli_pi_send_cmd(cli_ftp, cmd);
	free(cmd.line_send);
}
