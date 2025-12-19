#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "lib/tree/diff_dump.h"
#include "../diff_struct.h"
#include "lib/onegin/buffer.h"
#include "input_back.h"
#include "math.h"

Variable_t *variables_array = (Variable_t *) calloc(1, sizeof(Variable_t) * 1);
unsigned int variables_count = 0;

Function_t **functions_array = (Function_t **) calloc(1, sizeof(Function_t*) * 1);
unsigned int curr_function_num = -1;

unsigned int functions_count = 0;

const char *const DEFAULT_INPUT_FILE_NAME = "lang_front_output.txt";
const int MAX_COMMAND_SIZE = 100;

#define SYNTAX_ERROR { fprintf(stderr, "SYNTAX ERROR from " __FILE__ ":%d:0 at %s\n\n", __LINE__, __FUNCTION__); assert(NULL); }


Tree_t *GetBackendBaseFromFile() {

    size_t buffer_size = 0;
    char *buffer = GetBuffer(DEFAULT_INPUT_FILE_NAME, &buffer_size);

    if (buffer == NULL) {
        printf("Input file error\n");
        return NULL;
    }

ON_DEBUG(buffer_begin = buffer);

    Tree_t *tree = (Tree_t *) calloc(1, sizeof(Tree_t));
    tree->root = ReadCurrNode(&buffer);

    functions_count = curr_function_num + 1;
    CountFunctionParamsCount(tree->root);

    return tree;
}

void CountFunctionParamsCount(Node_t *node) {
    
    assert(node);

    if (node->type == NewFuncType) {
        CountFunctionParamsCount(node->left);
        CountFunctionParamsCount(node->right);

        return;
    }

    int params_count = 0;
    Node_t *curr_param_node = node->right;
    while (curr_param_node != NULL) {
        params_count++;

        curr_param_node = curr_param_node->left;
        if (curr_param_node == NULL)
            break;
    }

    functions_array[node->value.function_index_in_array]->params_count = params_count;
    functions_array[node->value.function_index_in_array]->variables_count += params_count;
ON_DEBUG(fprintf(stderr, "Curr params count is %d\n", params_count));
}

void SkipSpaces(char **curr_pos) {

    assert(curr_pos);

    while (isspace(**curr_pos) && **curr_pos != '\0')
        (*curr_pos)++;

ON_DEBUG(Print_Curr_Input_Information(*curr_pos, "Dump after skipping spaces"));

}

Node_t *ReadCurrNode(char **curr_symbol) {

    assert(curr_symbol);
    SkipSpaces(curr_symbol);

ON_DEBUG(Print_Curr_Input_Information(*curr_symbol, "Dump in the begin of recursion"));

    if (**curr_symbol == '(') {

ON_DEBUG(Print_Curr_Input_Information(*curr_symbol, "Dump in the begin of reading new node"));

        Node_t *new_node = (Node_t *) calloc(1, sizeof(Node_t));
        (*curr_symbol)++;

        size_t data_len = 0;
        sscanf(*curr_symbol, " \"%*[^\"]\"%n", &data_len);

        char *curr_data = (char *) calloc(data_len, sizeof(char));
        sscanf(*curr_symbol, " \"%[^\"]", curr_data);
ON_DEBUG(fprintf(stderr, "curr data is %s\n", curr_data));
ON_DEBUG(Print_Curr_Input_Information(*curr_symbol, "Dump ufter skipping ("));        
        new_node = CheckNodeTypeAndFillValue(curr_data);

        (*curr_symbol) += data_len;

ON_DEBUG(Print_Curr_Input_Information(*curr_symbol, "Tree dump after creating new node without sons"));
ON_DEBUG(TreeDumpCreateGraphFile(new_node));
ON_DEBUG(TreeDumpCreateGraphFile(new_node));
        new_node->left = ReadCurrNode(curr_symbol);
ON_DEBUG(TreeDumpCreateGraphFile(new_node));
        new_node->right = ReadCurrNode(curr_symbol);
ON_DEBUG(TreeDumpCreateGraphFile(new_node));
        SkipSpaces(curr_symbol);

        if (**curr_symbol != ')') {
            printf("Read from input file error: SYNTAX ERROR\n");
            return NULL;
        }
        (*curr_symbol)++;

ON_DEBUG(Print_Curr_Input_Information(*curr_symbol, "Dump after reading new node"));
ON_DEBUG(TreeDumpCreateGraphFile(new_node));

        return new_node;
    } else {
ON_DEBUG(Print_Curr_Input_Information(*curr_symbol, "Dump before nil"));

        if (strncmp(*curr_symbol, "nil", strlen("nil")) == 0)
            *curr_symbol += strlen("nil");
        else
            printf("Read from input file error: SYNTAX ERROR\n");

ON_DEBUG(Print_Curr_Input_Information(*curr_symbol, "Dump after nil"));

        return NULL;
    }
    return NULL;
}

Node_t *CheckNodeTypeAndFillValue(char *curr_data) {

    assert(curr_data);

    for (int i = 0; i < STANDARDS_COUNT; i++) 
        if (strncmp(curr_data, STANDARDS_ARRAY[i].standard_back_name, STANDARDS_ARRAY[i].standard_back_name_len) == 0) {
            curr_data += STANDARDS_ARRAY[i].standard_back_name_len;
            SkipSpaces(&curr_data);

            return STANDARDS_ARRAY[i].creating_back_node_func_ptr(curr_data);
        }

    SYNTAX_ERROR;
}

Node_t *GetEmptyNode(char *curr_data) {

    return CreateEmptyNode();
}

Node_t *GetFunctionCallNode(char *curr_data) {

    return CreateFuncCallNode();
}

Node_t *GetNewFunctionNode(char *curr_data) {
  
    return CreateNewFuncNode();
}

Node_t *GetComparisonOperatorNode(char *curr_data) {

    assert(curr_data);

    for (int i = 0; i < COMPARISON_OPERATORS_COUNT; i++) 
        if (strcmp(curr_data, COMPARISON_OPERATORS_ARRAY[i].comparison_back_standard_name) == 0)

            return CreateNewComparisonOperatorNode(NULL, NULL, COMPARISON_OPERATORS_ARRAY[i]);

    SYNTAX_ERROR;
    return NULL;
}

Node_t *GetServiceStructureNode(char *curr_data) {

    assert(curr_data);

    for (int i = 0; i < SERVICE_STRUCTURES_COUNT; i++) 
        if (strcmp(curr_data, SERVICE_STRUCTURES_ARRAY[i].service_back_standard_name) == 0)

            return CreateNewServiceNode(NULL, NULL, SERVICE_STRUCTURES_ARRAY[i]);

    SYNTAX_ERROR;
    return NULL;
}

Node_t *GetControlStructureNode(char *curr_data) {
    
    assert(curr_data);

    for (int i = 0; i < CONTROL_STRUCTURES_COUNT; i++)
        if (strcmp(curr_data, CONTROL_STRUCTURES_ARRAY[i].control_back_standard_name) == 0) 

            return CreateNewControlNode(NULL, NULL, CONTROL_STRUCTURES_ARRAY[i]);

    SYNTAX_ERROR;
    return NULL;
}

Node_t *GetOperationNode(char *curr_data) {

    assert(curr_data);

    for (int i = 0; i < OPERATIONS_COUNT; i++) 
        if (strcmp(curr_data, OPERATIONS_ARRAY[i].operation_back_standard_name) == 0)
            return CreateNewOperationNode(NULL, NULL, OPERATIONS_ARRAY[i]);

    SYNTAX_ERROR;
    return NULL;
}

Node_t *GetNumNode(char *curr_data) {

    assert(curr_data);

    double curr_num = 0;
    sscanf(curr_data, "%lf", &curr_num);

    return CreateNewNumNode(NULL, NULL, curr_num);
}

Node_t *GetVariableNode(char *curr_symbol) {
    
    assert(curr_symbol);

    size_t data_len = 0;
    sscanf(curr_symbol, " %*[^\"]%n", &data_len);

ON_DEBUG(Print_Curr_Input_Information(curr_symbol, "GET VARIABLE"));
    
    char *data = (char *) calloc(data_len, sizeof(char));
    
    if (!sscanf(curr_symbol, " %[^\"]", data)) {
        SYNTAX_ERROR;
        return NULL;
    } 

    int variable_hash = CountStringHashDJB2(data);
    for (int i = 0; i < variables_count; i++)
        if (variable_hash == variables_array[i].variable_hash)
            if (strcmp(data, variables_array[i].variable_name) == 0)

                return CreateNewVariableNode(NULL, NULL, i);

    functions_array[curr_function_num]->variables_count++;
    variables_count++;
    variables_array = (Variable_t *) realloc(variables_array, sizeof(Variable_t) * variables_count);
    variables_array[variables_count - 1] = {data, variables_count - 1, variable_hash, 0};      

    return CreateNewVariableNode(NULL, NULL, variables_count - 1);
}

Node_t *GetFunctionNode(char *curr_symbol) {

    assert(curr_symbol);

    curr_function_num++;
    size_t data_len = 0;
    sscanf(curr_symbol, " %*[^\"]%n", &data_len);

ON_DEBUG(Print_Curr_Input_Information(curr_symbol, "GET FUNCTION"));
    
    char *data = (char *) calloc(data_len, sizeof(char));
    
    if (!sscanf(curr_symbol, " %[^\"]", data)) {
        SYNTAX_ERROR;
        return NULL;
    } 

ON_DEBUG(fprintf(stderr, "Curr function name is %s\n", data));

    int function_hash = CountStringHashDJB2(data);
    for (int i = 0; i < functions_count; i++)
        if (function_hash == functions_array[i]->function_hash)
            if (strcmp(data, functions_array[i]->function_name) == 0) 
                return CreateFunctionNode(NULL, NULL, i);

    functions_count++;
    functions_array = (Function_t **) realloc(functions_array, sizeof(Function_t*) * functions_count);
    functions_array[functions_count - 1] = CreateNewFunction(data, functions_count - 1, function_hash);
    
    return CreateFunctionNode(NULL, NULL, functions_count - 1);
}

Function_t *CreateNewFunction(char *func_name, int func_num, int func_hash) {

    Function_t *new_function = (Function_t *) calloc(1, sizeof(Function_t));

    new_function->function_name = func_name;
    new_function->function_num = func_num;
    new_function->variables_table = (Variable_t *) calloc(1, sizeof(Variable_t));
    new_function->function_hash = func_hash;

    return new_function;
}

void PutVariablesPosesInNametableInTree(Tree_t *tokens_graph) {

    assert(tokens_graph);

    PrintFunctionVariablesPosesInNametable(tokens_graph->root);
}

void PrintFunctionVariablesPosesInNametable(Node_t *node) {

    if (node->type == NewFuncType) {
        PrintFunctionVariablesPosesInNametable(node->left);
        PrintFunctionVariablesPosesInNametable(node->right);

        return;
    }

    PutVariableNodePosInNametable(node->left, functions_array[node->value.function_index_in_array]);
    if (functions_array[node->value.function_index_in_array]->params_count > 0)
        PutVariableNodePosInNametable(node->right, functions_array[node->value.function_index_in_array]);
}

void PutVariableNodePosInNametable(Node_t *node, Function_t *curr_function_struct) {

    assert(node);

    if (node->type != VariableType) {
        if (node->left != NULL)
            PutVariableNodePosInNametable(node->left, curr_function_struct);

        if (node->right != NULL)
            PutVariableNodePosInNametable(node->right, curr_function_struct);
        
        return;
    }

    for (int i = 0; i < curr_function_struct->variables_count; i++)
        if (curr_function_struct->variables_table[i].variable_hash == GET_VARIABLE_HASH(node))
            if (strcmp(curr_function_struct->variables_table[i].variable_name, GET_VARIABLE_NAME(node)) == 0) {
                node->value.variable_index_in_array = i;
                return;
            }

    for (int i = 0; i < functions_count; i++)
        if (GET_VARIABLE_HASH(node) == functions_array[i]->function_hash)
            if (strcmp(GET_VARIABLE_NAME(node), functions_array[i]->function_name) == 0) {
                node->value.function_index_in_array = i;
                node->type = FunctionType;

                return;
            }

    SYNTAX_ERROR;
}