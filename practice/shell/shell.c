#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct cmd_struct {
        char* cmd;
        char* argv[];
}cmd_struct_typedef;

int main()
{
        char input_buf[BUFSIZ];
        int ret = 0;
        void *p_ret = NULL;
        cmd_struct_typedef shell_struct;
        shell_struct.cmd = malloc(BUFSIZ);

        printf("@root:");

        if( fgets(input_buf, BUFSIZ, stdin) == NULL) {
                printf("fgets error!\n");
                exit(-1);
        }
        printf("input:%s\n",input_buf);

        p_ret = strchr(input_buf, ' ');
        if(p_ret == NULL) {
                printf("no find blank!\n");
                exit(-1);
        }
        printf("after blank:%s\n",(char *)p_ret);
        
        strncpy(shell_struct.cmd, input_buf, ((char *)p_ret - input_buf));
        printf("shell_struct.cmd:%s\n", shell_struct.cmd);

        return 0;
}
