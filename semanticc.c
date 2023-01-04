void semantic_analysis(ASTNode *root)
{
    // traverse the abstract syntax tree recursively
    if (root == NULL)
        return;

    // check for semantic errors at this node
    check_semantic_errors(root);

    // check for semantic errors in the children of this node
    for (int i = 0; i < root->num_children; i++)
    {
        semantic_analysis(root->children[i]);
    }
}
void check_variable_declaration(ASTNode *node)
{
    // Check that the variable is not already declared in the current scope
    if (is_variable_declared(node->identifier))
    {
        report_error("Variable '" + node->identifier + "' already declared in this scope");
    }

    // Check that the variable is being assigned a value of the correct type
    if (!is_assignment_type_valid(node))
    {
        report_error("Invalid type for assignment to variable '" + node->identifier + "'");
    }
}

void check_assignment(ASTNode *node)
{
    // Check that the variable being assigned to has been declared
    if (!is_variable_declared(node->identifier))
    {
        report_error("Variable '" + node->identifier + "' has not been declared");
    }

    // Check that the value being assigned has a compatible type with the variable
    if (!is_assignment_type_compatible(node))
    {
        report_error("Incompatible type for assignment to variable '" + node->identifier + "'");
    }
}

void check_function_declaration(ASTNode *node)
{
    // Check that the function name is not already in use
    if (is_function_declared(node->identifier))
    {
        report_error("Function '" + node->identifier + "' already declared");
    }

    // Check that the function has a return type
    if (node->return_type == UNDEFINED)
    {
        report_error("Function '" + node->identifier + "' must have a return type");
    }

    // Check that all function parameters have unique names
    if (!are_parameter_names_unique(node->parameters))
    {
        report_error("Function '" + node->identifier + "' has duplicate parameter names");
    }

    // Check that the function body is well-formed
    check_function_body(node->function_body);
}

void check_function_call(ASTNode *node)
{
    // Check that the function has been declared
    if (!is_function_declared(node->identifier))
    {
        report_error("Function '" + node->identifier + "' has not been declared");
    }

    // Check that the correct number of arguments are being passed to the function
    if (!are_argument_counts_matching(node))
    {
        report_error("Incorrect number of arguments passed to function '" + node->identifier + "'");
    }

    // Check that the argument types are compatible with the function's parameter types
    if (!are_argument_types_compatible(node))
    {
        report_error("Incompatible argument types passed to function '" + node->identifier + "'");
    }
}
void check_control_flow(ASTNode *node)
{
    // Check that the condition for the control flow statement is a boolean expression
    if (!is_boolean_expression(node->condition))
    {
        report_error("Control flow condition must be a boolean expression");
    }

    // Check that the body of the control flow statement is well-formed
    check_control_flow_body(node->control_flow_body);
}

void check_return(ASTNode *node)
{
    // Check that the return type matches the function's declared return type
    if (!is_return_type_matching(node))
    {
        report_error("Incompatible return type");
    }
}

void check_array_declaration(ASTNode *node)
{
    // Check that the array is not already declared in the current scope
    if (is_variable_declared(node->identifier))
    {
        report_error("Array '" + node->identifier + "' already declared in this scope");
    }

    // Check that the array size is a positive integer expression
    if (!is_positive_integer_expression(node->array_size))
    {
        report_error("Array size must be a positive integer expression");
    }

    // Check that the array is being assigned a value of the correct type
    if (!is_assignment_type_valid(node))
    {
        report_error("Invalid type for assignment to array '" + node->identifier + "'");
    }
}
void check_array_access(ASTNode *node)
{
    // Check that the array has been declared
    if (!is_variable_declared(node->identifier))
    {
        report_error("Array '" + node->identifier + "' has not been declared");
    }

    // Check that the array access expression is a positive integer expression
    if (!is_positive_integer_expression(node->array_access_expression))
    {
        report_error("Array access expression must be a positive integer expression");
    }
}
void check_struct_declaration(ASTNode *node)
{
    // Check that the struct is not already declared in the current scope
    if (is_struct_declared(node->identifier))
    {
        report_error("Struct '" + node->identifier + "' already declared in this scope");
    }

    // Check that all struct field names are unique
    if (!are_field_names_unique(node->fields))
    {
        report_error("Struct '" + node->identifier + "' has duplicate field names");
    }
}
void check_semantic_errors(ASTNode *node)
{
    switch (node->node_type)
    {
    case VARIABLE_DECLARATION:
        check_variable_declaration(node);
        break;
    case ASSIGNMENT:
        check_assignment(node);
        break;
    case FUNCTION_DECLARATION:
        check_function_declaration(node);
        break;
    case FUNCTION_CALL:
        check_function_call(node);
        break;
    case CONTROL_FLOW:
        check_control_flow(node);
        break;
    case RETURN:
        check_return(node);
        break;
    case ARRAY_DECLARATION:
        check_array_declaration(node);
        break;
    case ARRAY_ACCESS:
        check_array_access(node);
        break;
    case STRUCT_DECLARATION:
        check_struct_declaration(node);
        break;
    default:
        // other cases
        break;
    }
}

// This code is a function that performs semantic analysis on a parse tree. The function recursively processes each node in the parse tree, performing the following steps:

// Check the type of the current parse tree node and process it accordingly. For example, if the node is a variable declaration, it checks if the variable has already been declared and adds it to the symbol table if it has not. If the node is an assignment, it checks if the variable being assigned to has been declared.

// Recursively process the children of the current node.

// Create an abstract syntax tree (AST) node for the current parse tree node. The AST node is created by calling the create_ast_node function, which returns a new AST node with the given type. The data of the AST node is then populated by copying the data from the parse tree node. The children of the AST node are set to be the corresponding AST nodes of the parse tree node's children.

// Set the AST node for the parse tree node by assigning it to the ast_node field of the parse tree node.

// The function returns nothing, as the AST is constructed in place. If an error is encountered during semantic analysis, the function prints an error message and exits the program.