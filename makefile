main:  minishell.c
	gcc minishell.c -o minishell
	./minishell "ls --help" ">" "lol.txt"