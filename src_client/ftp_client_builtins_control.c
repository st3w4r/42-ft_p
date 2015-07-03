/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_client_builtins_control.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/07/03 11:00:33 by ybarbier          #+#    #+#             */
/*   Updated: 2015/07/03 11:00:34 by ybarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_cli.h"

void	ftp_cli_builtin_cd(t_cli_ftp *cli_ftp, char **args)
{
	t_cmd_nvt cmd;

	cmd.name = "CWD";
	cmd.args = ++args;
	cmd.line_send = ftp_create_cmd_line(cmd.name, cmd.args);
	ftp_cli_pi_send_cmd(cli_ftp, cmd);
	free(cmd.line_send);
}

void	ftp_cli_builtin_user(t_cli_ftp *cli_ftp, char **args)
{
	t_cmd_nvt cmd;

	cmd.name = "USER";
	cmd.args = ++args;
	cmd.line_send = ftp_create_cmd_line(cmd.name, cmd.args);
	ftp_cli_pi_send_cmd(cli_ftp, cmd);
	free(cmd.line_send);
}

void	ftp_cli_builtin_pass(t_cli_ftp *cli_ftp, char **args)
{
	t_cmd_nvt cmd;

	cmd.name = "PASS";
	cmd.args = ++args;
	cmd.line_send = ftp_create_cmd_line(cmd.name, cmd.args);
	ftp_cli_pi_send_cmd(cli_ftp, cmd);
	free(cmd.line_send);
}

void	ftp_cli_builtin_quit(t_cli_ftp *cli_ftp, char **args)
{
	t_cmd_nvt	cmd;
	char		*msg;

	(void)args;
	cmd.name = "QUIT";
	cmd.args = NULL;
	cmd.line_send = ftp_create_cmd_line(cmd.name, cmd.args);
	ftp_cli_pi_send_cmd(cli_ftp, cmd);
	free(cmd.line_send);
	msg = ftp_cli_pi_recive_data(cli_ftp->sock_ctl);
	ftp_receive_msg(msg);
	free(msg);
	close(cli_ftp->sock_ctl);
	exit(0);
}
