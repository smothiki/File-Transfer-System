fts_server: cmd_mapper.o command_interpreter.o main.o server_cmd_handlers.o
	gcc -g -o fts_server cmd_mapper.o command_interpreter.o main.o server_cmd_handlers.o

cmd_mapper.o: cmd_mapper.h cmd_mapper.c
	gcc -g -c cmd_mapper.c

command_interpreter.o: command_interpreter.h command_interpreter.c
	gcc -g -c command_interpreter.c

server_cmd_handlers.o: server_cmd_handlers.c server_cmd_handlers.h
	gcc -g -c server_cmd_handlers.c

main.o: main.c
	gcc -g -c main.c

