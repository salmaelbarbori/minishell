/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjuliean <tjuliean@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/17 19:17:19 by tjuliean          #+#    #+#             */
/*   Updated: 2021/05/16 14:18:50 by tjuliean         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "struct.h"
#include "buildin.h"
#include "env_func.h"


int main (int argc, char **argv, char **env)
{
	env = env_dup(env);
	if (argc == 2)
	{
		ft_cd(argv[1]);
	}
	else if (argc == 3)
	{
		ft_export(argv + 1, &env);
		ft_env((const char **)env);
	}
	else
		ft_pwd();
	free(env);
	return (0);
}
