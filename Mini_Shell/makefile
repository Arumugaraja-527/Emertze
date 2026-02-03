obj = $(patsubst %.c, %.o, $(wildcard *.c))
minishell.exe : $(obj)
	gcc -o $@ $^
clean :
	rm *.exe *.0