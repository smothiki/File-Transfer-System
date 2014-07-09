server: main.o logger.o config_reader.o string_map.o data_list.o authenticator.o
	gcc -g -o server main.o logger.o config_reader.o string_map.o data_list.o authenticator.o -lpthread

main.o: main.c
	gcc -g -c main.c

logger.o: logger.c logger.h
	gcc -g -c logger.c

config_reader.o: config_reader.c config_reader.h
	gcc -g -c config_reader.c

string_map.o: string_map.c string_map.h
	gcc -g -c string_map.c

data_list.o: data_list.h data_list.c
	gcc -g -c data_list.c

authenticator.o: authenticator.c authenticator.h
	gcc -g -c authenticator.c


