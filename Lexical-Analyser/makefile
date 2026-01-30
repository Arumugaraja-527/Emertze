obj = $(patsubst %.c, %.o, $(wildcard *.c))
lexical.exe : $(obj)
	gcc -o $@ $^
clean :
	rm *.exe *.0	