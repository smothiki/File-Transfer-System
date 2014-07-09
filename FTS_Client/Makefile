fts_client: cmd_mapper.o command_interpreter.o main.o client_cmd_handlers.o logger.o config_reader.o progress_bar.o string_map.o data_list.o
	gcc -g -o fts_client cmd_mapper.o command_interpreter.o main.o client_cmd_handlers.o logger.o config_reader.o progress_bar.o string_map.o data_list.o

cmd_mapper.o: cmd_mapper.h cmd_mapper.c
	gcc -g -c cmd_mapper.c

command_interpreter.o: command_interpreter.h command_interpreter.c
	gcc -g -c command_interpreter.c

client_cmd_handlers.o: client_cmd_handlers.c client_cmd_handlers.h
	gcc -g -c client_cmd_handlers.c

main.o: main.c
	gcc -g -c main.c

logger.o: logger.c logger.h
	gcc -g -c logger.c

config_reader.o: config_reader.c config_reader.h
	gcc -g -c config_reader.c

progress_bar.o: progress_bar.c progress_bar.h
	gcc -g -c progress_bar.c

string_map.o: string_map.c string_map.h
	gcc -g -c string_map.c

data_list.o: data_list.h data_list.c
	gcc -g -c data_list.c


