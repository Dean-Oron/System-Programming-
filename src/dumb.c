#include <stdio.h>
#include <unistd.h>

/*
echo 0 | sudo tee /proc/sys/kernel/randomize_va_space
- disable aslr
echo 2 | sudo tee /proc/sys/kernel/randomize_va_space
- enable aslr
*/

int main(int argc, char **argv, char **envp)
{
    int i = 7;
    int j = 12;
    int k = 15;
    int *p = &j;

    FILE *fp;

    if(0 == (fp = fopen("stoopid", "w")))
    {

        printf("well, that didn\'t work!\n");
        return -1;
    }

    fprintf(fp, "%p\n", p);

    if(fclose(fp))
    {
          printf("oh well.");
          return -1;
    }

    sleep(1);

    return 0;
}
