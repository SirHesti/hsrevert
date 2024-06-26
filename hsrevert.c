#include "tools.h"
#include "VERSION.h"

#define LINEMAX 1024

int OPTOverwrite;

char InFile[PATH_MAX];
char OutFile[PATH_MAX];

FILE *S;
FILE *D;

char *Line;
void *LST;

char *DATA;
void *DUMP;
int i;

signed int main(int argc, char *argv[])
{
    if (InitTools(argc , argv, "%v%t", I_MAJOR, I_MINOR, I_BUILD, I_BETA, LOG_STDERR | LOG_SYSLLOG )) return -1;

    InitARG(argc);
    if (   (aChkARGlong("help"))
        || (aChkARG(".?"))
        || (argc<2)
       )
    {
        printf ("%s OPTIONS:\n",m_PRGNAME);
        printf ("  -i <InFile>   File to Read\n");
        printf ("  -o <OutFile>  File to Write\n");
        printf ("  -d            overwrite OutFile\n");
        printf ("or %s <InFile> <OutFile> maybe -d\n",m_PRGNAME);
        return 0;
    }

    OPTOverwrite = FALSE;
    InFile[0]=0;
    OutFile[0]=0;

    if (aChkARG("-i")) strcpy (InFile,ARG);
    if (aChkARG("-o")) strcpy (OutFile,ARG);
    if (aChkARG(".d")) OPTOverwrite=TRUE;

    if (!InFile[0])
    {
        if (aChkARG("**-"))
        {
            if (ARG) strcpy (InFile,ARG);
        }
    }

    if (!OutFile[0])
    {
        if (aChkARG("**-"))
        {
            if (ARG) strcpy (OutFile,ARG);
        }
    }
#ifdef HS_DEBUG
    if (0==1)
    {
        int b;
        char tmp[128];
        for (b=1;b<10;b++)
        {
            sprintf (tmp, "$%i", b);
            if (!aChkARG(tmp)) break;
            if (!ARG) break;
            //if (aChkARG("$2"))
            printf("  %s=%s\n", tmp, ARG);
        }
        for (b=1;b<10;b++)
        {
            sprintf (tmp, "#%i", b);
            if (!aChkARG(tmp)) break;
            if (!ARG) break;
            //if (aChkARG("$2"))
            printf("  %s=%s\n", tmp, ARG);
        }
        for (b=0;b<999;b++)
        {
            if (!aChkARG("**-")) break;
            if (!ARG) break;
            printf("%i**-=%s\n", b, ARG);
        }
        if (aChkARG("0-9")) printf(" 0-9=%s\n", ARG);
        printf ("  IN=%s\n OUT=%s\nOver=%s\n", InFile, OutFile, OPTOverwrite?"True":"False");
        aarg_unused_print();
        arg_Clean();
        return 0;
    }
#endif // HS_DEBUG
    if (aarg_unused_print()) return 99;;
    arg_Clean();

    if (!FileOK(InFile))
    {
        lprintf("InFile not found");
        return 1;
    }

    if (FileOK(OutFile))
    {
        if (OPTOverwrite!=TRUE)
        {
            lprintf("OutFile exists -D not given so don't overwrite");
            return 2;
        }
    }
    if(( S = fopen(InFile, "rt")) == NULL )
    {
        lprintf ("InFile can't read '%s'", InFile);
        return 3;
    }
    LST=NULL;
    for (;;)
    {
        Line = malloc(LINEMAX);                                                 // Speicher für Zeile holen
        if (mgetline(S,Line, LINEMAX-1)!=0)                                     // EOF ??
        {
            free(Line);
            break;
        }
        if (Line[0]==0)
        {
            free(Line);
            continue;
        }
        LST=Node_Add(LST, Line, false);
    }
    fclose(S);
    if(( D = fopen(OutFile, "wt")) == NULL )
    {
        lprintf ("OutFile can't write '%s'", OutFile);
        return 4;
    }
    DUMP = LST;
    for (i=1;;i++)
    {
        if (!DUMP) break;
        DATA = Node_GetData(DUMP);
        DUMP = Node_GetNext(DUMP);
        fprintf(D,"%s\n", DATA);
    }
    fclose (D);
    lprintf ("Done %i Rows reverted IN=%s OUT=%s",i-1, InFile, OutFile);
    return 0;
}
