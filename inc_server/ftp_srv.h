/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftp_srv.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybarbier <ybarbier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/05/15 18:33:05 by ybarbier          #+#    #+#             */
/*   Updated: 2015/05/15 18:33:08 by ybarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FTP_SRV_H
# define FTP_SRV_H

# include "libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/socket.h>
# include <netdb.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <ifaddrs.h>


# define TRUE 1
# define FALSE 0

# define FREE_ARR(x) { if (x) ft_arrfree(&x); }

# define TYPE_DEFAULT BINARY
# define MODE_FTP_DEFAULT PASSIVE

typedef unsigned char	t_bool;

typedef struct	s_res {
	int			code_res;
	char		*msg_res;
}				t_res;

static t_res	g_res_list[] =
{
	{200, "Command success."},
	{500, "Unknown command."},
	{0}
};

typedef enum	e_type
{
	ASCII = 0,
	BINARY
}				t_type;

typedef enum	e_mode_ftp
{
	ACTIVE = 0,
	PASSIVE
}				t_mode_ftp;

typedef struct	s_srv_ftp
{
	char		*addr;
	int			port;
	int			sock;
	int			sock_data;
	int			cs;
	int			cs_data;
	t_type		type;
	t_mode_ftp	mode_ftp;

}				t_srv_ftp;


/*
** Name: ftp_server_pi
** File: ftp_server_pi.c
** Desc: Fucntions start server
*/
void	ftp_srv_pi_send_response(t_srv_ftp *srv_ftp, int code, char *msg);
void	ftp_create_socket(t_srv_ftp *srv_ftp);

/*
** Name: ftp_server_dtp
** File: ftp_server_dtp.c
** Desc: Function of server Data Channel
*/
void	ftp_srv_dtp_accept_connection(t_srv_ftp *srv_ftp);
int		ftp_srv_dtp_create_channel(t_srv_ftp *srv_ftp);
void	ftp_srv_dtp_close_channel(t_srv_ftp *srv_ftp);
void	ftp_srv_dtp_send_data(t_srv_ftp *srv_ftp, char *data, int len);

/*
** Name: ftp_server_file
** File: ftp_server_file.c
** Desc: Function of File System
*/
int		ftp_srv_fs_open_file(char *name);
char	*ftp_srv_fs_read_file(int fd, int *len);
int		ftp_srv_fs_size_file(int fd);

/*
** Name: ftp_server_builtins
** File: ftp_server_builtins.c
** Desc: All builtins servers
*/
void	ftp_srv_builtin_ls(t_srv_ftp *srv_ftp, char **args);
void	ftp_srv_builtin_cd(t_srv_ftp *srv_ftp, char **args);
void	ftp_srv_builtin_pwd(t_srv_ftp *srv_ftp, char **args);
void	ftp_srv_builtin_get(t_srv_ftp *srv_ftp, char **args);
void	ftp_srv_builtin_put(t_srv_ftp *srv_ftp, char **args);
void	ftp_srv_builtin_quit(t_srv_ftp *srv_ftp, char **args);
void	ftp_srv_builtin_user(t_srv_ftp *srv_ftp, char **args);
void	ftp_srv_builtin_pass(t_srv_ftp *srv_ftp, char **args);
void	ftp_srv_builtin_noop(t_srv_ftp *srv_ftp, char **args);

void	ftp_srv_builtin_pasv(t_srv_ftp *srv_ftp, char **args);
void	ftp_srv_builtin_port(t_srv_ftp *srv_ftp, char **args);
void	ftp_srv_builtin_type(t_srv_ftp *srv_ftp, char **args);
void	ftp_srv_builtin_size(t_srv_ftp *srv_ftp, char **args);

/*
** Name: ftp_server_helper
** File: ftp_server_helper.c
** Desc: Function supplementary for server
*/
void	ftp_redirect_fd(int srcfd, int dstfd);
void	ftp_fork_process(char *path, char **av);
char	*ft_str_toupper(char *str);
char	*ft_arrjoin(char **arr_str);


/*
** Name: ftp_server_helper
** File: ftp_server_helper.c
** Desc: Function supplementary for server
*/
void	ftp_srv_ui_display_cmd(char *cmd);

/*
** Name: Struct of commands nvt, list in a global array.
*/
typedef	void	(*t_builtin_func)(t_srv_ftp *, char **);

typedef struct	s_cmd_nvt
{
	char			*name;
	char			**args;
	// char			*line_send;
	t_builtin_func	builtin;
}				t_cmd_nvt;

static t_cmd_nvt	g_cmd_nvt_list[] =
{
	{ "LIST",	NULL, ftp_srv_builtin_ls},
	{ "CWD",	NULL, ftp_srv_builtin_cd},
	{ "PWD",	NULL, ftp_srv_builtin_pwd},
	{ "RETR",	NULL, ftp_srv_builtin_get},
	{ "STOR",	NULL, ftp_srv_builtin_put},
	{ "QUIT",	NULL, ftp_srv_builtin_quit},
	{ "USER",	NULL, ftp_srv_builtin_user},
	{ "PASS",	NULL, ftp_srv_builtin_pass},
	{ "NOOP",	NULL, ftp_srv_builtin_noop},
	{ "PASV",	NULL, ftp_srv_builtin_pasv},
	{ "PORT",	NULL, ftp_srv_builtin_port},
	{ "TYPE",	NULL, ftp_srv_builtin_type},
	{ "SIZE",	NULL, ftp_srv_builtin_size},
	{ 0 }
};



#endif
