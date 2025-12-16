#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>

#include "diff_dump.h"
#include "diff_struct.h"

//=============================================================================

FILE *LOGFILE = OpenLogFile();

const char *DEFAULT_LOG_FILE_NAME = "Programm_lenguage_LogFile.html";

//-----------------------------------------------------------------------------

// TODO: move to constants.cpp
const char *DARK_RED_COLOR             = "990000";
const char *PURPLE_BLUE_COLOR          = "6666FF";
const char *LIGHT_PURPLE_PINK_COLOR    = "baacc7";
const char *SOFT_PURPLE_COLOR          = "876f9e";
const char *LIGHT_BLUE_COLOR           = "99CCFF";
const char *DEEP_BLUE_COLOR            = "0066cc";
const char *ERROR_RED_COLOR            = "CC0000";
const char *LIGHT_PASTEL_RED_COLOR     = "FF6666";
const char *HOLY_WIGHT_COLOR           = "f5f5dc";
const char *GRASS_GREEN_COLOR          = "4C9900";
const char *LAVANDER_BLUE_COLOR        = "ccccff";
const char *COBALITE_COLOR             = "9999ff";
const char *LIGHT_YELLOW_GREEN_COLOR   = "CCFF99";
const char *PASTEL_PURPLE_PINK_COLOR   = "cfa6e0";
const char *LIGHT_GRAY_PURPLE_COLOR    = "e3cced";
const char *BRIGHT_GREEN_COLOR         = "66CC00";
const char *LAVANDER_PINK_COLOR        = "fff0f5";
const char *SOFT_PINK_COLOR            = "ff97bb";

const int MAX_DOT_NAME_SIZE      = 100;
const int MAX_DOT_FILE_NAME_SIZE = 100;
const int MAX_DOT_COMMAND_SIZE   = 100;


//-----------------------------------------------------------------------------

FILE* OpenLogFile() {

    FILE *file_ptr = fopen(DEFAULT_LOG_FILE_NAME, "w");
    atexit(CloseLogFile);

    return file_ptr;
}

void CloseLogFile() {

    fclose(SEREGA);
    printf("Log file closed successfully\n");
}

//=============================================================================

int DiffTreeVerify(Tree_t *tree) {

    int error = NoDiffError;

    if (tree == NULL)
        return DiffTreePtrError;

    if (tree->root == NULL)
        error |= DiffRootPtrError;

    if (tree->nodes_count <= 0)
        error |= DiffTreeSizeError;

    return error;
}

//=============================================================================

void TreeDump(Tree_t *tree, const char *file_name, const char *function_name,
                  int line_number, const char *calling_reason_string, int curr_error) {

    TreeDumpPrintErrors(tree, file_name, function_name, line_number, calling_reason_string, curr_error);

    if (tree != NULL)
        TreeDumpCreateGraphFile(tree->root);
}

//-----------------------------------------------------------------------------

void TreeDumpPrintErrors(Tree_t *tree, const char *file_name, const char *function_name,
                             int line_number, const char *calling_reason_string, int curr_error) {

    int error = DiffTreeVerify(tree) | curr_error;

    fprintf(SEREGA, "<h2><font color=\"#%s\"><p align=\"center\">===========DUMP==========="
                    "</p></font></h2>\n<h3><p align=\"center\">%s</p></h3>\n\n",
                    ERROR_RED_COLOR, calling_reason_string);

    fprintf(SEREGA, "<h4>TreeDump() from %s at %s:%d:</h4>\n<pre>", file_name, function_name,
                                                                    line_number);

    if (tree == NULL || error & DiffTreePtrError) {
        fprintf(SEREGA, "\nERROR: tree pointer error. Tree pointer is [%p]\n\n", tree);
        return;
    }

    fprintf(SEREGA, "Diff tree [%p]\n", tree);

    if (error & DiffTreeSizeError)
        fprintf(SEREGA, "\nERROR: tree array size error. Size is %d\n\n", tree->nodes_count);
    else
        fprintf(SEREGA, "tree size is %d\n\n", tree->nodes_count);

    if (error & DiffRootPtrError || tree->root == NULL)
        fprintf(SEREGA, "\nERROR: tree root error. Root pointer is [%p]\n\n", tree->root);
    else
        fprintf(SEREGA, "Root pointer is [%p]\n\n", tree->root);

    if (error & DiffNodePtrError)
        fprintf(SEREGA, "ERROR: error with tree node\n\n");


    fprintf(SEREGA, "</pre>\n");
}

//-----------------------------------------------------------------------------

void TreeDumpCreateGraphFile(Node_t *root) {

    if (root == NULL)
        return;

    char *DotFileName = CreateDotFileName("txt");
    char *DotFilePngName = CreateDotFileName("png");

    if (DotFileName == NULL || DotFilePngName == NULL) {
        fprintf(SEREGA, "ERROR: pointer dot file name error\n\n");
        return;
    }


    FILE *dot_file_ptr = fopen(DotFileName, "w");

    if (dot_file_ptr == NULL) {
        fprintf(SEREGA, "ERROR: dot file pointer error\n\n");
        return;
    }

    PrintDotFileHeader(dot_file_ptr, DotFilePngName);
    DrawDotNode(root, dot_file_ptr, 1, 1);
    PrintDotFileEnd(dot_file_ptr);

    fclose(dot_file_ptr);

    CreateImageFromDotFile(DotFileName, DotFilePngName);

    free(DotFileName);
    free(DotFilePngName);
}

//-----------------------------------------------------------------------------


char *CreateDotFileName(const char *file_type) {

    static int dot_file_number = 0;

    char *file_name = (char *) calloc(MAX_DOT_FILE_NAME_SIZE, sizeof(char));

    snprintf(file_name, MAX_DOT_FILE_NAME_SIZE, "DotFileNum_%d.%s", dot_file_number, file_type);

    dot_file_number++;

    return file_name;
}

//-----------------------------------------------------------------------------

void PrintDotFileHeader(FILE *dot_file_ptr, char *DotFilePngName) {

    assert(dot_file_ptr);
    assert(DotFilePngName);

    fprintf(SEREGA, "<img src=\"%s\" />\n", DotFilePngName);

    fprintf(dot_file_ptr, "digraph {\nrankdir=\"TB\";\n");
    fprintf(dot_file_ptr, "node [shape = \"Mrecord\", style = \"filled\", fillcolor = \"#%s\""
                          ", color = \"#%s\", fontcolor = red, fontsize=20, margin=0.1];\n", LAVANDER_PINK_COLOR, SOFT_PINK_COLOR);
}

//-----------------------------------------------------------------------------

int DrawDotNode(const Node_t *node, FILE *dot_file_ptr, int rank_num, int node_num) {

    assert(node);
    assert(dot_file_ptr);

    char *curr_node_name = GetNodeName(node_num);
    int curr_node_num = node_num;

    assert(curr_node_name);

    switch (node->type) {
        case OperationType:
            switch (GET_OPERATION_ARGUMENTS_COUNT(node)) {
            
            case Binary:
                fprintf(dot_file_ptr, "%s [label = \"{ type = OPERATION [%d] | value = %s [%d] | { <left> left | <right> right } | [%p] }\", rank = %d]\n",
                                curr_node_name, node->type, TranslateWordToEnglish(node->value.operation_type_value.operation_name), 
                                node->value.operation_type_value.operation_type, node, rank_num);    
            
                break;

            case Unary:
                fprintf(dot_file_ptr, "%s [label = \"{ type = OPERATION [%d] | value = %s [%d] | { <left> left | 0 } | [%p]  }\", rank = %d]\n",
                                curr_node_name, node->type,  TranslateWordToEnglish(node->value.operation_type_value.operation_name), 
                                node->value.operation_type_value.operation_type, node, rank_num); 
                break;

            default:
                fprintf(dot_file_ptr, "%s [label = \"{ type = OPERATION [?] | value = [?]] | { ? | ? } | [?]  }\", rank = %d]\n", curr_node_name, rank_num);
                
                free(curr_node_name);
                return curr_node_num;
            }

            break;

        case VariableType:
            fprintf(dot_file_ptr, "%s [label = \"{ type = VAR [%d] | value = %d | %s = %.0lf [%d] | { 0 | 0 } | [%p]  }\", rank = %d]\n",
                                curr_node_name,  node->type, node->value.variable_index_in_array,variables_array[node->value.variable_index_in_array].variable_name, variables_array[node->value.variable_index_in_array].variable_value,
                                node->value.variable_index_in_array, node, rank_num); 
            break;

        case NumberType:
            fprintf(dot_file_ptr, "%s [label = \"{ type = NUM [%d] | value = %.0lf | { 0 | 0 } | [%p]  }\", rank = %d]\n",
                                curr_node_name, node->type, node->value.num_type_value, node, rank_num);
            break;

        case ServiceType:
             fprintf(dot_file_ptr, "%s [label = \"{ type = SERVICE [%d] | value = %s [%d] | { <left> left | <right> right } | [%p]  }\", rank = %d]\n",
                                curr_node_name, node->type, TranslateWordToEnglish(node->value.service_type_value.service_structure_name), 
                                node->value.service_type_value.service_structure_type, node, rank_num); 
             break;
                                
        case ComparisonOperatorType:
             fprintf(dot_file_ptr, "%s [label = \"{ type = COMPARISON [%d] | value = %s [%d] | { <left> left | <right> right } | [%p]  }\", rank = %d]\n",
                                curr_node_name, node->type, TranslateWordToEnglish(node->value.operation_type_value.operation_name), 
                                node->value.operation_type_value.operation_type, node, rank_num); 
             break;                                

        case ControlStructureType:
             fprintf(dot_file_ptr, "%s [label = \"{ type = CONTROL [%d] | value = %s [%d] | { <left> left | <right> right } | [%p]  }\", rank = %d]\n",
                                curr_node_name, node->type, TranslateWordToEnglish(node->value.control_structure_type_value.control_structure_name), 
                                node->value.control_structure_type_value.control_structure_type, node, rank_num); 
             break;

        case LogicalOperatorType:
             fprintf(dot_file_ptr, "%s [label = \"{ type = LOGICAL [%d] | value = %s [%d] | { <left> left | <right> right } | [%p]  }\", rank = %d]\n",
                                curr_node_name, node->type, TranslateWordToEnglish(node->value.logical_operator_type_value.logical_operator_name), 
                                node->value.logical_operator_type_value.logical_operator_type, node, rank_num); 
             break;

        case FunctionCallType:
            fprintf(dot_file_ptr, "%s [label = \"{ type = CALL [%d] | { <left> left | <right> right } | [%p]  }\", rank = %d]\n",
                                curr_node_name, node->type, node, rank_num); 
            break;

        case NewFuncType:
            fprintf(dot_file_ptr, "%s [label = \"{ type = NEW FUNC [%d] | { <left> left | <right> right } | [%p]  }\", rank = %d]\n",
                                curr_node_name, node->type, node, rank_num); 
            break;

        case EmptyOperatorType:
            fprintf(dot_file_ptr, "%s [label = \"{ type = EMPTY NODE [%d] | { <left> left | <right> right } | [%p]  }\", rank = %d]\n",
                                curr_node_name, node->type, node, rank_num); 
            break;   
        
        case FunctionType:
            fprintf(dot_file_ptr, "%s [label = \"{ type = FUNC [%d] | value = %s [%d] | params %d | { <left> left | <right> right } | [%p]  }\", rank = %d]\n",
                                curr_node_name,  node->type, functions_array[node->value.function_index_in_array]->function_name, node->value.function_index_in_array, functions_array[node->value.function_index_in_array]->params_count,
                                node, rank_num); 
            break;

        default:
            fprintf(stderr, "%s [label = \"{ type = ? [%d] | value = %.0lf | { 0 | 0 } | [%p]  }\", rank = %d]\n",
                            curr_node_name, node->type, node->value.num_type_value, node, rank_num);
            free(curr_node_name);
            return curr_node_num;
    }
    
    if (node->left != NULL) {
        node_num = DrawDotNode(node->left, dot_file_ptr, rank_num + 1, node_num + 1);
        char *left_node_name = GetNodeName(curr_node_num + 1);
        fprintf(dot_file_ptr,  "%s:<left> -> %s\n", curr_node_name, left_node_name);
        curr_node_num = node_num;
        free(left_node_name);
    }

    if (node->right != NULL) {
        node_num = DrawDotNode(node->right, dot_file_ptr, rank_num + 1, node_num + 1);
        char *right_node_name = GetNodeName(curr_node_num + 1);
        fprintf(dot_file_ptr,  "%s:<right> -> %s\n", curr_node_name, right_node_name);
        free(right_node_name);
    }

    free(curr_node_name);
    return node_num;
}

//-----------------------------------------------------------------------------

void PrintDotFileEnd(FILE *dot_file_ptr) {

    if (dot_file_ptr == NULL)
        return;

    fprintf(dot_file_ptr, "}");
}

//=============================================================================

void CreateImageFromDotFile(char *DotFileName, char *DotFilePngName) {

    if (DotFileName == NULL || DotFilePngName == NULL)
        return;

    char command[MAX_DOT_COMMAND_SIZE] = "";
    snprintf(command, MAX_DOT_COMMAND_SIZE, "dot %s -T png > %s", DotFileName, DotFilePngName);

    //char command_change_encoding[MAX_DOT_COMMAND_SIZE] = "";
    //snprintf(command_change_encoding, MAX_DOT_COMMAND_SIZE, "CMD /U /C Type \"%s\" > \"aboba.txt\"", DotFileName);

    //fopen 

    //system(command_change_encoding);
    system(command);

//ON_DEBUG(fprintf(stderr, "%s\n", command));
}

//=============================================================================

char *GetNodeName(int node_num) {

    char *node = (char *) calloc(MAX_DOT_NAME_SIZE, sizeof(char));
    if (node == NULL)
        return node;

    snprintf(node, MAX_DOT_NAME_SIZE, "node_%d", node_num);

    return node;
}

char *TranslateWordToEnglish(const char *rus_word) {

    assert(rus_word);

    int rus_len = strlen(rus_word);

    char *eng_word = (char *) calloc(rus_len * 4, sizeof(char));
    char *eng_pos = eng_word;

    if (!('À' <= *rus_word && *rus_word <= 'ß')) {
        strcpy(eng_word, rus_word);
        return eng_word;
    }

    static const char *translate_array[33] = {
        "A", "B", "V", "G", "D", "E", "Zh", "Z", "I", "I", "K", "L", "M", "N", "O", "P", "R", "S", "T", "U", "F", "Kh", "Ts", "Ch", "Sh", "Shch", "", "Y", "", "E", "Iu", "Ia"
    };

    for (int i = 0; i < rus_len; i++) {

        if (isspace(rus_word[i])) {
            *eng_pos = ' ';
            eng_pos++;
            continue;
        }
//ON_DEBUG(printf("Curr symbol num is %d, russian A is %d", rus_word[i], 'À'));

        strcpy(eng_pos, translate_array[(rus_word[i] - 'À') % 33]);
        eng_pos += strlen(translate_array[(rus_word[i] - 'À') % 33]);
    }

    eng_pos = '\0';

    return eng_word;
}
