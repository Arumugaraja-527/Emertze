obj = $(patsubst %.c, %.o, $(wildcard *.c))
Inverted_search.exe : $(obj)
	gcc -o $@ $^
clean :
	rm *.exe *.0	