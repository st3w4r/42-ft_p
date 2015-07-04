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

# define FREE_ARR(x) { if (x) ft_arrfree(&x); }

# define UNIX 1
# define DOS 2

# define SRV_CONF DOS
# define CLI_CONF UNIX

# define BUFF_SIZE_READ 1024
# define BUFF_SIZE_RECV 1024

typedef unsigned char	t_bool;

typedef struct	s_res {
	int			code;
	char		*msg;
}				t_res;

typedef struct	s_cli_ftp
{
	char			*addr_ctl;
	char			*addr_data;
	int				port_ctl;
	int				port_data;
	int				sock_ctl;
	int				sock_data;
	struct hostent	*host;
}				t_cli_ftp;

typedef struct	s_cmd_nvt
{
	char	*name;
	char	**args;
	char	*line_send;
}				t_cmd_nvt;

/*
** Name: ftp_client_ui
** File: ftp_client_ui.c
** Desc: Fucntions User Interface
*/

int		g_need_read;

/*
** Name: ftp_client_pi
** File: ftp_client_pi.c
** Desc: Fucntions start client, Protocol Interpreteur
*/
void			ftp_cli_pi_send_cmd(t_cli_ftp *cli_ftp, t_cmd_nvt cmd);
t_bool			ftp_cli_pi_search_builtins(t_cli_ftp *cli_ftp, char **agrs);
int				ftp_cli_pi_create(t_cli_ftp *cli_ftp);
void			ftp_cli_pi_open_data_channel(t_cli_ftp *cli_ftp);
char			*ftp_cli_pi_recive_data(int sock);
int				ftp_cli_use_ipv4(t_cli_ftp *cli_ftp, int port);
int				ftp_cli_use_ipv6(t_cli_ftp *cli_ftp, int port);

/*
** Name: ftp_client_dtp
** File: ftp_client_dtp.c
** Desc: Function of Data Channel
*/
void			ftp_cli_dtp_send_data(t_cli_ftp *cli_ftp, char *data, int len);
char			*ftp_cli_dtp_read_on_channel_one(t_cli_ftp *cli_ftp, int *len);
void			ftp_cli_dtp_read_on_channel(t_cli_ftp *cli_ftp);
int				ftp_cli_dtp_create_channel(t_cli_ftp *cli_ftp);
void			ftp_cli_dtp_close_channel(t_cli_ftp *cli_ftp);

/*
** Name: ftp_client_file
** File: ftp_client_file.c
** Desc: Function of File System
*/
int				ftp_cli_fs_open_file(char *name);
int				ftp_cli_fs_create_file(char *name);
t_bool			ftp_cli_fs_write_in_file(int fd, char *data, int len);
char			*ftp_cli_fs_read_file(int fd, int *len);
char			*ftp_cli_fs_get_path(void);

/*
** Name: ftp_client_builtins
** File: ftp_client_builtins.c
** Desc: All builtins clients
*/
void			ftp_cli_builtin_cd(t_cli_ftp *cli_ftp, char **args);
void			ftp_cli_builtin_lcd(t_cli_ftp *cli_ftp, char **args);
void			ftp_cli_builtin_pwd(t_cli_ftp *cli_ftp, char **args);
void			ftp_cli_builtin_lpwd(t_cli_ftp *cli_ftp, char **args);
void			ftp_cli_builtin_ls(t_cli_ftp *cli_ftp, char **args);
void			ftp_cli_builtin_lls(t_cli_ftp *cli_ftp, char **args);
void			ftp_cli_builtin_get(t_cli_ftp *cli_ftp, char **args);
void			ftp_cli_builtin_put(t_cli_ftp *cli_ftp, char **args);
void			ftp_cli_builtin_quit(t_cli_ftp *cli_ftp, char **args);
void			ftp_cli_builtin_user(t_cli_ftp *cli_ftp, char **args);
void			ftp_cli_builtin_pass(t_cli_ftp *cli_ftp, char **args);
void			ftp_cli_builtin_binary(t_cli_ftp *cli_ftp, char **args);
void			ftp_cli_builtin_ascii(t_cli_ftp *cli_ftp, char **args);
void			ftp_cli_builtin_size(t_cli_ftp *cli_ftp, char **args);
void			ftp_cli_builtin_mkdir(t_cli_ftp *cli_ftp, char **args);
void			ftp_cli_builtin_rmdir(t_cli_ftp *cli_ftp, char **args);
void			ftp_cli_builtin_delete(t_cli_ftp *cli_ftp, char **args);

/*
** Name: ftp_server_conversion
** File: ftp_server_conversion.c
** Desc: Function conversion unix dos
*/
char			*ftp_cli_crlf(char *data, int from, int to);

/*
** Name: ftp_client_helper
** File: ftp_client_helper.c
** Desc: Fucntions helpers
*/
void			ftp_receive_msg(char *msg);
t_res			ftp_parse_response(char *response);
char			*ftp_create_cmd_line(char *name, char **args);
void			ftp_parse_pasv_addr_port(t_cli_ftp *cli_ftp, char *msg);
void			ftp_parse_epsv_port(t_cli_ftp *cli_ftp, char *msg);
void			ftp_fork_process(char **av);

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
	{"lls", ftp_cli_builtin_lls},
	{"pwd", ftp_cli_builtin_pwd},
	{"lpwd", ftp_cli_builtin_lpwd},
	{"cd", ftp_cli_builtin_cd},
	{"lcd", ftp_cli_builtin_lcd},
	{"get", ftp_cli_builtin_get},
	{"put", ftp_cli_builtin_put},
	{"quit", ftp_cli_builtin_quit},
	{"user", ftp_cli_builtin_user},
	{"pass", ftp_cli_builtin_pass},
	{"binary", ftp_cli_builtin_binary},
	{"ascii", ftp_cli_builtin_ascii},
	{"size", ftp_cli_builtin_size},
	{"mkdir", ftp_cli_builtin_mkdir},
	{"rmdir", ftp_cli_builtin_rmdir},
	{"delete", ftp_cli_builtin_delete},
	{0, 0}
};

#endif
