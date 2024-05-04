#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct      s_list
{
    struct s_list   *next;
    void            *data;
}                   t_list;

void ft_list_remove_if(t_list **begin_list, void *data_ref, int (*cmp)())
{
	t_list *previous;
	t_list *current;

	if (!begin_list || !*begin_list)
		return ;

	previous = *begin_list;
	current = previous->next;
	while (current)
	{
		if (cmp(current->data, data_ref) == 0) {
			previous->next = current->next;
			free(current);
		} else {
			previous = current;
		}
		current = previous->next;
	}
	if (cmp((*begin_list)->data, data_ref) == 0) {
		current = *begin_list;
		(*begin_list) = (*begin_list)->next;
		free(current);
	}
}

int	ft_strcmp(char *s1, char *s2)
{
	size_t	i;

	i = 0;
	while ((s1[i] && s2[i]))
	{
		if (s1[i] != s2[i])
			return (s1[i] - s2[i]);
		i++;
	}
	return (0);
}

void	display_all(t_list *maillon)
{
	while (maillon)
	{
		printf("[%s]\n", (char *)maillon->data);
		maillon = maillon->next;
	}
	printf("\n");
}

void	add_to_list(t_list *maillon, char *word)
{
	t_list *new;

	new = malloc(sizeof(t_list));
	new->next = NULL;
	new->data = word;

	while (maillon->next)
	{
		maillon = maillon->next;
	}
	maillon->next = new;
}

int	main(void)
{
	t_list *maillon;

	maillon = malloc(sizeof(t_list));
	maillon->data = "Heyyy";
	maillon->next = NULL;

	add_to_list(maillon, "salut");
	add_to_list(maillon, "les");
	add_to_list(maillon, "amis");

	display_all(maillon);
	ft_list_remove_if(&maillon, "les", ft_strcmp);
	display_all(maillon);
}
