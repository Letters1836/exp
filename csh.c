#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <linux/limits.h>
#include <sys/wait.h>

#define _NRM  "\x1B[0m"
#define _RED  "\x1B[31m"
#define _GRN  "\x1B[32m"
#define _YEL  "\x1B[33m"
#define _BLU  "\x1B[34m"
#define _MAG  "\x1B[35m"
#define _CYN  "\x1B[36m"
#define _WHT  "\x1B[37m"

int main(int argc, char *argv[])
{
	// clearing screen
	system("clear");

	// getting current directory
	char _curDir[PATH_MAX];
	getcwd(_curDir, sizeof(_curDir));

	// getting username
	char *_username = getlogin();

	while (1)
	{
		printf("%s%s@c-shell:%s%s> %s", _YEL, _username, _BLU, _curDir, _NRM);

		char *_buffer = NULL;
		size_t _bufSize = 32;
		size_t _characters;
		getline(&_buffer, &_bufSize, stdin);

		// printf("%s", buffer);

		// getting arguments from shell
		_buffer[strlen(_buffer) - 1] = 0; // remove \n from end of _buffer
		if (strcmp(_buffer, "") == 0) // if there is no input, go to the end
		{
			goto end;
		}

		int _argc = 0;
		char _delims[] = " \t";
		char *_token = strtok(_buffer, _delims);

		if (_token == NULL) // if there are only spaces, go to the end
		{
			goto end;
		}

		char *_cmd; // command
		char *_args[10]; // arguments
		memset(_args, 0, sizeof _args); // reset arguments

		while (_token != NULL)
		{
			if (_argc == 0)
			{
				_cmd = _token;
				// printf("command: %s\n", _cmd);
			}
			_args[_argc] = _token;
			// printf("argument: %s\n", _args[argc - 1]);

			_token = strtok(NULL, " ");
			_argc++;
		}

		// recognized commands
		char *_exit = "exit";
		char *_cd = "cd";

		if (strcmp(_cmd, _exit) == 0)
		{
			exit(0);
		}
		else if (strcmp(_cmd, _cd) == 0)
		{
			char _newPath[PATH_MAX] = ""; // prepare input
			strcat(_newPath, _args[1]); // get argument from user
			if (strcmp(_args[1], "~") == 0)
			{
				chdir(getenv("HOME"));
			}
			chdir(_newPath); // change directory
			getcwd(_curDir, sizeof(_curDir)); // replace current directory with new directory
		}
		else
		{
			int _pid = fork(); // set child process PID

			if (_pid == 0) // if the child is running
			{
				execvp(_cmd, _args); // execute the inputted command with arguments
				fprintf(stderr, "Child process failed.\n");
			}
			else // if the child is over or the parent process
			{
				wait(NULL);
				// printf("Child exited\n");
			}
		}

		end: ; // goto point for error handling purposes
	}
}
