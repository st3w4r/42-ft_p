/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_server_builtins.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/24 18:23:26 by ybarbier          #+#    #+#             */
/*   Updated: 2015/05/24 18:23:28 by ybarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_srv.h"

void	ftp_srv_builtin_cd(t_srv_ftp *srv_ftp, char **args)
{
	ft_putstr_fd("\n", 1);
	while (args && args[0])
	{
		ft_putstr_fd(args[0], 1);
		++args;
	}


	// t_cmd_nvt cmd;

	// cmd.name = "CWD";
	// cmd.args = ++args;
	// cmd.line_send = ftp_create_cmd_line(cmd.name, cmd.args);
	// ftp_srv_pi_send_cmd(srv_ftp, cmd);
	// free(cmd.line_send);

}

void	ftp_srv_builtin_pwd(t_srv_ftp *srv_ftp, char **args)
{
	ft_putstr_fd("\n", 1);
	while (args && args[0])
	{
		ft_putstr_fd(args[0], 1);
		++args;

	}
	// send(srv_ftp->cs, "\r\n", 2, 0);

	// t_cmd_nvt cmd;

	// cmd.name = "PWD";
	// cmd.args = ++args;
	// cmd.line_send = ftp_create_cmd_line(cmd.name, cmd.args);
	// ftp_srv_pi_send_cmd(srv_ftp, cmd);
	// free(cmd.line_send);
}

void	ftp_srv_builtin_ls(t_srv_ftp *srv_ftp, char **args)
{
	int fd;
	// char buf[1024];
	// char *str;
	// int r;
	// args[0] = "/bin/ls";
	free(*args);
	args[0] = ft_strdup("/bin/ls");
	// args++;
	// fd = open("out", O_RDONLY);
	ftp_redirect_fd(srv_ftp->cs, STDOUT_FILENO);
	ftp_redirect_fd(srv_ftp->cs, STDERR_FILENO);
	// dup2(srv_ftp->cs, STDOUT_FILENO);
	// dup2(srv_ftp->cs, STDERR_FILENO);
	ftp_fork_process("/bin/ls", args);
	// dup2(1, STDOUT_FILENO);

	ftp_redirect_fd(STDIN_FILENO, STDOUT_FILENO);
	ftp_redirect_fd(STDIN_FILENO, STDERR_FILENO);

	// puts("IN");
	// ftp_redirect_fd(STDIN_FILENO, STDERR_FILENO);

	// ftp_redirect_fd(STDERR_FILENO, 2);

	// write(srv_ftp->cs, "\r\n", 2);
	// send(srv_ftp->cs, "\r\n", 2, 0);

	// send(srv_ftp->cs, "Error\n\r", 5)
	// send(srv_ftp->cs, NULL, 1024, 0);
	// str = ft_strdup("test");
	/*
	while ((r = read(srv_ftp->cs, buf, 254)) > 0)
	{
		buf[r] = '\0';

		// ft_putstr(buf);
		ft_putnbr(r);
	}
	ft_putendl("END");
*/


/*
	ft_putstr_fd("\n", 1);
	while (args && args[0])
	{
		ft_putstr_fd(args[0], 1);
		++args;

	}*/

	// t_cmd_nvt cmd;
	//
	// cmd.name = "LIST";
	// cmd.args = ++args;
	// cmd.line_send = ftp_create_cmd_line(cmd.name, cmd.args);
	// ftp_srv_pi_send_cmd(srv_ftp, cmd);
	// free(cmd.line_send);
}

void	ftp_srv_builtin_get(t_srv_ftp *srv_ftp, char **args)
{
	ft_putstr_fd("\n", 1);
	while (args && args[0])
	{
		ft_putstr_fd(args[0], 1);
		++args;
	}
	// t_cmd_nvt cmd;
	//
	// cmd.name = "RETR";
	// cmd.args = ++args;
	// cmd.line_send = ftp_create_cmd_line(cmd.name, cmd.args);
	// ftp_srv_pi_send_cmd(srv_ftp, cmd);
	// free(cmd.line_send);
}

void	ftp_srv_builtin_put(t_srv_ftp *srv_ftp, char **args)
{
	ft_putstr_fd("\n", 1);
	while (args && args[0])
	{
		ft_putstr_fd(args[0], 1);
		++args;
	}
	// t_cmd_nvt cmd;
	//
	// cmd.name = "STOR";
	// cmd.args = ++args;
	// cmd.line_send = ftp_create_cmd_line(cmd.name, cmd.args);
	// ftp_srv_pi_send_cmd(srv_ftp, cmd);
	// free(cmd.line_send);
}

void	ftp_srv_builtin_quit(t_srv_ftp *srv_ftp, char **args)
{
	ft_putstr_fd("\n", 1);
	while (args && args[0])
	{
		ft_putstr_fd(args[0], 1);
		++args;
	}
	// t_cmd_nvt cmd;
	//
	// cmd.name = "QUIT";
	// cmd.args = ++args;
	// cmd.line_send = ftp_create_cmd_line(cmd.name, cmd.args);
	// ftp_srv_pi_send_cmd(srv_ftp, cmd);
	// free(cmd.line_send);
}

void	ftp_srv_builtin_user(t_srv_ftp *srv_ftp, char **args)
{
	ft_putstr_fd("\n", 1);
	while (args && args[0])
	{
		ft_putstr_fd(args[0], 1);
		++args;
	}
	// t_cmd_nvt cmd;
	//
	// cmd.name = "USER";
	// cmd.args = ++args;
	// cmd.line_send = ftp_create_cmd_line(cmd.name, cmd.args);
	// ftp_srv_pi_send_cmd(srv_ftp, cmd);
	// free(cmd.line_send);
}

void	ftp_srv_builtin_pass(t_srv_ftp *srv_ftp, char **args)
{
	ft_putstr_fd("\n", 1);
	while (args && args[0])
	{
		ft_putstr_fd(args[0], 1);
		++args;
	}
	// t_cmd_nvt cmd;
	//
	// cmd.name = "USER";
	// cmd.args = ++args;
	// cmd.line_send = ftp_create_cmd_line(cmd.name, cmd.args);
	// ftp_srv_pi_send_cmd(srv_ftp, cmd);
	// free(cmd.line_send);
}
