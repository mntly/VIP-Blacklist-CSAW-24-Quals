_QWORD *__fastcall randGen(_QWORD *a1)
{
  unsigned int seed; // eax
  _QWORD *result; // rax
  unsigned __int64 i; // [rsp+10h] [rbp-10h]
  _BYTE *v4; // [rsp+18h] [rbp-8h]

  v4 = malloc(10uLL);
  seed = time(0LL);
  srand(seed);
  for ( i = 0LL; i <= 9; ++i )
    v4[i] = rand();
  result = a1;
  *a1 = v4;
  return result;
}

int displayCommands()
{
  unsigned int i; // [rsp+Ch] [rbp-4h]

  printf("\nCommands: ");
  for ( i = 0; i <= 3; ++i )
    printf("%s ", &whitelist[6 * i]);
  return putchar('\n');
}

void __noreturn kickOut()
{
  puts("\"You are not a real VIP. Follow this person out.\"");
  exit(1);
}

unsigned __int64 allowCopy()
{
  int i; // [rsp+8h] [rbp-88h]
  int j; // [rsp+Ch] [rbp-84h]
  int k; // [rsp+10h] [rbp-80h]
  unsigned __int64 m; // [rsp+18h] [rbp-78h]
  ssize_t read_length; // [rsp+20h] [rbp-70h]
  char v6[6]; // [rsp+30h] [rbp-60h] BYREF
  char v7[6]; // [rsp+36h] [rbp-5Ah] BYREF
  int v8; // [rsp+3Ch] [rbp-54h]
  __int64 v9; // [rsp+40h] [rbp-50h]
  char input_VIP[40]; // [rsp+50h] [rbp-40h] BYREF
  unsigned __int64 v11; // [rsp+78h] [rbp-18h]

  v11 = __readfsqword(0x28u);
  puts(
    "You may add a new command, \"queue\", to your possible commands which will give you your position. \n"
    "If you would not like this, just press enter.");
  displayCommands();
  read_length = read(0, input_VIP, 0x20uLL);
  if ( read_length < 0 ) {
    perror("Error reading from stdin");
    exit(1);
  }
  input_VIP[strcspn(input_VIP, "\n")] = 0;
  for ( i = 0; i < strlen("queue") + 1; ++i ) {
    if ( input_VIP[i] != s2[i] )
      kickOut();
  }
  puts("\"We are currently getting you a valet to inform you of your queue position\nPlease wait one second...\"");
  v9 = 0LL;
  strcpy(v6, "clear");
  strcpy(v7, "exit");                           // "exit"
  v7[5] = '\0';
  v8 = 'sl';                                    // "ls"
  for ( j = 3; j >= 0; --j )
    strcpy(&whitelist[6 * j], &whitelist[6 * j - 6]);// [j] <- [j - 1]
  for ( k = 0; k < read_length - 1; ++k )
    whitelist[k] = input_VIP[k];                // [0] <- input
  if ( !(unsigned int)safety((__int64)v6) )
    kickOut();
  sleep(1u);
  puts("\"The valet has arrived, feel free to check your queue position now.\"");
  for ( m = 0LL; m < 4; ++m )
    puts(&whitelist[6 * m]);
  return v11 - __readfsqword(0x28u);
}

unsigned __int64 handle_client()
{
  unsigned int v1; // [rsp+Ch] [rbp-A4h]
  int v2; // [rsp+10h] [rbp-A0h]
  unsigned int i; // [rsp+14h] [rbp-9Ch]
  char *random_ten[2]; // [rsp+18h] [rbp-98h] BYREF
  FILE *stream; // [rsp+28h] [rbp-88h]
  char src[10]; // [rsp+36h] [rbp-7Ah] BYREF
  char input[32]; // [rsp+40h] [rbp-70h] BYREF
  char dest[12]; // [rsp+60h] [rbp-50h] BYREF
  int v9; // [rsp+6Ch] [rbp-44h]
  __int64 v10; // [rsp+70h] [rbp-40h]
  __int64 v11; // [rsp+78h] [rbp-38h]
  __int64 v12; // [rsp+80h] [rbp-30h]
  __int64 v13; // [rsp+88h] [rbp-28h]
  __int64 v14; // [rsp+90h] [rbp-20h]
  __int64 v15; // [rsp+98h] [rbp-18h]
  unsigned __int64 v16; // [rsp+A8h] [rbp-8h]

  v16 = __readfsqword(0x28u);
  v1 = 20;
  randGen(random_ten);
  puts(
    "\"Welcome to the club. It's ok, don't be in a rush. You've got all the time in the world. As long as you are a vip that is.\"");
  displayCommands();
  while ( fgets(input, 32, stdin) )
  {
    input[strcspn(input, "\n")] = 0;
    if ( !strcmp(input, "exit") ) {
      puts("Bye!");
      return v16 - __readfsqword(0x28u);
    }
    if ( !strcmp(input, random_ten[0]) && strcmp(whitelist, "queue") ) {
      puts("\nAh VIP, please come this way...");
      allowCopy();
    }
    sprintf(src, input);
    strcpy(dest, "Executing: ");
    v9 = 0;
    v10 = 0LL;
    v11 = 0LL;
    v12 = 0LL;
    v13 = 0LL;
    v14 = 0LL;
    v15 = 0LL;
    strcat(dest, src);
    strcpy(&dest[strlen(dest)], "...\n");
    puts(dest);
    v2 = 0;
    for ( i = 0; i <= 3; ++i ) {
      if ( !strcmp(input, &whitelist[6 * i]) ) {
        v2 = 1;
        break;
      }
    }
    if ( !v2 ) {
      random_ten[1] = "Command not allowed\n";
      printf("%s", "Command not allowed\n");
      goto LABEL_22;
    }
    if ( !strcmp(input, "queue") ) {
      printf("You are currently in position: %d\n", v1);
    } else {
      stream = popen(input, "r");
      if ( !stream ) {
        perror("Error executing command");
        return v16 - __readfsqword(0x28u);
      }
      while ( fgets(input, 32, stream) )
        printf("%s", input);
      pclose(stream);
      if ( !--v1 ) {
        puts("Hello! You are at the front of the queue now. Oh hold on one second");
        puts("I'm getting some new info...");
        kickOut();
      }
LABEL_22:
      displayCommands();
    }
  }
  return v16 - __readfsqword(0x28u);
}

int __cdecl main(int argc, const char **argv, const char **envp)
{
  setvbuf(stdout, 0LL, 2, 0LL);
  setvbuf(stdin, 0LL, 2, 0LL);
  fflush(stdout);
  handle_client();
  return 0;
}
