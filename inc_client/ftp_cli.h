/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_cli.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/15 18:32:47 by ybarbier          #+#    #+#             */
/*   Updated: 2015/05/15 18:32:48 by ybarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FTP_CLI_H
# define FTP_CLI_H

# include "libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/socket.h>
# include <netdb.h>
# include <netinet/in.h>
# include <arpa/inet.h>

# define TRUE 1
# define FALSE 0

typedef unsigned char	t_bool;

typedef struct	s_cli_ftp
{
	char	*addr;
	int		port;
	int		sock;
}				t_cli_ftp;


typedef struct	s_cmd_nvt
{
	char	*name;
	char	**args;
	char	*line_send;
}				t_cmd_nvt;
/*
typedef t_cmd_nvt	s_cmd_list[] =
{
	{ "LIST",	NULL },
	{ "CWD",	NULL },
	{ "PWD",	NULL },
	{ "RETR",	NULL },
	{ "STOR",	NULL },
	{ "QUIT",	NULL },
	{ 0 }
}				t_cmd_nvt_list;
*/

/*
** Name: ftp_client_ui
** File: ftp_client_ui.c
** Desc: Fucntions User Interface
*/



/*
** Name: ftp_client_pi
** File: ftp_client_pi.c
** Desc: Fucntions start client, Protocol Interpreteur
*/
// void	ftp_cli_pi_write(t_cli_ftp *cli_ftp, char *line);
// void	ftp_cli_pi_send_cmd(t_cli_ftp *cli_ftp, char *line);
void	ftp_cli_pi_send_cmd(t_cli_ftp *cli_ftp, t_cmd_nvt cmd);
t_bool	ftp_cli_pi_search_builtins(t_cli_ftp *cli_ftp, char **agrs);
int		ftp_cli_pi_create(t_cli_ftp *cli_ftp);
// int		ftp_create_client(char *addr, int port);

/*
** Name: ftp_client_builtins
** File: ftp_client_builtins.c
** Desc: All builtins clients
*/
void	ftp_cli_builtin_cd(t_cli_ftp *cli_ftp, char **args);
void	ftp_cli_builtin_pwd(t_cli_ftp *cli_ftp, char **args);
void	ftp_cli_builtin_ls(t_cli_ftp *cli_ftp, char **args);
void	ftp_cli_builtin_get(t_cli_ftp *cli_ftp, char **args);
void	ftp_cli_builtin_put(t_cli_ftp *cli_ftp, char **args);
void	ftp_cli_builtin_quit(t_cli_ftp *cli_ftp, char **args);




/*
** Name: ftp_helper
** File: ftp_helper.c
** Desc: Fucntions helpers
*/
// void	ftp_display_prompt(void);
char	*ftp_create_cmd_line(char *name, char **args);
void	ftp_receive_msg(t_bool done);

/*
** Name: Struct of commands user, list in a global array.
*/
typedef	void	(*t_builtin_func)(t_cli_ftp *, char **);

typedef struct	s_cmd_cli
{
	char			*name;
	t_builtin_func	builtin;
}				t_cmd_cli;

static t_cmd_cli	g_cmd_cli_list[] =
{
	{"ls", ftp_cli_builtin_ls},
	{"pwd", ftp_cli_builtin_pwd},
	{"cd", ftp_cli_builtin_cd},
	{"get", ftp_cli_builtin_get},
	{"put", ftp_cli_builtin_put},
	{"quit", ftp_cli_builtin_quit},
	{0}
};

#endif
