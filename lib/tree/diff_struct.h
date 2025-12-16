#ifndef DIFF_STRUCT_H_INCLUDED
#define DIFF_STRUCT_H_INCLUDED

#define GET_NODE_NUM_VALUE(node) node->value.num_type_value

#define GET_OPERATION_FUNC_PTR(node) node->value.operation_type_value.operation_func_ptr
#define GET_OPERATION_TYPE(node) node->value.operation_type_value.operation_type
#define GET_OPERATION_ARGUMENTS_COUNT(node) node->value.operation_type_value.arguments_count
#define GET_OPERATION_VALUE(node) node->value.operation_type_value
#define GET_OPERATION_TEX_NAME(node) node->value.operation_type_value.latex_operation_name
#define GET_OPERATION_NAME(node) node->value.operation_type_value.operation_name
#define GET_OPERATION_ASM_NAME(node) node->value.operation_type_value.operation_asm_name

#define GET_VARIABLE_NAME(node) variables_array[node->value.variable_index_in_array].variable_name
#define GET_VARIABLE_VALUE(node) variables_array[node->value.variable_index_in_array].variable_value
#define GET_VARIABLE_HASH(node) variables_array[node->value.variable_index_in_array].variable_hash
#define GET_VARIABLE_ARRAY_POS(node) node->value.variable_index_in_array

#define GET_CONTROL_STRUCTURE_TYPE(node) node->value.control_structure_type_value.control_structure_type

#define GET_SERVICE_STRUCTURE_TYPE(node) node->value.service_type_value.service_structure_type

#define GET_COMPARISON_OPERATOR_TYPE(node) node->value.comparison_operator_type_value.comparison_operator_type

#define CHECK_IF_PROGRAM_BEGIN(node) (GET_SERVICE_STRUCTURE_TYPE(node) == ProgramBeginStructure && node->type == ServiceType)
#define CHECK_IF_PROGRAM_END(node) (GET_SERVICE_STRUCTURE_TYPE(node) == ProgramEndStructure && node->type == ServiceType)
#define CHECK_IF_NEW_FUNCTION(node) (GET_SERVICE_STRUCTURE_TYPE(node) == FunctionStructure && node->type == ServiceType)
#define CHECK_IF_OPEN_BRACKET(node) (GET_SERVICE_STRUCTURE_TYPE(node) == OpenBracketStructure && node->type == ServiceType)
#define CHECK_IF_CLOSE_BRACKET(node) (GET_SERVICE_STRUCTURE_TYPE(node) == CloseBracketStructure && node->type == ServiceType)

const int MAX_WORD_FORMAT_SIZE = 100;

struct Variable_t {
    const char  *variable_name;
    unsigned int variable_num;
    int          variable_hash;
    double       variable_value;
};

enum NodeTypes {
    OperationType,
    VariableType,
    NumberType,
    ServiceType, //ÚËÔÓ ÚÓ˜ÍË Ò Á‡Ôˇ¸˚ÏË, Ó·ÓÁÌ‡˜ÂÌËˇ Ì‡˜‡Î‡-ÍÓÌˆ‡ ÔÓ„‡ÏÏ˚ Ë Ú‰
    ComparisonOperatorType,
    ControlStructureType,
    LogicalOperatorType,
    EmptyOperatorType,
    FunctionCallType,
    FunctionType,
    NewFuncType
};

struct DiffType_t {
    NodeTypes type_num;
    const char *type_name;
};

 const DiffType_t NodeTypes_ARRAY[] = { 
    {OperationType, "OP"},
    {VariableType,  "VAR"},
    {NumberType,    "NUM"}
};

enum ServiceConstructions {
    BeginConstruction,
    EndConstructions,

};

enum LogicalOperators {
    AndLogicalOperator,
    OrLogicalOperator,
    NotLogicalOperator
};

enum Operations {
    AddictionOperation,
    SubstractionOperation,
    MultiplicationOperation,
    DivisionOperation,
    SinOperation,
    CosOperation,
    PowOperation,
    SinHyperbolicOperation,
    CosHyperbolicOperation,
    TanOperation,
    TanHyperbolicOperation,
    ArcSinOperation,
    ArcCosOperation,
    ArcTanOperation
};

enum OperationArgumentsCount {
    Binary,
    Unary
};

enum ComparisonOperators {
    EqualityComparison,
    LessComparison,
    LessOrEqualComparison,
    GreaterComparison,
    GreaterOrEqualityComparison,
    DenialComparison,
};

enum ControlStructures {
    BeginWhileCycle,
    EndWhileCycle,
    IfOperator,
    IfElseOperator,
    EndIfOperator,
    EqualizationOperator,
    DeclarationOperator,
    InVariableOperator,
    ReturnOperator,
    PrintOperator
};

enum ServiceStructures {
    ProgramBeginStructure,
    ProgramEndStructure,
    FunctionStructure,
    OpenBracketStructure,
    CloseBracketStructure,
    BodyBeginStructure,
    ComaStructure
};

struct LogicalOperator_t {
    LogicalOperators logical_operator_type;
    const char      *logical_operator_name;
    int              logical_operator_hash;
};

struct ServiceStructure_t {
    ServiceStructures service_structure_type;
    const char       *service_structure_name;
    int               service_structure_hash;
};

struct Operation_t {
    Operations  operation_type;
    const char *operation_name;
    int         operation_hash;
    double    (*operation_func_ptr) (double first_num, double second_num);
    OperationArgumentsCount arguments_count;
    const char *operation_asm_name;
};

struct ComparisonOperator_t {
    ComparisonOperators comparison_operator_type;
    const char         *comparison_operator_name;
    int                 comparison_operator_hash;
};

struct ControlStructure_t {
    ControlStructures control_structure_type;
    const char       *control_structure_name;
    int               control_structure_hash;
};

struct Function_t {
    const char *function_name;
    int params_count;
    int function_num;
    int variables_count;
    Variable_t *variables_table;
};

union value_t {
    Operation_t          operation_type_value;
    double               num_type_value;
    int                  variable_index_in_array;
    int                  function_index_in_array;
    ComparisonOperator_t comparison_operator_type_value;
    ControlStructure_t   control_structure_type_value;
    ServiceStructure_t   service_type_value;
    LogicalOperator_t    logical_operator_type_value;
};

struct Node_t {
    NodeTypes   type;
    Node_t     *left;
    Node_t     *right;
    value_t     value;
    Node_t     *parent;
};

struct Tree_t {
    Node_t *root;
    int nodes_count;
};

struct FunctionTable_t {
    int params_count;
    int variables_count;
    Variable_t *variables_table;
};

double RunAddictionOperation(double first_num, double second_num);
double RunMultiplicationOperation(double first_num, double second_num);
double RunDIvisionOperation(double first_num, double second_num);
double RunSubstractionOperation(double first_num, double second_num);
double RunSinOperation(double first_num, double second_num);
double RunCosOperation(double first_num, double second_num);
double RunPowOperation(double first_num, double second_num);
double RunSinHyperbolicOperation(double first_num, double second_num);
double RunCosHyperbolicOperation(double first_num, double second_num);
double RunTanOperation(double first_num, double second_num);
double RunTanHyperbolicOperation(double first_num, double second_num);
double RunArcSinOperation(double first_num, double second_num);
double RunArcCosOperation(double first_num, double second_num);
double RunArcTanOperation(double first_num, double second_num);

int CountStringHashDJB2(const char *curr_string);

bool CheckIfArraysCorrect();

ServiceStructure_t const SERVICE_STRUCTURES_ARRAY[] = {
    {ProgramBeginStructure,         "Õ¿◊¿ÀŒ",       CountStringHashDJB2("Õ¿◊¿ÀŒ")       }, 
    {ProgramEndStructure,           " ŒÕ≈÷",        CountStringHashDJB2(" ŒÕ≈÷")        },
    {FunctionStructure,             "‘”Õ ÷»ﬂ",      CountStringHashDJB2("‘”Õ ÷»ﬂ")      },
    {OpenBracketStructure,          "(",            CountStringHashDJB2("(")            },
    {CloseBracketStructure,         ")",            CountStringHashDJB2(")")            },
    {BodyBeginStructure,            "¿¡Œ¡¿",        CountStringHashDJB2("¿¡Œ¡¿")        },
    {ComaStructure,                 ",",            CountStringHashDJB2(",")            } };

ComparisonOperator_t const COMPARISON_OPERATORS_ARRAY[] = {
    {EqualityComparison,            "–¿¬ÕŒ",        CountStringHashDJB2("–¿¬ÕŒ")        },
    {LessComparison,                "Ã≈Õ‹ÿ≈",       CountStringHashDJB2("Ã≈Õ‹ÿ≈")       },
    {LessOrEqualComparison,         "Ã≈Õ‹ÿ≈–¿¬ÕŒ",  CountStringHashDJB2("Ã≈Õ‹ÿ≈–¿¬ÕŒ")  },
    {GreaterComparison,             "¡ŒÀ‹ÿ≈",       CountStringHashDJB2("¡ŒÀ‹ÿ≈")       },
    {GreaterOrEqualityComparison,   "¡ŒÀ‹ÿ≈–¿¬ÕŒ",  CountStringHashDJB2("¡ŒÀ‹ÿ≈–¿¬ÕŒ")  },
    {DenialComparison,              "Õ≈–¿¬ÕŒ",      CountStringHashDJB2("Õ≈–¿¬ÕŒ")      } };

LogicalOperator_t const LOGICAL_OPERATORS_ARRAY[] = {
    {AndLogicalOperator,            "»",            CountStringHashDJB2("»")            },
    {OrLogicalOperator,             "»À»",          CountStringHashDJB2("»À»")          },
    {NotLogicalOperator,            "Õ≈",           CountStringHashDJB2("Õ≈")           } };

ControlStructure_t const CONTROL_STRUCTURES_ARRAY[] = {
    {BeginWhileCycle,               "œŒ ¿",         CountStringHashDJB2("œŒ ¿")         },
    {EndWhileCycle,                 " ŒÕ≈÷œŒ ¿",    CountStringHashDJB2(" ŒÕ≈÷œŒ ¿")    },
    {IfOperator,                    "≈—À»",         CountStringHashDJB2("≈—À»")         },
    {IfElseOperator,                "»Õ¿◊≈",        CountStringHashDJB2("»Õ¿◊≈")        },
    {EndIfOperator,                 " ŒÕ≈÷≈—À»",    CountStringHashDJB2(" ŒÕ≈÷≈—À»")    }, 
    {EqualizationOperator,          "«¿Ã≈Õ»“‹",     CountStringHashDJB2("«¿Ã≈Õ»“‹")     },
    {DeclarationOperator,           "Œ¡⁄ﬂ¬»“‹",     CountStringHashDJB2("Œ¡⁄ﬂ¬»“‹")     },
    {InVariableOperator,            "œŒÀ”◊»“‹",     CountStringHashDJB2("œŒÀ”◊»“‹")     },
    {ReturnOperator,                "¬≈–Õ”“‹",      CountStringHashDJB2("¬≈–Õ”“‹")      },
    {PrintOperator,                 "¬€¬≈—“»",      CountStringHashDJB2("¬€¬≈—“»")      } };

Operation_t const OPERATIONS_ARRAY[] = {
    { AddictionOperation,     "œÀﬁ—",      CountStringHashDJB2("œÀﬁ—"),     RunAddictionOperation      , Binary,     "ADD"},
    { SubstractionOperation,  "Ã»Õ”—",     CountStringHashDJB2("Ã»Õ”—"),    RunSubstractionOperation   , Binary,     "SUB"},
    { MultiplicationOperation,"”ÃÕŒ∆»“‹",  CountStringHashDJB2("”ÃÕŒ∆»“‹"), RunMultiplicationOperation , Binary,     "MUL"},
    { DivisionOperation,      "–¿«ƒ≈À»“‹", CountStringHashDJB2("–¿«ƒ≈À»“‹"),RunDIvisionOperation       , Binary,     "DIV"}, 
    { SinOperation,           "—»Õ”—",     CountStringHashDJB2("—»Õ”—"),    RunSinOperation            , Unary ,     },
    { CosOperation,           " Œ—»Õ”—",   CountStringHashDJB2(" Œ—»Õ”—"),  RunCosOperation            , Unary ,     },
    { PowOperation,           "—“≈œ≈Õ‹",   CountStringHashDJB2("—“≈œ≈Õ‹"),  RunPowOperation            , Binary,     "POW"},
    { SinHyperbolicOperation, "ÿ»Õ”—",     CountStringHashDJB2("ÿ»Õ”—"),    RunSinHyperbolicOperation  , Unary ,     },
    { CosHyperbolicOperation, "◊Œ—»Õ”—",   CountStringHashDJB2("ch"),       RunCosHyperbolicOperation  , Unary ,     }, 
    { TanOperation,           "“¿Õ√≈Õ—",   CountStringHashDJB2("“¿Õ√≈Õ—"),  RunTanOperation            , Unary ,     },
    { TanHyperbolicOperation, "th",        CountStringHashDJB2("th"),       RunTanHyperbolicOperation  , Unary ,     },
    { ArcSinOperation,        "¿– —»Õ”—",  CountStringHashDJB2("¿– —»Õ”—"), RunArcSinOperation         , Unary ,     },
    { ArcCosOperation,        "¿–  Œ—»Õ”—",CountStringHashDJB2("¿–  Œ—»Õ”—"),RunArcCosOperation        , Unary ,     }, 
    { ArcTanOperation,        "¿– “¿Õ√≈Õ—",CountStringHashDJB2("¿– “¿Õ√≈Õ—"),RunArcTanOperation        , Unary ,     } };

const int OPERATIONS_COUNT           = sizeof(OPERATIONS_ARRAY)             / sizeof(Operation_t);
const int CONTROL_STRUCTURES_COUNT   = sizeof(CONTROL_STRUCTURES_ARRAY)     / sizeof(ControlStructure_t);
const int COMPARISON_OPERATORS_COUNT = sizeof(COMPARISON_OPERATORS_ARRAY)   / sizeof(ComparisonOperator_t);
const int SERVICE_STRUCTURES_COUNT   = sizeof(SERVICE_STRUCTURES_ARRAY)     / sizeof(ServiceStructure_t);
const int LOGICAL_OPERATORS_COUNT    = sizeof(LOGICAL_OPERATORS_ARRAY)      / sizeof(LogicalOperator_t);

const bool OperationsArrayCheckingResult = CheckIfArraysCorrect();

extern Variable_t *variables_array;
extern unsigned int variables_count;

extern Function_t **functions_array;
extern unsigned int curr_function_num;

Node_t *CreateNewNode(NodeTypes type, Node_t *left, Node_t *right, value_t value);
Node_t *CopyNode(Node_t *node);

Node_t *CreateNewNumNode(Node_t *left, Node_t *right, double value);
Node_t *CreateNewOperationNode(Node_t *left, Node_t *right, Operation_t value);
Node_t *CreateNewVariableNode(Node_t *left, Node_t *right, int value);
Node_t *CreateNewComparisonOperatorNode(Node_t *left, Node_t *right, ComparisonOperator_t value);
Node_t *CreateNewServiceNode(Node_t *left, Node_t *right, ServiceStructure_t value);
Node_t *CreateNewControlNode(Node_t *left, Node_t *right, ControlStructure_t value);
Node_t *CreateNewLogicalOperatorNode(Node_t *left, Node_t *right, LogicalOperator_t value);
Node_t *CreateEmptyNode();
Node_t *CreateNewFuncNode();

Node_t *SimplifyDiffTree(Node_t *node);

Node_t *CountConstants(Node_t *curr_node, bool *IsTreeChanged);

Node_t *DeleteUselessElements(Node_t *curr_node, bool *IsTreeChanged);
Node_t *TryToOptimizeNode(Node_t *curr_node, bool *IsTreeChanged);

Node_t *DeleteOneNeutralElement(Node_t *parent_node, Node_t *first_Neutral_elem_node, Node_t *second_node, bool *IsTreeChanged);
Node_t *DeleteBothNeutralElementsAndChangeValueToZero(Node_t *curr_node, bool *IsTreeChanged);

void DeleteNode(Node_t *node);

double CountNodeValue(Node_t* node);

void FillVariableValue(const char *variable_name, double variable_value);

Node_t *SetNodeParent(Node_t *node, Node_t *parent);

#endif // DIFF_STRUCT_H_INCLUDED
