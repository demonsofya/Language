#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "lib/tree/diff_dump.h"
#include "lib/tree/diff_struct.h"
#include "tokens.h"
#include "reading_tokens.h"
#include "assembling.h"
#include "middle_end.h"

#include "TXLib.h"

int main()
{
    system("chcp 1251");

    printf("%s", TranslateWordToEnglish("Ã¿Ã¿ Ã€À¿ –¿Ã” Œ◊≈Õ‹ ƒŒÀ√Œ ¿¡Œ¡¿"));

    //Tree_t *tree = GetGraph();
    //TreeDump(tree, __FILE__, __FUNCTION__, __LINE__, "Dump in main", 0);
    
    Tree_t *tree = GetTokensGraph();
    TreeDump(tree, __FILE__, __FUNCTION__, __LINE__, "Dump in main", 0);

    PutVariablesPosesInNametableInTree(tree);
    TreeDump(tree, __FILE__, __FUNCTION__, __LINE__, "Dump after putting poses from curr nametable", 0);

    CreateAsmFile(tree);

    return 0;
}
