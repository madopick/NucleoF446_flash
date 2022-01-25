/*
 * tinysh.c
 *
 *  Created on: Jan 24, 2022
 *      Author: madop
 */


#include "main.h"
#include "tiny_sh.h"


#ifdef SHELL_CMD

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 	256
#endif
#ifndef HISTORY_DEPTH
#define HISTORY_DEPTH 	6		// 16
#endif
#ifndef MAX_ARGS
#define MAX_ARGS 		4		// 16
#endif
#ifndef PROMPT_SIZE
#define PROMPT_SIZE 	8 		// 16
#endif
#ifndef TOPCHAR
#define TOPCHAR 		'/'
#endif

typedef unsigned char uchar;

/* redefine some useful and maybe missing utilities to avoid conflicts */
#define	strlen	tinysh_strlen
#define	puts	tinysh_puts
#define putchar tinysh_char_out

static void help_fnt(int argc, char **argv);
static tinysh_cmd_t help_cmd={  0,"HELP","	   	[NONE]","[NONE]",help_fnt,0,0,0 };
static int strlen(uchar *s);
static void puts(char *s);

static uchar 		input_buffers[HISTORY_DEPTH][BUFFER_SIZE+1] = {0};
static uchar 		trash_buffer[BUFFER_SIZE+1] ={0};
static int 	 		cur_buf_index=0;
static uchar 		context_buffer[BUFFER_SIZE+1] ={0};
static int 	 		cur_context=0;
static int 	 		cur_index=0;
static int 	 		echo=1;
static char  		prompt[PROMPT_SIZE+1]="$ ";
static tinysh_cmd_t *root_cmd=&help_cmd;
static tinysh_cmd_t *cur_cmd_ctx=0;
static void 		*tinysh_arg=0;


extern UART_HandleTypeDef huart2;
extern struct fwCfg_t userConfig;


//static char argumentNotmatch[] 	= "argument not match\r\n";
//static char argumentInvalid[] 	= "invalid argument\r\n";



//static tinysh_cmd_t lsetRFMode={0,"SETRF","		[PP,BB,SS,CC,TT,RR]","[22,01,09,01,04,04]",loraSetRFMode,0,0,0};


/**************************************
 * FLASH ERASE COMMAND	  			  *
 **************************************/
static void fEraseCMD(int argc, char **argv)
{
	if (argc > 1){
		puts("FERASE invalid arguments!\r\n");
		return;
	}else{
		flashErase();
		puts("FERASE finish!\r\n");
	}
}


/**************************************
 * FLASH WRITE COMMAND	  			  *
 **************************************/
static void fWriteCMD(int argc, char **argv)
{
	if (argc != 2){
		puts("FWRITE invalid arguments!\r\n");
		return;
	}else{

		uint8_t len = strlen((uchar*)&argv[1][0]);
		//printf("FWRITE: %s (%d)\r\n", (uchar*)&argv[1][0], len);

		uint32_t value = (uint32_t)tinysh_dec((char*)&argv[1][0]);
		printf("DEC (%d): %ld \r\n", len, value);


		HAL_StatusTypeDef status = flashWrite(FLASH_USER_START_ADDR, value);
		if(status == HAL_OK){
			puts("FWRITE complete!");
		}else{
			puts("FWRITE fail!");
		}

	}
}


/**************************************
 * FLASH READ COMMAND	  			  *
 **************************************/
static void fReadCMD(int argc, char **argv)
{
	if (argc > 1){
		//puts(argumentNotmatch);
		return;
	}else{
		printf("FREAD: %ld \r\n", flashRead(FLASH_USER_START_ADDR));
	}
}



/**************************************
 * RAM READ COMMAND	  			  *
 **************************************/
static void ram2ReadCMD(int argc, char **argv)
{
	uint32_t data32;

	data32 = *(__IO uint32_t *)0x2001F000;
	printf("0x2001F000: %ld\r\n", data32);

	data32 = *(__IO uint32_t *)0x2001F004;
	printf("0x2001F004: %ld\r\n", data32);

	data32 = *(__IO uint32_t *)0x2001F008;
	printf("0x2001F000: %ld\r\n", data32);

	//printf("userConfig: %d %d %d %d \r\n", userConfig.u32_crc, userConfig.u32_crcN, userConfig.u32_len, userConfig.u32_lenN);
}


/**************************************
 * RAM WRITE COMMAND	  			  *
 **************************************/
static void ram2WriteCMD(int argc, char **argv)
{
	if (argc != 2){
		puts("RAMWRITE invalid arguments!\r\n");
		return;
	}else{

		uint8_t len = strlen((uchar*)&argv[1][0]);
		//printf("FWRITE: %s (%d)\r\n", (uchar*)&argv[1][0], len);

		uint32_t value = (uint32_t)tinysh_dec((char*)&argv[1][0]);
		printf("DEC (%d): %ld \r\n", len, value);


		volatile int *point = (int *)0x2001F000;
		*point = value;

		puts("RAMWRITE complete!");
	}
}


/**************************************
 * CONFIG WRITE COMMAND	  			  *
 **************************************/
static void configWriteCMD(int argc, char **argv)
{
	if (argc != 2){
		puts("RAMWRITE invalid arguments!\r\n");
		return;
	}else{
		uint8_t len = strlen((uchar*)&argv[1][0]);
		uint32_t value = (uint32_t)tinysh_dec((char*)&argv[1][0]);
		printf("DEC (%d): %ld \r\n", len, value);

		 userConfig.u32_crc = value;
	}
}


/**************************************
 * CONFIG READ COMMAND	  			  *
 **************************************/
static void configReadCMD(int argc, char **argv)
{
	printf("Config CRC: %ld LEN: %ld CRCN: %ld LENN: %ld\r\n", userConfig.u32_crc, userConfig.u32_len, userConfig.u32_crcN, userConfig.u32_lenN);
	//printf("Config CRC: %ld \r\n", userConfig.u32_crc);
}


static tinysh_cmd_t fwritecmd={0,"FWRITE","		[NONE]","[NONE]",fWriteCMD,0,0,0};
static tinysh_cmd_t freadcmd={0,"FREAD","		[NONE]","[NONE]",fReadCMD,0,0,0};
static tinysh_cmd_t ferasecmd={0,"FERASE","		[NONE]","[NONE]",fEraseCMD,0,0,0};
static tinysh_cmd_t ram2readcmd={0,"RAMREAD","		[NONE]","[NONE]",ram2ReadCMD,0,0,0};
static tinysh_cmd_t ram2writecmd={0,"RAMWRITE","		[NONE]","[NONE]",ram2WriteCMD,0,0,0};
static tinysh_cmd_t configWritecmd={0,"CFGWRITE","		[NONE]","[NONE]",configWriteCMD,0,0,0};
static tinysh_cmd_t configReadcmd={0,"CFGREAD","		[NONE]","[NONE]",configReadCMD,0,0,0};

void tinysh_init(void)
{
	puts("\e[1;1H\e[2J");
	puts("\r\n");
	puts("===========================| CONSOLE CMD |===============================\r\n");
	puts("type '?' or help for MANPAGE\r\n");

	tinysh_set_prompt("\r\n\nSEI$");
	tinysh_add_command(&fwritecmd);
	tinysh_add_command(&freadcmd);
	tinysh_add_command(&ferasecmd);
	tinysh_add_command(&ram2readcmd);
	tinysh_add_command(&ram2writecmd);
	tinysh_add_command(&configWritecmd);
	tinysh_add_command(&configReadcmd);
}



/* few useful utilities that may be missing */
void tinysh_char_out(unsigned char c)
{
  HAL_UART_Transmit(&huart2, (uint8_t *)&c, 1, 0xFFFF);	//Use UART1
}

static int strlen(uchar *s)	{
	int i;
	for(i=0;*s;s++,i++);
	return i;
}

static void puts(char *s)
{
	while(*s)
	    putchar(*s++);
}


/* callback for help function */
static void help_fnt(int argc, char **argv)
{
  //puts("\e[1;1H\e[2J");
  puts("\r\n");
  puts("===========================| HELP PAGE |===============================\r\n");
  puts("[COMMANDS] ?			CMD Help.\r\n");
  puts("CTRL-P or N			Previous/Next CMD.\r\n");
  puts("TAB				Auto complete.\r\n\n");
  puts("COMMANDS:       ARGUMENTS:	             NOTE:\r\n");
  puts("========================================================================\r\n");
  //puts("HW		[NONE]		READ LORA CONFIG.\r\n");
  //puts("SETRF		[P,B,S,C,T,R]	SET LORA CONFIG.\r\n");
  puts("SN		[10 chars]		SET Internal SN\r\n");
  puts("\r\n\n");

}

/*
 */

enum { NULLMATCH,FULLMATCH,PARTMATCH,UNMATCH,MATCH,AMBIG };

/* verify if the non-spaced part of s2 is included at the begining
 * of s1.
 * return FULLMATCH if s2 equal to s1, PARTMATCH if s1 starts with s2
 * but there are remaining chars in s1, UNMATCH if s1 does not start with
 * s2
 */
int strstart(uchar *s1, uchar *s2)
{
	while(*s1 && *s1==*s2) { s1++; s2++; }

	if(*s2==' ' || *s2==0)
	{
	  if(*s1==0)
		return FULLMATCH; /* full match */
	  else
		return PARTMATCH; /* partial match */
	}
	else
	return UNMATCH;     /* no match */
}

/*
 * check commands at given level with input string.
 * _cmd: point to first command at this level, return matched cmd
 * _str: point to current unprocessed input, return next unprocessed
 */
static int parse_command(tinysh_cmd_t **_cmd, uchar **_str)
{
  uchar *str=*_str;
  tinysh_cmd_t *cmd;
  tinysh_cmd_t *matched_cmd=0;

  /* first eliminate first blanks */
  while(*str==' ') str++;
  if(!*str)
    {
      *_str=str;
      return NULLMATCH; /* end of input */
    }

  /* first pass: count matches */
  for(cmd=*_cmd;cmd;cmd=cmd->next)
    {
      int ret=strstart((uchar*)cmd->name,str);

      if(ret==FULLMATCH)
        {
          /* found full match */
          while(*str && *str!=' ') str++;
          while(*str==' ') str++;
          *_str=str;
          *_cmd=cmd;
          return MATCH;
        }
      else if (ret==PARTMATCH)
        {
          if(matched_cmd)
            {
              *_cmd=matched_cmd;
              return AMBIG;
            }
          else
            {
              matched_cmd=cmd;
            }
        }
      else /* UNMATCH */
        {
        }
    }
  if(matched_cmd)
    {
      while(*str && *str!=' ') str++;
      while(*str==' ') str++;
      *_cmd=matched_cmd;
      *_str=str;
      return MATCH;
    }
  else
    return UNMATCH;
}

/* create a context from current input line
 */
static void do_context(tinysh_cmd_t *cmd, uchar *str)
{
  while(*str)
    context_buffer[cur_context++]=*str++;
  context_buffer[cur_context]=0;
  cur_cmd_ctx=cmd;
}

/* execute the given command by calling callback with appropriate
 * arguments
 */
static void exec_command(tinysh_cmd_t *cmd, uchar *str)
{
  char *argv[MAX_ARGS];
  int argc=0;
  int i;

  /* copy command line to preserve it for history */
  for(i=0;i<BUFFER_SIZE;i++)
    trash_buffer[i]=str[i];
  str=trash_buffer;

  /* cut into arguments */
  argv[argc++]=cmd->name;
  while(*str && argc<MAX_ARGS)
    {
      while(*str==' ') str++;
      if(*str==0)
        break;
      argv[argc++]=(char*)str;
      while(*str!=' ' && *str) str++;
      if(!*str) break;
      *str++=0;
    }
  /* call command function if present */
  if(cmd->function)
    {
      tinysh_arg=cmd->arg;
      cmd->function(argc,&argv[0]);
    }
}

/* try to execute the current command line
 */
static int exec_command_line(tinysh_cmd_t *cmd, uchar *_str)
{
  uchar *str=_str;

  while(1)
    {
      int ret;
      ret=parse_command(&cmd,&str);
      if(ret==MATCH) /* found unique match */
        {
          if(cmd)
            {
              if(!cmd->child) /* no sub-command, execute */
                {
                  puts("\r\n");
                  exec_command(cmd,str);
                  return 0;
                }
              else
                {
                  if(*str==0) /* no more input, this is a context */
                    {
                      do_context(cmd,_str);
                      return 0;
                    }
                  else /* process next command word */
                    {
                      cmd=cmd->child;
                    }
                }
            }
          else /* cmd == 0 */
            {
              puts("\r\n");
              return 0;
            }
        }
      else if(ret==AMBIG)
        {
          puts("AMBIGUITY: ");
          puts((char*)str);
          puts("\r\n");
          return 0;
        }
      else if(ret==UNMATCH) /* UNMATCH */
        {
          puts("\r\n  NO MATCH: ");
          puts((char*)str);
          puts("\r\n");
          return 0;
        }
      else /* NULLMATCH */
        return 0;
    }
}

/* display help for list of commands
*/
static void display_child_help(tinysh_cmd_t *cmd)
{
  tinysh_cmd_t *cm;
  int len=0;

  putchar('\n');
  for(cm=cmd;cm;cm=cm->next)
    if(len<strlen((uchar*)cm->name))
      len=strlen((uchar*)cm->name);
  for(cm=cmd;cm;cm=cm->next)
    if(cm->help)
      {
        int i;
        puts(cm->name);
        for(i=strlen((uchar*)cm->name);i<len+2;i++)
          putchar(' ');
        puts(cm->help);
        puts("\r\n");
      }
}

/* try to display help for current comand line
 */
static int help_command_line(tinysh_cmd_t *cmd, uchar *_str)
{
	uchar *str=_str;

	while(1)
	{
	  int ret;
	  ret=parse_command(&cmd,&str);
	  if(ret==MATCH && *str==0) 				/* found unique match or empty line */
	  {
		  if(cmd->child) 						/* display sub-commands help */
		  {
			  display_child_help(cmd->child);
			  return 0;
		  }
		  else  								/* no sub-command, show single help */
		  {
			  if(*(str-1)!=' ')
				  putchar(' ');
			  if(cmd->usage)
				  puts(cmd->usage);
			  puts(": ");
			  if(cmd->help)
				  puts(cmd->help);
			  else
				  puts("no help available");
			  	  putchar('\n');
		  }
		  return 0;
	  }
	  else if(ret==MATCH && *str)
	  {
		  /* continue processing the line */
	  cmd=cmd->child;
	}
	  else if(ret==AMBIG)
		{
		  puts("\r\nambiguity: ");
		  puts((char*)str);
		  puts("\r\n");
		  return 0;
		}
	  else if(ret==UNMATCH)
		{
		  puts("\r\nno match: ");
		  puts((char*)str);
		  puts("\r\n");
		  return 0;
		}
	  else /* NULLMATCH */
		{
		  if(cur_cmd_ctx)
			display_child_help(cur_cmd_ctx->child);
		  else
			display_child_help(root_cmd);
		  return 0;
		}
	}
}

/* try to complete current command line
 */
static int complete_command_line(tinysh_cmd_t *cmd, uchar *_str)
{
  uchar *str=_str;

  while(1)
  {
      int ret;
      int common_len=BUFFER_SIZE;
      int _str_len;
      int i;
      uchar *__str=str;

      ret=parse_command(&cmd,&str);
      for(_str_len=0;__str[_str_len]&&__str[_str_len]!=' ';_str_len++);
      if(ret==MATCH && *str){
          cmd=cmd->child;

  	  }else if(ret==AMBIG || ret==MATCH || ret==NULLMATCH)
        {
          tinysh_cmd_t *cm;
          tinysh_cmd_t *matched_cmd=0;
          int nb_match=0;

          for(cm=cmd;cm;cm=cm->next)
            {
              int r=strstart((uchar*)cm->name,__str);
              if(r==FULLMATCH)
                {
                  for(i=_str_len;cmd->name[i];i++)
                    tinysh_char_in(cmd->name[i]);
                  if(*(str-1)!=' ')
                    tinysh_char_in(' ');
                  if(!cmd->child)
                    {
                      if(cmd->usage)
                        {
                          puts(cmd->usage);
                          puts("\r\n");
                          return 1;
                        }
                      else
                        return 0;
                    }
                  else
                    {
                      cmd=cmd->child;
                      break;
                    }
                }
              else if(r==PARTMATCH)
                {
                  nb_match++;
                  if(!matched_cmd)
                    {
                      matched_cmd=cm;
                      common_len=strlen((uchar*)cm->name);
                    }
                  else
                    {
                      for(i=_str_len;cm->name[i] && i<common_len &&
                            cm->name[i]==matched_cmd->name[i];i++);
                      if(i<common_len)
                        common_len=i;
                    }
                }
            }
          if(cm)
            continue;
          if(matched_cmd)
            {
              if(_str_len==common_len)
                {
                  puts("\r\n");
                  for(cm=cmd;cm;cm=cm->next)
                    {
                      int r=strstart((uchar*)cm->name,__str);
                      if(r==FULLMATCH || r==PARTMATCH)
                        {
						  puts("  ");
						  puts(cm->name);
                          puts("\r\n");
                        }
                    }
                  return 1;
                }
              else
                {
                  for(i=_str_len;i<common_len;i++)
                    tinysh_char_in(matched_cmd->name[i]);
                  if(nb_match==1)
                    tinysh_char_in(' ');
                }
            }
          return 0;
        }
      else /* UNMATCH */
        {
          return 0;
        }
    }
}

/* start a new line
 */
static void start_of_line()
{
  /* display start of new line */
  puts(prompt);
  if(cur_context)
  {
     puts((char*)context_buffer);
     puts("> ");
  }
  cur_index=0;
}

/* character input
 */
static void _tinysh_char_in(uchar c)
{
  uchar *line=input_buffers[cur_buf_index];

  if(c=='\n' || c=='\r') /* validate command */
  {
      tinysh_cmd_t *cmd;

	  /* first, echo the newline */
      if(echo)
        putchar(c);

      while(*line && *line==' ') line++;
      if(*line) /* not empty line */
        {
          cmd=cur_cmd_ctx?cur_cmd_ctx->child:root_cmd;
          exec_command_line(cmd,line);
          cur_buf_index=(cur_buf_index+1)%HISTORY_DEPTH;
          cur_index=0;
          input_buffers[cur_buf_index][0]=0;
        }
      start_of_line();
  }
  else if(c==TOPCHAR) 			// return to top level //
  {
	//if(echo)
	//  putchar(c);
	//cur_context=0;
	//cur_cmd_ctx=0;

	  putchar('\r');
	  puts("SEI$");

	  while(cur_index > 0){
		  puts(" ");
		  cur_index--;
	  }
	  putchar('\r');
	  puts("SEI$");
	  cur_index=0;
	  line[cur_index]=0;
  }
  else if(c==8 || c==127) 		/* backspace */
  {
    if(cur_index>0)
    {
       puts("\b");
       puts("  ");
       puts("\b\b");
       cur_index--;
       line[cur_index]=0;
    }
  }
  else if(c==16) /* CTRL-P: back in history */
  {
      int prevline=(cur_buf_index+HISTORY_DEPTH-1)%HISTORY_DEPTH;

      if(input_buffers[prevline][0])
        {
          line=input_buffers[prevline];
          /* fill the rest of the line with spaces */
          while(cur_index-->strlen(line))
            puts("\b \b");
          putchar('\r');
          puts("SEI$");
          puts((char*)line);
          cur_index=strlen(line);
          cur_buf_index=prevline;
        }
  }

  else if(c==14) /* CTRL-N: next in history */
  {

	  int nextline=(cur_buf_index+1)%HISTORY_DEPTH;

	  if(input_buffers[nextline][0])
	  {
		  line=input_buffers[nextline];
		  /* fill the rest of the line with spaces */
		  while(cur_index-->strlen(line))
			puts("\b \b");
		  putchar('\r');
		  //start_of_line();
		  puts("SEI$");
		  puts((char*)line);
		  cur_index=strlen(line);
		  cur_buf_index=nextline;
	  }
  }

  else if(c=='?') 			// display help //
    {
	  puts("\r\n\n");
	  tinysh_cmd_t *cmd;
      cmd=cur_cmd_ctx?cur_cmd_ctx->child:root_cmd;
      help_command_line(cmd,line);
      start_of_line();
      puts((char*)line);
      cur_index=strlen(line);
    }
  else if(c==9) 			/* TAB: autocompletion */
  {
	  tinysh_cmd_t *cmd;
      cmd=cur_cmd_ctx?cur_cmd_ctx->child:root_cmd;
      if(complete_command_line(cmd,line))
      {
         start_of_line();
         puts((char*)line);
      }
      cur_index=strlen(line);
  }
  else /* any input character */
  {
    if(cur_index<BUFFER_SIZE)
    {
       if(echo)
         putchar(c);
       line[cur_index++]=c;
       line[cur_index]=0;
    }
  }
}

/* new character input */
void tinysh_char_in(uchar c)
{
  /*
   * filter characters here
   */
  _tinysh_char_in(c);
}

/* add a new command */
void tinysh_add_command(tinysh_cmd_t *cmd)
{
  tinysh_cmd_t *cm;

  if(cmd->parent)
    {
      cm=cmd->parent->child;
      if(!cm)
        {
          cmd->parent->child=cmd;
        }
      else
        {
          while(cm->next) cm=cm->next;
          cm->next=cmd;
        }
    }
  else if(!root_cmd)
    {
      root_cmd=cmd;
    }
  else
    {
      cm=root_cmd;
      while(cm->next) cm=cm->next;
      cm->next=cmd;
    }
}

/* modify shell prompt */
void tinysh_set_prompt(char *str)
{
  int i;
  for(i=0;str[i] && i<PROMPT_SIZE;i++)
    prompt[i]=str[i];

  prompt[i]=0;
  /* force prompt display by generating empty command */
  tinysh_char_in('\r');
}

/* return current command argument
 */
void *tinysh_get_arg()
{
  return tinysh_arg;
}


#endif 	//SHELL_CMD

/*********************************************************************
 * @name	: tinysh_longhex
 * @brief	: string to decimal/hexadecimal conversion
 *********************************************************************/
unsigned long tinysh_atoxi(char *s)
{
  int ishex=0;
  unsigned long res=0;

  if(*s==0) return 0;

  if(*s=='0' && *(s+1)=='x')
  {
      ishex=1;
      s+=2;
  }


  while(*s)
  {
	  if(ishex)
		  res*=16;
	  else
		  res*=10;

      if(*s>='0' && *s<='9')
    	  res+=*s-'0';
      else if(ishex && *s>='a' && *s<='f')
    	  res+=*s+10-'a';
      else if(ishex && *s>='A' && *s<='F')
    	  res+=*s+10-'A';
      else
    	  break;

      s++;
  }

  return res;
}


/*********************************************************************
 * @name	: tinysh_dec
 * @brief	: string to decimal conversion (1 bytes only)
 *********************************************************************/
unsigned long tinysh_dec(char *s)
{
  unsigned long res=0;
  uint8_t index = 0;

  while(*s)
  {
	  //printf("%c\r\n",*s);

	  res*=10;

	  if((*s == '0')&&(index == 0))
		  res = 0;
	  else if(*s>='0' && *s<='9')
		  res+=*s-'0';
	  else
		  break;

	  s++;
	  index++;

	  if(index > 10){
		 break;
	  }
  }

  return res;
}


/*********************************************************************
 * @name	: tinysh_longhex
 * @brief	: string to hexadecimal conversion (2 bytes only)
 *********************************************************************/
unsigned long tinysh_hex(char *s)
{
  //puts(s);
  unsigned long res=0;
  uint8_t index = 0;
  int ishex=1;

  while(*s)
  {
	  if(index >= 2){
		  break;
	  }

	  res*=16;

	  if((*s == '0')&&(index == 0))
		  res = 0;
	  else if(*s>='0' && *s<='9')
		  res+=*s-'0';
	  else if(ishex && *s>='a' && *s<='f')
		  res+=*s+10-'a';
	  else if(ishex && *s>='A' && *s<='F')
		  res+=*s+10-'A';
	  else
		  break;

      s++;
      index++;
  }

  return res;
}


/************************************************************
 * @name	: tinysh_longhex
 * @brief	: string to hexadecimal conversion (long)
 ************************************************************/
uint32_t tinysh_longhex(char *s)
{
	int ishex=1;
	unsigned long res=0;


	if(*s=='0')
	{
	  s+=1;
	}


	while(*s)
	{
	  res*=16;

	  if(*s>='0' && *s<='9')
		  res+=*s-'0';
	  else if(ishex && *s>='a' && *s<='f')
		  res+=*s+10-'a';
	  else if(ishex && *s>='A' && *s<='F')
		  res+=*s+10-'A';
	  else
		  break;

	  s++;
	}

	return res;
}


