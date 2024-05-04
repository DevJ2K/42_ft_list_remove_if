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

	// Pouvoir eviter les potentiels segfault en essayant de faire (*begin_list)->next;
	if (!begin_list || !*begin_list)
		return ;

	// On conserve le precedent maillon dans 'previous' et celui actuelle dans 'current'.
	// Par defaut le previous est le debut et le current est le suivant
	// Ex. de liste chainees : SALUT -> LES -> AMIS
	previous = *begin_list; // Il y aura 'SALUT' ici
	current = previous->next; // 'LES' ici

	// On commence a parcourir a partir du 'current' du coup vu qu'on souhaite conserver le precedent.
	while (current)
	{
		// La on fait la comparaison de la data
		// 1er cas : La data est la meme (cmp(x, y) == 0) :
		//   Alors le suivant du precedent devient le suivant de celui actuelle.
		//   Si on veut retirer 'LES' de la liste chainees,
		//   en sachant que 'previous' = SALUT & 'current' = LES
		//   on va passer de :
		//   SALUT -> LES -> AMIS
		//   a ca :
		//   SALUT -> AMIS
		if (cmp(current->data, data_ref) == 0) {
			previous->next = current->next;
			free(current);
		// 2e cas : On actualise juste le previous qui devient le 'current'
		} else {
			previous = current;
		}
		// Puis on passe au suivant en recuperant le suivant du precedent.
		current = previous->next;
	}
	// Etant donne qu'on a commence au 2e maillon, c'est a dire 'LES',
	// et bien a aucun moment on a verifie le 1er maillon qui contenait 'SALUT'
	// Donc on rajoute cette condition pour changer le debut si jamais il faut le retirer.
	if (cmp((*begin_list)->data, data_ref) == 0) {
		current = *begin_list;
		(*begin_list) = (*begin_list)->next;
		free(current);
	}
}

// TESTER SON CODE

// STRCMP c'est la plus simple pour comparer des data a mon avis
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

// Afficher la liste chainees pour le debuggage
void	display_all(t_list *maillon)
{
	while (maillon)
	{
		printf("[%s]\n", (char *)maillon->data);
		maillon = maillon->next;
	}
	printf("\n");
}

// Ajouter un maillon a la liste
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

// ULTRA OPTIONNEL : Nettoyer la liste, pour checker que valgrind est bien clean
void	clear_list(t_list *maillon)
{
	t_list *to_del;
	while (maillon)
	{
		to_del = maillon;
		maillon = maillon->next;
		free(to_del);
	}
}

int	main(int ac, char **av)
{
	t_list *maillon;
	char *first_word = "Heyyy";
	char *tab[] = {
		"remove",
		"remove",
		"salut",
		"remove",
		"remove",
		"les",
		"remove",
		"amis",
		"remove",
		"!",
		"remove",
		NULL
	};
	// Pouvoir eviter certains soucis on cree le premier maillon direct dans le main
	maillon = malloc(sizeof(t_list));
	maillon->data = first_word;
	maillon->next = NULL;

	for (size_t i = 0; tab[i]; i++)
	{
		add_to_list(maillon, tab[i]);
	}
	printf("====DEFAULT LIST====\n");
	display_all(maillon);
	printf("====CALLING FT_LIST_REMOVE_IF====\n");
	for (int i = 1; i < ac; i++)
	{
		ft_list_remove_if(&maillon, av[i], ft_strcmp);
		printf("AFTER REMOVE {%s}:\n", av[i]);
		display_all(maillon);
	}
	printf("====END OF TESTING====\n");
	clear_list(maillon);
}
