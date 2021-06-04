#include <stdio.h>
#include <stdlib.h>
#include "libft.h"
#include "parser.h"
#include "env_func.h"
#include "structs.h"

#include "exit.h"
#include "env_func.h"
#include <string.h> //delete

#define BLUE "\033[34;1m"
#define YELLOW  "\033[33m"
#define GREEN   "\033[32m"
#define RESET   "\033[0m"
#define ARROW "\U000027A1"
#define SHELL "\U0001F41A"
#define CROSS "\U0000274E"
#define DOLLAR GREEN"$"RESET
#define DIAMOND "\U0001F538"

static unsigned int g_ret;

void syntax_error(char *str)
{
	printf("%s: syntax error near unexpected token `%s'\n", PROMPT, str);
	g_ret = 258;
}

int	check_tokens(t_token *last)
{
	t_token	*previous;

	if(!last)
		return (1);
	previous = last;
	if(last->type == SEMICOLON || last->type == PIPE)
	{
		syntax_error(last->data);
		return 0;
	}
	while ((last = last->prev))
	{
		if(previous->type <= RED_DRIGHT && last->type < TEXT)
		{
			syntax_error(previous->data);
			return 0;
		}
		if(previous->type <= SEMICOLON && last->type == SEMICOLON)
		{
			syntax_error(previous->data);
			return 0;
		}
		previous = last;
	}
	if(previous->type <= PIPE)
	{
		syntax_error(previous->data);
		return 0;
	}
	return 1;

}

// char *type(int type) //delete
// {
// 	if(type == EMPTY)
// 		return strdup("EMPTY");
// 	else if(type == TEXT)
// 		return strdup("TEXT");
// 	else if(type ==  RED_RIGHT)
// 		return strdup("RED_RIGHT");
// 	else if(type ==  RED_DRIGHT)
// 		return strdup("RED_DRIGHT");
// 	else if(type ==  RED_LEFT)
// 		return strdup("RED_LEFT");
// 	else if(type == PIPE)
// 		return strdup("PIPE");
// 	else if(type == SEMICOLON)
// 		return strdup("SEMICOLON");
// 	else if(type == OR)
// 		return strdup("OR");
// 	else if(type == AND)
// 		return strdup("AND");
// 	else if(type == ENV)
// 		return strdup("ENV");
// 	else
// 		return strdup("ERROR TYPE");
// }


void	check_last_token(t_token **tokens)
{
	if (*tokens && (*tokens)->type == EMPTY)
	{
		if((*tokens)->next)
		{
			tokens = &(*tokens)->next;
			clear_prev(&((*tokens)->prev), free);
		}
		else
			clear_next(tokens, free);
	}
}

t_token	*parser(char *line)
{
	t_token	*tokens;

	tokens = parse_line(&line);
	check_last_token(&tokens);
	tokens = token_last(tokens);
	//create_array(&tokens, token_lst_size(tokens));
	if(!check_tokens(tokens))
		clear_prev(&tokens, free);
	//create_array(&tokens, token_lst_size(tokens));
	return (tokens);
}

void run_parser(char *line, t_list	**lenv)
{
	t_token	*tokens;
	t_list	**aaa;
	aaa = lenv;

	tokens = parser(line);
	//clear_prev(&tokens, free);
	// if(tokens)
	tokens_handler(&tokens, lenv);
}

void asd(char **env)
{
	t_list	*lenv;
	char	*line;

	lenv = env_create(env);

	// while (1)
	// {
		ft_putstr_fd(PROMPT, 1);
		get_next_line(0, &line);
		run_parser(line, &lenv);

	//}
	ft_lstclear(&lenv, env_clear);
}

int	main(int argc, char **argv, char **env)
{
	int num;
	char **aaa;
	num = argc;
	aaa = argv;

	asd(env);

	sleep(10);
	return (0);
}
