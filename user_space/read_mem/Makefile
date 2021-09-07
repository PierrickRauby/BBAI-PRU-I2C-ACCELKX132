read_mem : read_mem.o
	gcc -o read_mem read_mem.o
	rm -rf *.o
	#./read_mem
read_mem.o : read_mem.c
	gcc -o read_mem.o -c read_mem.c 

clean:
	rm -rf *.o
	rm -rf read_mem
