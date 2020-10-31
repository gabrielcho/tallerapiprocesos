main:  minishell.c
	gcc minishell.c -o minishell
	./lol "ls --help" ">" "lol.txt"