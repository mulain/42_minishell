#include "../../include/minishell.h"

/*
For now, is only creating the export shismshmang if the function is called.
That prolly should be changed later and moved to init to be more structured.

export a=3 b =2 c=3
- 	a and c will work, it will not process b and give error ` =2': not a valid
	identifier b will only get declared. So each string represents the var name
	until either = or end of token (0, metachar whaeva)
-	bash-3.2$ export "a      =b"
	bash: export: `a      =b': not a valid identifier
-	bash-3.2$ export "a|=b"
	bash: export: `a|=b': not a valid identifier
-	bash-3.2$ export "a<=b"
	bash: export: `a<=b': not a valid identifier
-	Gonna go with: no metachars allowed except ofc '='

*/
bool	export(t_cmd *cmdnode)
{
	//char	**env_clone;

	//syntaxcheck, gotta do it per cmdarrpos tho, cause invalid ones get
	//skipped and dont terminate whole shmismshmang
	/* if (!cmdnode->cmd_arr[1])
		return (print_export(cmdnode->data), false); */
	if (cmdnode->cmd_arr[1][0] == '-')
		return (msg_error("export", cmdnode->cmd_arr[1], E_INVALOPT), true);
	build_exportlist(cmdnode->data);
	replace_env(cmdnode->data);
	return (false);
}

void	replace_env(t_data *data)
{
	int		i;
	char	**env_clone;

	if (!data->env)
		return ;
	i = 0;
	env_clone = NULL;
	while (data->env[i])
	{
		env_clone = append_string(env_clone, data->env[i]);
		i++;
	}
	data->env = env_clone;
}

/*
-	get name of env line:

-	get value of env line:
	ft_strchr returns a pointer to the first occurence of a char. Use this to get
	the address of the '=' in the env[i] string and pass that + 1 as the start of
	ft_substr. Pass ft_strlen as max len to ft_substr because it only has to be
	long enough, doesn't matter if too long.
*/
void	build_exportlist(t_data *data)
{
	int		i;
	t_exp	*expnode;

	i = 0;
	while (data->env[i])
	{
		expnode = malloc(1 * sizeof(t_exp));
		expnode->name = ft_substr(data->env[i], 0, ft_strchr(data->env[i], '=') - data->env[i]);
		expnode->value = ft_substr(data->env[i], ft_strchr(data->env[i], '=') - data->env[i] + 1,
				ft_strlen(data->env[i]));
		printf("expnode name:%s\n", expnode->name);
		printf("expnode value:%s\n", expnode->value);
		ft_lstadd_back(&data->exp_list, ft_lstnew(expnode));
		i++;
	}
}
