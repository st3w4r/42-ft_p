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
	cmd.line_send = ftp_create_cmd_line(cmd.name, cmd.args);
	ftp_cli_pi_send_cmd(cli_ftp, cmd);
	free(cmd.line_send);

}

void	ftp_cli_builtin_lcd(t_cli_ftp *cli_ftp, char **args)
{
	char *path;

	if (chdir(args[1]) == 0)
	{
		path = ftp_cli_fs_get_path();
		ft_putstr("Local directory now: ");
		ft_putendl(path);
		free(path);
	}
	g_need_read = FALSE;
}

void	ftp_cli_builtin_pwd(t_cli_ftp *cli_ftp, char **args)
{
	t_cmd_nvt cmd;

	cmd.name = "PWD";
	cmd.args = ++args;
	cmd.line_send = ftp_create_cmd_line(cmd.name, cmd.args);
	ftp_cli_pi_send_cmd(cli_ftp, cmd);
	free(cmd.line_send);
}

void	ftp_cli_builtin_lpwd(t_cli_ftp *cli_ftp, char **args)
{
	char *path;

	path = ftp_cli_fs_get_path();
	ft_putstr("Local directory: ");
	ft_putendl(path);
	free(path);
	g_need_read = FALSE;
}

void	ftp_cli_builtin_ls(t_cli_ftp *cli_ftp, char **args)
{
	t_cmd_nvt	cmd;
	t_res		res;
	char		*response;

	ftp_cli_pi_open_data_channel(cli_ftp);
	cmd.name = "LIST";
	cmd.args = ++args;
	cmd.line_send = ftp_create_cmd_line(cmd.name, cmd.args);
	ftp_cli_pi_send_cmd(cli_ftp, cmd);
	response = ftp_cli_pi_recive_data(cli_ftp->sock_ctl);
	res = ftp_parse_response(response);
	ft_putstr(response);
	if (res.code_res == 150)
		ftp_cli_dtp_read_on_channel(cli_ftp);
	free(res.msg_res);
	free(response);
	free(cmd.line_send);
}

void	ftp_cli_builtin_lls(t_cli_ftp *cli_ftp, char **args)
{
	free(args[0]);
	args[0] = ft_strdup("/bin/ls");
	ftp_fork_process(args);
	g_need_read = FALSE;
}

void	ftp_cli_builtin_get(t_cli_ftp *cli_ftp, char **args)
{
	t_cmd_nvt	cmd;
	t_res		res;
	char		*response;
	char		*data_one;
	int			len;
	int			fd_create;
	int			argc;

	argc = ft_arrlen(args) -1;
	if (argc != 1 && argc != 2)
	{
		ft_putstr("usage: get remote-file [local-file]\n");
		g_need_read = FALSE;
		return ;
	}
	ftp_cli_pi_open_data_channel(cli_ftp);
	cmd.name = "RETR";
	cmd.args = ++args;
	cmd.line_send = ftp_create_cmd_line(cmd.name, cmd.args);
	ftp_cli_pi_send_cmd(cli_ftp, cmd);

	response = ftp_cli_pi_recive_data(cli_ftp->sock_ctl);
	ft_putstr(response);
	res = ftp_parse_response(response);

	if (res.code_res == 150)
	{
		if ((fd_create = ftp_cli_fs_create_file(args[argc - 1])) != -1)
		{
			while ((data_one = ftp_cli_dtp_read_on_channel_one(cli_ftp, &len)))
			{
				ftp_cli_fs_write_in_file(fd_create, data_one, len);
				free(data_one);
			}
			close(fd_create);
		}
	}
	else
		g_need_read = FALSE;
	free(cmd.line_send);
	free(res.msg_res);
	free(response);
}

void	ftp_cli_builtin_put(t_cli_ftp *cli_ftp, char **args)
{
	t_cmd_nvt	cmd;
	t_res		res;
	char		*data;
	int			len_data;
	char		*response;
	int			fd;
	int			argc;

	argc = ft_arrlen(args) -1;
	if (argc != 1 && argc != 2)
	{
		ft_putstr("usage: put remote-file [local-file]\n");
		g_need_read = FALSE;
		return ;
	}
	ftp_cli_pi_open_data_channel(cli_ftp);
	cmd.name = "STOR";
	cmd.args = argc == 2 ? &(args[2]) : &(args[1]);
	cmd.line_send = ftp_create_cmd_line(cmd.name, cmd.args);
	ftp_cli_pi_send_cmd(cli_ftp, cmd);

	response = ftp_cli_pi_recive_data(cli_ftp->sock_ctl);
	ft_putstr(response);
	res = ftp_parse_response(response);
	if (res.code_res == 150)
	{
		if ((fd = ftp_cli_fs_open_file(args[1])) != -1)
		{
			while ((data = ftp_cli_fs_read_file(fd, &len_data)))
			{
				ftp_cli_dtp_send_data(cli_ftp, data, len_data);
				free(data);
			}
			ftp_cli_dtp_close_channel(cli_ftp);
			close(fd);
		}
	}
	else
		g_need_read = FALSE;

	free(cmd.line_send);
	free(res.msg_res);
	free(response);
}

void	ftp_cli_builtin_quit(t_cli_ftp *cli_ftp, char **args)
{
	t_cmd_nvt	cmd;
	char		*msg;

	cmd.name = "QUIT";
	cmd.args = NULL;
	cmd.line_send = ftp_create_cmd_line(cmd.name, cmd.args);
	ftp_cli_pi_send_cmd(cli_ftp, cmd);
	free(cmd.line_send);

	msg = ftp_cli_pi_recive_data(cli_ftp->sock_ctl);
	ft_putstr(msg);
	free(msg);
	close(cli_ftp->sock_ctl);
	exit(0);
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

void	ftp_cli_builtin_binary(t_cli_ftp *cli_ftp, char **args)
{
	t_cmd_nvt cmd;

	cmd.name = "TYPE I";
	cmd.args = ++args;
	cmd.line_send = ftp_create_cmd_line(cmd.name, cmd.args);
	ftp_cli_pi_send_cmd(cli_ftp, cmd);
	free(cmd.line_send);
}

void	ftp_cli_builtin_ascii(t_cli_ftp *cli_ftp, char **args)
{
	t_cmd_nvt cmd;

	cmd.name = "TYPE A";
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
