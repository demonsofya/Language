#ifndef DIFF_DUMP_H_INCLUDED
#define DIFF_DUMP_H_INCLUDED

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "diff_struct.h"

//=============================================================================
#define DEBUG

#ifdef DEBUG
    #define ON_DEBUG(...) __VA_ARGS__
#else
    #define ON_DEBUG(...)
#endif

#define Return_If_Diff_Error(diff_tree)                                                     \
    {                                                                                       \
        int error = DiffTreeVerify(diff_tree);                                              \
        if (error != 0) {                                                                   \
            TreeDump(diff_tree, __FILE__, __FUNCTION__, __LINE__, "", NoDiffError);     \
            return error;                                                                   \
        }                                                                                   \
    }

#define Return_Diff_Error(diff_tree)                                                        \
    {                                                                                       \
        int error = DiffTreeVerify(diff_tree);                                              \
        if (error != 0)                                                                     \
            TreeDump(diff_tree, __FILE__, __FUNCTION__, __LINE__, "", NoDiffError);     \
        return error;                                                                       \
    }

//-----------------------------------------------------------------------------
//---------DEBUG----------
static char *buffer_begin = NULL;

#define Print_Curr_Input_Information(curr_symbol, dump_information_string)                      \
    {                                                                                           \
        fprintf(SEREGA, "<h3><font color=\"#%s\"><p align=\"center\">Input DUMP"                \
                    "</p></font></h3><h4><p align=\"center\">%s</p></h4>",                      \
                    ERROR_RED_COLOR, dump_information_string);                                  \
        fprintf(SEREGA, "<h5>Input dump from %s at %s:%d:</h5>",                                \
        __FILE__, __FUNCTION__, __LINE__);                                                      \
        char help_curr_symbol = *curr_symbol;                                                   \
        *curr_symbol = '\0';                                                                    \
        fprintf(SEREGA, "<h6><font color=\"#%s\"><p align=\"left\">"                            \
                "%s</font>", LAVANDER_BLUE_COLOR, buffer_begin);                                \
        *curr_symbol = help_curr_symbol;                                                        \
        fprintf(SEREGA, "<font color=\"#%s\">"                                                  \
                "%s</font></p></h6>", DEEP_BLUE_COLOR, curr_symbol);                            \
    }

//-----------------------------------------------------------------------------
//-----CONSTANTS--------

extern const char *DARK_RED_COLOR;
extern const char *PURPLE_BLUE_COLOR;
extern const char *LIGHT_PURPLE_PINK_COLOR;
extern const char *SOFT_PINK_COLOR;
extern const char *LIGHT_BLUE_COLOR;
extern const char *DEEP_BLUE_COLOR;
extern const char *ERROR_RED_COLOR;
extern const char *LIGHT_PASTEL_RED_COLOR;
extern const char *HOLY_WIGHT_COLOR;
extern const char *GRASS_GREEN_COLOR;
extern const char *LAVANDER_BLUE_COLOR;
extern const char *COBALITE_COLOR;
extern const char *LIGHT_YELLOW_GREEN_COLOR;
extern const char *PASTEL_PURPLE_PINK_COLOR;
extern const char *LIGHT_GRAY_PURPLE_COLOR;
extern const char *BRIGHT_GREEN_COLOR;
extern const char *LAVANDER_PINK_COLOR;
extern const char *SOFT_PINK_COLOR;

extern const int MAX_DOT_NAME_SIZE;
extern const int MAX_DOT_FILE_NAME_SIZE;
extern const int MAX_DOT_COMMAND_SIZE;
//-----------------------------------------------------------------------------
//-----LOGFILE----------

FILE* OpenLogFile();
void CloseLogFile();

#define SEREGA LOGFILE
extern FILE *LOGFILE;

extern const char *DEFAULT_LOG_FILE_NAME;
//-----------------------------------------------------------------------------

enum DiffErrors {
    NoDiffError         = 0,
    DiffTreePtrError    = 1 << 0,
    DiffRootPtrError    = 1 << 1,
    DiffNodePtrError    = 1 << 2,
    DiffTreeSizeError   = 1 << 3,
    InsertDiffError     = 1 << 5,
    ObjectDiffError     = 1 << 6
};

int DiffTreeVerify(Tree_t *tree);
void TreeDump(Tree_t *tree, const char *file_name, const char *function_name,
              int line_number, const char *calling_reason_string, int curr_error);

void TreeDumpPrintErrors(Tree_t *tree, const char *file_name, const char *function_name,
                         int line_number, const char *calling_reason_string, int curr_error);

void TreeDumpCreateGraphFile(Node_t *root);
char *CreateDotFileName(const char *file_type);

void PrintDotFileHeader(FILE *dot_file_ptr, char *DotFilePngName);
int DrawDotNode(const Node_t *node, FILE *dot_file_ptr, int rank_num, int node_num);
void PrintDotFileEnd(FILE *dot_file_ptr);

void CreateImageFromDotFile(char *DotFileName, char *DotFilePngName);

char *GetNodeName(int node_num);

char *TranslateWordToEnglish(const char *rus_word);

#endif // DIFF_DUMP_H_INCLUDED
