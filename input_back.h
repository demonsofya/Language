#ifndef INPUT_BACK_H_INCLUDED
#define INPUT_BACK_H_INCLUDED

#include "../diff_struct.h"

Tree_t *GetBackendBaseFromFile();
Node_t *ReadCurrNode(char **curr_symbol);

void SkipSpaces(char **curr_pos);

Node_t *CheckNodeTypeAndFillValue(char *curr_data);

Node_t *GetVariableNode(char *curr_symbol);
Node_t *GetFunctionNode(char *curr_symbol);
Node_t *GetControlStructureNode(char *curr_data);
Node_t *GetOperationNode(char *curr_data);
Node_t *GetNumNode(char *curr_data);
Node_t *GetEmptyNode(char *curr_data);
Node_t *GetFunctionCallNode(char *curr_data);
Node_t *GetNewFunctionNode(char *curr_data);
Node_t *GetComparisonOperatorNode(char *curr_data);
Node_t *GetServiceStructureNode(char *curr_data);

Function_t *CreateNewFunction(char *func_name, int func_num, int func_hash);

void CountFunctionParamsCount(Node_t *node);

struct Standard_t {
    const char *standard_back_name;
    int         standard_back_hash;
    int         standard_back_name_len;
    Node_t *(*creating_back_node_func_ptr) (char *curr_data);
};

Standard_t const STANDARDS_ARRAY[] = {
    {"OP",      CountStringHashDJB2("OP"),      strlen("OP"),       GetOperationNode            },
    {"VAR",     CountStringHashDJB2("VAR"),     strlen("VAR"),      GetVariableNode             },
    {"NUM",     CountStringHashDJB2("NUM"),     strlen("NUM"),      GetNumNode                  },
    {"SERV",    CountStringHashDJB2("SERV"),    strlen("SERV"),     GetServiceStructureNode     },
    {"COMP",    CountStringHashDJB2("COMP"),    strlen("COMP"),     GetComparisonOperatorNode   },
    {"CONTROL", CountStringHashDJB2("CONTROL"), strlen("CONTROL"),  GetControlStructureNode     },
    {"FUNC",    CountStringHashDJB2("FUNC"),    strlen("FUNC"),     GetFunctionNode             },
    {";",       CountStringHashDJB2(";"),       strlen(";"),        GetEmptyNode                },
    {"CALL",    CountStringHashDJB2("CALL"),    strlen("CALL"),     GetFunctionCallNode         },
    {"NEWFUNC", CountStringHashDJB2("NEWFUNC"), strlen("NEWFUNC"),  GetNewFunctionNode          }
};

const int STANDARDS_COUNT = sizeof(STANDARDS_ARRAY) / sizeof(Standard_t);

void PutVariablesPosesInNametableInTree(Tree_t *tokens_graph);
void PrintFunctionVariablesPosesInNametable(Node_t *node);
void PutVariableNodePosInNametable(Node_t *node, Function_t *curr_function_struct);

#endif