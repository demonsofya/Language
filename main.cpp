#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "lib/tree/diff_dump.h"
#include "lib/tree/diff_struct.h"
#include "tokens.h"
#include "reading_tokens.h"

#include "TXLib.h"

int main()
{
    system("chcp 1251");

    printf("%s", TranslateWordToEnglish("люлю лшкю пюлс нвемэ днкцн юанаю"));

    //Tree_t *tree = GetGraph();
    //TreeDump(tree, __FILE__, __FUNCTION__, __LINE__, "Dump in main", 0);
    
    Tree_t *tree = GetTokensGraph();
    TreeDump(tree, __FILE__, __FUNCTION__, __LINE__, "Dump in main", 0);

    return 0;
}
