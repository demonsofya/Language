#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "../../../diff_struct.h"
#include "../math/mathematics.h"

//#include "TXlib.h"

bool CheckIfArraysCorrect() {

    for (int i = 0; i < OPERATIONS_COUNT; i++)
        assert(OPERATIONS_ARRAY[i].operation_type == i);

    for (int i = 0; i < COMPARISON_OPERATORS_COUNT; i++)
        assert(COMPARISON_OPERATORS_ARRAY[i].comparison_operator_type == i);

    for (int i = 0; i < SERVICE_STRUCTURES_COUNT; i++)
        assert(SERVICE_STRUCTURES_ARRAY[i].service_structure_type == i);

    for (int i = 0; i < CONTROL_STRUCTURES_COUNT; i++)
        assert(CONTROL_STRUCTURES_ARRAY[i].control_structure_type == i);

    for (int i = 0; i < LOGICAL_OPERATORS_COUNT; i++)
        assert(LOGICAL_OPERATORS_ARRAY[i].logical_operator_type == i);

    return true;
}

Node_t *CreateNewNode(NodeTypes type, Node_t *left, Node_t *right, value_t value) {
    
    Node_t *new_node = (Node_t *) calloc(1, sizeof(Node_t));
    *new_node = {type, left, right, value};

    return new_node;
}

Node_t *CreateNewNumNode(Node_t *left, Node_t *right, double value) {
    
    value_t new_value = {};
    new_value.num_type_value = value;
    return CreateNewNode(NumberType, left, right, new_value);
}

Node_t *CreateNewOperationNode(Node_t *left, Node_t *right, Operation_t value) {
    

    value_t new_value = {};
    new_value.operation_type_value = value;
    return CreateNewNode(OperationType, left, right, new_value);
}

Node_t *CreateNewComparisonOperatorNode(Node_t *left, Node_t *right, ComparisonOperator_t value) {

    value_t new_value = {};
    new_value.comparison_operator_type_value = value;
    return CreateNewNode(ComparisonOperatorType, left, right, new_value);
}

Node_t *CreateNewVariableNode(Node_t *left, Node_t *right, int value) {
    
    value_t new_value = {};
    new_value.variable_index_in_array = value;
    return CreateNewNode(VariableType, left, right, new_value);
}

Node_t *CreateNewServiceNode(Node_t *left, Node_t *right, ServiceStructure_t value) {
    
    value_t new_value = {};
    new_value.service_type_value = value;
    return CreateNewNode(ServiceType, left, right, new_value);
}

Node_t *CreateEmptyNode() {

    return CreateNewNode(EmptyOperatorType, NULL, NULL, {});
}

Node_t *CreateNewFuncNode() {

    return CreateNewNode(NewFuncType, NULL, NULL, {});
}

Node_t *CreateNewControlNode(Node_t *left, Node_t *right, ControlStructure_t value) {
    
    value_t new_value = {};
    new_value.control_structure_type_value = value;
    return CreateNewNode(ControlStructureType, left, right, new_value);
}

Node_t *CreateNewLogicalOperatorNode(Node_t *left, Node_t *right, LogicalOperator_t value) {
     
    value_t new_value = {};
    new_value.logical_operator_type_value = value;
    return CreateNewNode(LogicalOperatorType, left, right, new_value);
}

Node_t *CopyNode(Node_t *node) {
    
    return CreateNewNode(node->type, node->left, node->right, node->value);
}

Node_t *SimplifyDiffTree(Node_t *node) {

    assert(node);

    bool IsTreeChanged = true;

    while (IsTreeChanged) {

        IsTreeChanged = false;
        CountConstants(node, &IsTreeChanged);

        //TreeDump(tree, __FILE__, __FUNCTION__, __LINE__,
        //            "DUMP AFTER COUNTING CONSTANTS TREE", NoDiffError);

        node = DeleteUselessElements(node, &IsTreeChanged);
    }

    return node;
}

Node_t *CountConstants(Node_t *curr_node, bool *IsTreeChanged) {

    assert(curr_node);

    Node_t *new_left_node = NULL;
    Node_t *new_right_node = NULL;

    if (curr_node->type == NumberType)
        return curr_node;

    if (curr_node->type == VariableType)
        return NULL;

    if (curr_node->left != NULL)
        new_left_node = CountConstants(curr_node->left, IsTreeChanged);

    if (curr_node->right != NULL)
        new_right_node = CountConstants(curr_node->right, IsTreeChanged);

    OperationArgumentsCount arguments_count_type = GET_OPERATION_ARGUMENTS_COUNT(curr_node);

    if ((arguments_count_type == Binary && (new_left_node == NULL || new_right_node == NULL)) ||
        (arguments_count_type == Unary  && new_left_node == NULL && new_right_node == NULL))
        return NULL;

    curr_node->type = NumberType;
    GET_NODE_NUM_VALUE(curr_node) = GET_OPERATION_FUNC_PTR(curr_node)(GET_NODE_NUM_VALUE(new_left_node), GET_NODE_NUM_VALUE(new_right_node));

    curr_node->left = NULL;
    curr_node->right = NULL;

    free(new_left_node);
    free(new_right_node);

    *IsTreeChanged = true;

    return curr_node;
}

Node_t *DeleteUselessElements(Node_t *curr_node, bool *IsTreeChanged) {

    assert(curr_node);

    if (curr_node->type != OperationType)
        return curr_node;

    if (GET_OPERATION_ARGUMENTS_COUNT(curr_node) == Unary)
        return curr_node;

    assert(curr_node->left);
    assert(curr_node->right);

    Operations operation_type = GET_OPERATION_TYPE(curr_node);

    curr_node->left = DeleteUselessElements(curr_node->left, IsTreeChanged);
    curr_node->right = DeleteUselessElements(curr_node->right, IsTreeChanged);

    return TryToOptimizeNode(curr_node, IsTreeChanged);
}

Node_t *TryToOptimizeNode(Node_t *curr_node, bool *IsTreeChanged) {

    assert(curr_node);
    assert(IsTreeChanged);

    Operations operation_type = curr_node->value.operation_type_value.operation_type;

    if (curr_node->left->type == NumberType) {
        if (CompareDoubles(GET_NODE_NUM_VALUE(curr_node->left), 0)) {
            if (operation_type == AddictionOperation || operation_type == SubstractionOperation)
                return DeleteOneNeutralElement(curr_node, curr_node->left, curr_node->right, IsTreeChanged);

            if (operation_type == MultiplicationOperation)
                return DeleteBothNeutralElementsAndChangeValueToZero(curr_node, IsTreeChanged);

        } else if (CompareDoubles(GET_NODE_NUM_VALUE(curr_node->left), 1))
            if (operation_type == MultiplicationOperation || operation_type == DivisionOperation)
                return DeleteOneNeutralElement(curr_node, curr_node->left, curr_node->right, IsTreeChanged);

    }

    if (curr_node->right->type == NumberType) {
        if (CompareDoubles(GET_NODE_NUM_VALUE(curr_node->right), 0)) {
            if (operation_type == AddictionOperation || operation_type == SubstractionOperation)
                return DeleteOneNeutralElement(curr_node, curr_node->right, curr_node->left, IsTreeChanged);

            if (operation_type == MultiplicationOperation)
                return DeleteBothNeutralElementsAndChangeValueToZero(curr_node, IsTreeChanged);

        } else if (CompareDoubles(GET_NODE_NUM_VALUE(curr_node->right), 1))
            if (operation_type == MultiplicationOperation || operation_type == DivisionOperation || operation_type == PowOperation)
                return DeleteOneNeutralElement(curr_node, curr_node->right, curr_node->left, IsTreeChanged);
    }

    return curr_node;
}

Node_t *DeleteOneNeutralElement(Node_t *parent_node, Node_t *first_Neutral_elem_node, Node_t *second_node, bool *IsTreeChanged) {

    assert(parent_node);
    assert(first_Neutral_elem_node);
    assert(second_node);
    assert(IsTreeChanged);

    DeleteNode(first_Neutral_elem_node);

    parent_node = CopyNode(second_node);
    free(second_node);

    *IsTreeChanged = true;
    return parent_node;
}

Node_t *DeleteBothNeutralElementsAndChangeValueToZero(Node_t *curr_node, bool *IsTreeChanged) {

    assert(curr_node);
    assert(IsTreeChanged);

    DeleteNode(curr_node->right);
    DeleteNode(curr_node->left);

    curr_node->type = NumberType;
    GET_NODE_NUM_VALUE(curr_node) = 0;
    curr_node->left = NULL;
    curr_node->right = NULL;

    *IsTreeChanged = true;
    return curr_node;
}

double RunAddictionOperation(double first_num, double second_num) {

    return first_num + second_num;
}

double RunMultiplicationOperation(double first_num, double second_num) {

    return first_num * second_num;
}

double RunDIvisionOperation(double first_num, double second_num) {

    if (second_num == NAN)
        return 0;

    return first_num / second_num;
}

double RunSubstractionOperation(double first_num, double second_num) {
    return first_num - second_num;
}

double RunPowOperation(double first_num, double second_num) {
    return pow(first_num, second_num);
}

double RunSqrtOperation(double first_num, double second_num) {
    return sqrt(first_num);
}

void DeleteNode(Node_t *node) {

    assert(node);

    if (node->left != NULL)
        DeleteNode(node->left);

    if (node->right != NULL)
        DeleteNode(node->right);

    free(node);
}

//=============================================================

double CountNodeValue(Node_t* node) {

    if (node == NULL)
        return 0;

    switch(node->type) {
        case NumberType:
            return GET_NODE_NUM_VALUE(node);

        case VariableType:
            return GET_VARIABLE_VALUE(node);

        case OperationType:
            return GET_OPERATION_FUNC_PTR(node)(CountNodeValue(node->left), CountNodeValue(node->right));

        default:
            return 0;
    }
}

void FillVariableValue(const char *variable_name, double variable_value) {

    assert(variable_name);

    int variable_hash = CountStringHashDJB2(variable_name);

    for (int i = 0; i < variables_count; i++)
        if (variable_hash == variables_array[i].variable_hash)
            if (strcmp(variable_name, variables_array[i].variable_name) == 0) {

                variables_array[i].variable_value = variable_value;
                return;
            }

    printf("Variable didn't find\n\n");
    return;
}


Node_t *SetNodeParent(Node_t *node, Node_t *parent) {

    if (node == NULL)
        return NULL;

    node->parent = parent;

    SetNodeParent(node->left, node);
    SetNodeParent(node->right, node);

    return node;
}


//-----------------------------------------------------------------------------

int CountStringHashDJB2(const char *curr_string) {

    assert(curr_string);

    int curr_hash = 5381, curr_num = 0;

    while (curr_string[curr_num]) {
        curr_hash = ((curr_hash << 5) + curr_hash) + curr_string[curr_num];

        curr_num++;
    }

    return curr_hash;
}
