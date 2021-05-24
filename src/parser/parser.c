/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mharriso <mharriso@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/15 20:47:57 by mharriso          #+#    #+#             */
/*   Updated: 2021/05/23 16:05:21 by mharriso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "libft.h"
#include "exit.h"
#include "parser.h"
#include "env_func.h"
#include <string.h> //delete

void	parse_redirect(t_token **tokens, t_line *line, char c)
{
	if (c == '<')
	{
		create_new_token(tokens, line->len);
		(*tokens)->type =  RED_LEFT;
		(*tokens)->data[(*tokens)->len++] = c;
		create_new_token(tokens, line->len);
	}
	else if (c == '>' && line->data[line->index + 1] == '>')
	{
		create_new_token(tokens, line->len);
		(*tokens)->type =  RED_DRIGHT;
		(*tokens)->data[(*tokens)->len++] = c;
		(*tokens)->data[(*tokens)->len++] = line->data[++line->index];
		create_new_token(tokens, line->len);
	}
	else if (c == '>')
	{
		create_new_token(tokens, line->len);
		(*tokens)->type =  RED_RIGHT;
		(*tokens)->data[(*tokens)->len++] = c;
		create_new_token(tokens, line->len);
	}
}
char *get_env_name(t_line *line)
{
	char	*name;
	char	c;
	int		res;
	int		i;

	name = malloc(line->len - line->index + 1);
	if(!name)
		error_exit("malloc error");
	c = line->data[line->index++];
	res = ft_isalnum(c) || c == '_';
	i = 0;
	while(c && res)
	{
		name[i] = c;
		c = line->data[line->index++];
		res = ft_isalnum(c) || c == '_';
		i++;
	}
	line->index--;
	name[i] = '\0';
	return (name);
}
int	check_first_symbol(t_token **tokens, t_line *line, char c)
{
	int res;

	if(c == ' ' || c == '\0' || c == '\'' || c == '\"')
	{
		(*tokens)->type = TEXT;
		(*tokens)->data[(*tokens)->len++] = '$';
		return (0);
	}
	res = ft_isalpha(c) || c == '_';
	line->index++;
	if(c == '?')
	{
		(*tokens)->data[(*tokens)->len++] = '!'; //example
		return (0);
	}
	if(res)
		return (1);
	return (0);

}

void parse_env(t_token **tokens, t_line *line, t_list **env)
{
	char	*name;
	char	*value;
	int		res;
	char	*tmp;

	res = check_first_symbol(tokens, line, line->data[line->index + 1]);
	if(!res)
		return ;
	name = get_env_name(line);
	value = env_getvaluebyname(name, *env);
	tmp = ft_strjoin((*tokens)->data, value);
	free((*tokens)->data);
	(*tokens)->data = tmp;
	(*tokens)->len += ft_strlen(value);
	printf("name  = |%s|\nvalue = %s\n", name, value);
}

void	parse_normal(t_token **tokens, t_line *line, t_list **env, char c)
{
	if (c == '\"')
	{
		create_new_token(tokens, line->len);
		line->status = IN_DQUOTES;
	}
	else if (c == '\'')
	{
		create_new_token(tokens, line->len);
		line->status = IN_QUOTES;
	}
	else if (c == '>' || c == '<')
		parse_redirect(tokens, line, c);
	else if (c == ' ' || c == '\t')
		create_new_token(tokens, line->len);
	else if (c == '|')
	{
		create_new_token(tokens, line->len);
		(*tokens)->type = PIPE;
		(*tokens)->data[(*tokens)->len++] = c;
		create_new_token(tokens, line->len);
	}
	else if (c == '$')
		parse_env(tokens, line, env);
	else
	{
		(*tokens)->type = TEXT;
		(*tokens)->data[(*tokens)->len++] = c;
	}
}

void	parse_in_dquotes(t_token **tokens, t_line *line, t_list **env, char c)
{
	if (c == '\"')
	{
		create_new_token(tokens, line->len);
		line->status = NORMAL;
	}
	else if (c == '$' && line->data[line->index + 1])
	{
		(*tokens)->type = TEXT;
		parse_env(tokens, line, env);
	}
	else
	{
		(*tokens)->type = TEXT;
		(*tokens)->data[(*tokens)->len++] = c;
	}
}

void	parse_in_quotes(t_token **tokens, t_line *line, char c)
{
	if (c == '\'')
	{
		create_new_token(tokens, line->len);
		line->status = NORMAL;
	}
	else
	{
		(*tokens)->type = TEXT;
		(*tokens)->data[(*tokens)->len++] = c;
	}
}

char *type(int type) //delete
{
	if(type == EMPTY)
		return strdup("EMPTY");
	else if(type == TEXT)
		return strdup("TEXT");
	else if(type ==  RED_RIGHT)
		return strdup("RED_RIGHT");
	else if(type ==  RED_DRIGHT)
		return strdup("RED_DRIGHT");
	else if(type ==  RED_LEFT)
		return strdup("RED_LEFT");
	else if(type == PIPE)
		return strdup("PIPE");
	else
		return strdup("ERROR TYPE");
}

char		**create_array(t_token **head, int size)
{
	char	**arr;
	t_token	*tmp;

	tmp = *head;
	if (!(arr = malloc((size + 1) * sizeof(char *))))
		error_exit("malloc error");
	arr[size] = NULL;
	while (tmp)
	{
		size--;
		printf("%-11s:  %s\n", type(tmp->type), tmp->data); //delete
		if (!(arr[size] = malloc(tmp->len + 1)))
			error_exit("malloc error");
		ft_memcpy(arr[size], tmp->data, tmp->len);
		arr[size][tmp->len] = '\0';
		tmp = tmp->next;
	}
	return (arr);
}

char	**parser(char *str, t_list **env)
{
	t_token		*tokens;
	t_line		line;
	char		**tokens_arr;

	tokens = NULL;
	line_init(&line, str);
	start_tokens(&tokens, line.len);
	while (line.data[line.index])
	{
		if (line.status == NORMAL)
			parse_normal(&tokens, &line, env, line.data[line.index]);
		else if (line.status == IN_DQUOTES)
			parse_in_dquotes(&tokens, &line, env, line.data[line.index]);
		else if (line.status == IN_QUOTES)
			parse_in_quotes(&tokens, &line, line.data[line.index]);

		line.index++;
	}
	int size = token_lst_size(tokens);
	//printf("list size = %d\n\n", size);
	tokens_arr = create_array(&tokens, size);
	clear_tokens(&tokens, free);
	return (tokens_arr);
}
