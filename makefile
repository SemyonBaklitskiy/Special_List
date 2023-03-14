all: bin/start
	bin/./start

bin/start: bin/main.o bin/functions.o bin/errors.o
	g++ -Wall -Wextra bin/main.o bin/functions.o bin/errors.o -o bin/start

bin/main.o: src/main.cpp
	if [ ! -d bin ]; then mkdir bin; fi
	g++ -Wall -Wextra -c -Iincludes/ src/main.cpp -o bin/main.o

bin/functions.o: src/list_functions.cpp includes/list_functions.h
	g++ -Wall -Wextra -c -Iincludes/ src/list_functions.cpp -o bin/functions.o

bin/errors.o: src/list_errors.cpp includes/list_errors.h
	g++ -Wall -Wextra -c -Iincludes/ src/list_errors.cpp -o bin/errors.o

clean:
	rm -r bin/