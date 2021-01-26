CFLAGS=-c -g -Wall

test: clean eval
	./eval	'(A2*((3+45)-8))+14/3'
	./eval	'(22**( (3+ 45)-8))+14 /3'
	./eval	'(22*( (3+ 45)-8))+14 /3'

#edition du lien
eval: eval.o
	gcc eval.o -o eval 

#compilation
eval.o: eval.c 
	gcc	$(CFLAGS) eval.c
	
clean:
	rm -rf eval *.o 
