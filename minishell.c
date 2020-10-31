/*
Modifique el último programa de la sección Pipeline (item Pipes en acción) de tal modo que el usuario
pase como argumento dos comandos y el programa conecta la salida del primer argumento con la entrada
del segundo argumento. Si el programa resultado de compilar el código se llama pipeline entonces el 
usuario invoca el programa de la siguiente manera ./pipeline “cat p4.c” “wc -l” es equivalente a 
correr el programa de la siguiente manera cat p4.c | wc -l.


*/

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#define READ 0
#define WRITE 1

  void redireccion(char** commands)
  {
    int rc = fork();
    if (rc < 0)
    {
      // fork failed
      fprintf(stderr, "fork failed\n");
      exit(1);
    }
    else if (rc == 0)
    {
      // child: redirect standard output to a file
      close(STDOUT_FILENO);
      char* nombrearchivo = commands[3];
      char* comandoyargumentos = commands[1];
      
      open(nombrearchivo, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);

      // now exec "wc"...
      char *myargs[3];
      myargs[0] = strdup(strtok(comandoyargumentos, " "));
      myargs[1] = strdup(strtok(NULL, " "));
      myargs[2] = NULL;
      execvp(myargs[0], myargs);                   // runs word count
    }
    else
    {
      // parent goes down this path (main)
      int rc_wait = wait(NULL);
    }
  }

void writecommand(char *command)
{

  char *command_1;
  command_1 = (char *)malloc(100 * sizeof(char));
  char *myargvs[3];
  command_1 = command;

  myargvs[0] = strdup(strtok(command_1, " "));
  myargvs[1] = strdup(strtok(NULL, " "));
  myargvs[2] = NULL;

  execvp(myargvs[0], myargvs);
}

void el_pipe( char *argv[] )
{
  pid_t pid;
  int fd[2];
  int contador;

  if (pipe(fd) == -1)
  {
    perror("Creating pipe");
    exit(EXIT_FAILURE);
  }

  switch (pid = fork())
  {

  case 0:

    // The child process will execute wc.
    // Close the pipe write descriptor.
    close(fd[WRITE]);
    // Redirect STDIN to read from the pipe.
    dup2(fd[READ], STDIN_FILENO);
    writecommand(argv[3]);

  case -1:

    contador++;
    perror("fork() failed:   \n");
    exit(EXIT_FAILURE);

  default:
  {
    printf("eeee  %s", strdup(argv[1]));
    contador++;

    close(fd[READ]);
    dup2(fd[WRITE], STDOUT_FILENO);

    writecommand(argv[1]);
  }
  }
}

int main(int argc, char *argv[])
{
  printf("%s", strdup(argv[2]));
  if (strcmp(strdup(argv[2]), "|") == 0)
  {
    el_pipe( argv );

  }
  else if (strcmp(strdup(argv[2]), ">") == 0) {
    redireccion(argv);
  }
}

/*
void redirecttofile(){
  open("dup.txt", O_WRONLY | O_APPEND);

}*/