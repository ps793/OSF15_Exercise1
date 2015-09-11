#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "command.h"

#define MAX_CMD_COUNT 50
#define MAX_CMD_LEN 25


/* 
 * PURPOSE: read what use input is  
 * INPUTS: 
 *	input : users' input;
 *  cmd: put users' input into command information
 * RETURN:
 *  If no errors occurred during instantiation then true
 *  else false for an error in the process.
 *
 **/

bool parse_user_input (const char* input, Commands_t** cmd) {
	
	// ERROR CHECK INCOMING PARAMETERS
	if (input == NULL){
		printf("no input from user");
		return false;
	}
	//no limit on **cmd

	char *string = strdup(input);
	
	*cmd = calloc (1,sizeof(Commands_t));
	(*cmd)->cmds = calloc(MAX_CMD_COUNT,sizeof(char*));

	unsigned int i = 0;
	char *token;
	token = strtok(string, " \n");
	for (; token != NULL && i < MAX_CMD_COUNT; ++i) {
		(*cmd)->cmds[i] = calloc(MAX_CMD_LEN,sizeof(char));
		if (!(*cmd)->cmds[i]) {
			perror("Allocation Error\n");
			return false;
		}	
		strncpy((*cmd)->cmds[i],token, strlen(token) + 1);
		(*cmd)->num_cmds++;
		token = strtok(NULL, " \n");
	}
	free(string);
	return true;
}

/* 
 * PURPOSE: solve the problem of memory leaking for command 
 * INPUTS: 
 *	cmd: users' command;
 * RETURN:
 *  print something based on process;
 **/
void destroy_commands(Commands_t** cmd) {

	// ERROR CHECK INCOMING PARAMETERS
	if ((*cmd) == NULL){
		printf("no command in destroy_commands!");
		return;
	}
	
	for (int i = 0; i < (*cmd)->num_cmds; ++i) {
		free((*cmd)->cmds[i]);
	}
	free((*cmd)->cmds);
	free((*cmd));
	*cmd = NULL;
}

