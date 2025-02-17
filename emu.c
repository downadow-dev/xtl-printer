#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *xfopen(char *path, char *mode) {
    FILE *f;
    if((f = fopen(path, mode)) == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    return f;
}

int main(int argc, char **argv) {
    if(argc < 3) {
        fprintf(stderr, "usage: %s /path/to/iiixmish2/.comm /path/to/iiixmish2/.comm2\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    FILE *f;
    for(;;) {
        f = xfopen(argv[2], "rb");
        
        if(fgetc(f) != 1) {
            fclose(f);
            continue;
        }
        
        char req = fgetc(f);
        int c;
        switch(req) {
        case 1: /* INFO */
            fclose(f);
            f = xfopen(argv[1], "wb");
            fputs("\x01virtual printer by downadow", f);
            fclose(f);
            for(;;) {
                f = xfopen(argv[2], "rb");
                if(fgetc(f) == 0) {
                    fclose(f);
                    f = xfopen(argv[1], "wb");
                    fputc(0, f);
                    fclose(f);
                    break;
                }
                fclose(f);
            }
            break;
        case 2: /* WRITE */
            while((c = fgetc(f)) != EOF) {
                if(c == '\0') break;
                putchar(c);
            }
            
            fclose(f);
            
            f = xfopen(argv[1], "wb");
            fputc(1, f);
            fclose(f);
            
            for(;;) {
                f = xfopen(argv[2], "rb");
                if(fgetc(f) == 0) {
                    fclose(f);
                    f = xfopen(argv[1], "wb");
                    fputc(0, f);
                    fclose(f);
                    break;
                }
                fclose(f);
            }
            break;
        case 3: /* OFF */
            f = xfopen(argv[1], "wb");
            fputc(1, f);
            fclose(f);
            
            for(;;) {
                f = xfopen(argv[2], "rb");
                if(fgetc(f) == 0) {
                    fclose(f);
                    f = xfopen(argv[1], "wb");
                    fputc(0, f);
                    fclose(f);
                    break;
                }
                fclose(f);
            }
            
            exit(EXIT_SUCCESS);
        }
    }
}

