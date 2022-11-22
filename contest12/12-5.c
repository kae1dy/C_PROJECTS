#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/time.h>
#include <stdio.h>
#include <limits.h>

#define errorExit(message) fprintf(stderr, "%s\n", message), exit(EXIT_FAILURE);

enum 
{ 
    CHMOD = 0700,
    ARG_MIN = 2,
    BUF_SIZE = 20
};

const char script_body[] = "#!/bin/bash\ngcc";

const char header[] = "#include <stdio.h>\n"
                        "int\nmain(void)\n"
                        "{\nint x;\n"
                        "const char summon[] = \"summon\", "
                        "disqualify[] = \"disqualify\", "
                        "reject[] = \"reject\";\n";

const char footer[] = "}";

int
getPath(char *dist_c, char *dist_scr, char *dist_out, size_t size, char *def) {
    char *dirpath;
    struct timeval timenow;

    if (gettimeofday(&timenow, NULL) < 0) return -1;
     
    dirpath = (getenv("XDG_RUNTIME_DIR") != NULL) ? getenv("XDG_RUNTIME_DIR") : getenv("TMPDIR");
    if (!dirpath) dirpath = def;

    if (snprintf(dist_c, size, "%s/%ld.c", dirpath, timenow.tv_usec) < 0) return -1;
    if (snprintf(dist_scr, size, "%s/%ld", dirpath, timenow.tv_usec) < 0) return -1;
    if (snprintf(dist_out, size, "%s/%ld.o", dirpath, timenow.tv_usec) < 0) return -1;

    return 0;
}

int
main(int argc, char *argv[])
{   
    if (argc != ARG_MIN) errorExit("not enough/too many arguments.");

    char path_script[PATH_MAX] = {}, path_cfile[PATH_MAX] = {}, path_out[PATH_MAX] = {};

    if (getPath(path_cfile, path_script, path_out, PATH_MAX, "/tmp/") < 0) errorExit("error creating file in temp.");
    
    int fdscript = open(path_script, O_WRONLY | O_CREAT | O_TRUNC | O_CLOEXEC, CHMOD);
    FILE *script = fdopen(fdscript, "w");
    FILE *cfile = fopen(path_cfile, "w");

    if (fdscript < 0 || !script || !cfile) errorExit("error creating file in temp.");

    if (fprintf(script, "%s %s -o %s\n%s\nrm %s*", script_body, path_cfile, path_out, path_out, path_script) < 0) {
        errorExit("file write error.");
    }
    if (fprintf(cfile, "%s", header) < 0) errorExit("file write error.");;

    int buf;
    while (scanf("%d", &buf) == 1) { 
        if (fprintf(cfile, "x = %d;\nprintf(\"%%s\\n\", %s);\n", buf, argv[1]) < 0) {
            errorExit("file write error.");
        }
    }
    if (fprintf(cfile, "%s", footer) < 0) errorExit("file write error.");
    if (fclose(script) < 0 || fclose(cfile) < 0) errorExit("file close error.");

    execl(path_script, path_script, NULL);
    errorExit("execute error.");
}