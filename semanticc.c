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
void report_error(std::string error_message) {
  // Output the error message to the user
  std::cout << "Semantic error: " << error_message << std::endl;

  // Increment the error count
  error_count++;
}
bool is_assignment_type_valid(ASTNode *node) {
  // Get the declared type of the variable being assigned to
  DataType declared_type = get_variable_declared_type(node->identifier);

  // Check that the type of the assignment expression matches the declared type
  return node->assignment_expression_type == declared_type;
}
void  get_variable_declared_type(std::string identifier) {
  // Look up the variable's declared type in the symbol table
  return symbol_table[identifier].declared_type;
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
bool is_variable_declared(std::string identifier) {
  // Check if the variable is in the symbol table
  return symbol_table.count(identifier) > 0;
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
bool are_parameter_names_unique(std::vector<ASTNode*> parameters) {
  // Create a set to store the parameter names
  std::unordered_set<std::string> parameter_names;

  // Iterate through the parameters and check that their names are unique
  for (ASTNode *parameter : parameters) {
    // Check if the parameter name is already in the set
    if (parameter_names.count(parameter->identifier) > 0) {
      // Return false if the parameter name is not unique
      return false;
    }
    // Add the parameter name to the set
    parameter_names.insert(parameter->identifier);
  }

  // Return true if all parameter names are unique
  return true;
}
void check_function_body(ASTNode *function_body_node) {
  // Check that the function body is a block node
  if (function_body_node->node_type != BLOCK) {
    report_error("Function body must be a block of statements");
  }

  // Check that all statements in the function body are well-formed
  for (ASTNode *statement : function_body_node->statements) {
    check_statement(statement);
  }
}

void check_statement(ASTNode *statement) {
  // Switch on the statement type and check for well-formedness as appropriate
  switch (statement->node_type) {
    case VARIABLE_DECLARATION:
      check_variable_declaration(statement);
      break;
    case ASSIGNMENT:
      check_assignment(statement);
      break;
    case FUNCTION_CALL:
      check_function_call(statement);
      break;
    case CONTROL_FLOW:
      check_control_flow(statement);
      break;
    case RETURN:
      check_return(statement);
      break;
    // Add additional cases as needed
  }
}

bool is_function_declared(std::string identifier) {
  // Check if the function is in the function table
  return function_table.count(identifier) > 0;
}

void check_function_call(ASTNode *node)
{
    // Check that the function has been declared
    if (!is_function_declared(node->identifier))
    {
        report_error("Function '" + node->identifier + "' has not been declared");
    }

    

    // Check that the argument types are compatible with the function's parameter types
    if (!are_argument_types_compatible(node))
    {
        report_error("Incompatible argument types passed to function '" + node->identifier + "'");
    }
}
bool are_argument_types_compatible(ASTNode *function_call_node) {
  // Get the function declaration node
  ASTNode *function_declaration_node = get_function_declaration_node(function_call_node->identifier);

  // Check that the number of arguments matches the number of parameters
  if (function_call_node->arguments.size() != function_declaration_node->parameters.size()) {
    return false;
  }

  // Iterate through the arguments and check that their types are compatible with the corresponding parameter types
  for (int i = 0; i < function_call_node->arguments.size(); i++) {
    // Check that the argument type is compatible with the parameter type
    if (!is_type_compatible(function_call_node->arguments[i]->expression_type, function_declaration_node->parameters[i]->declared_type)) {
      return false;
    }
  }

  // Return true if all argument types are compatible with the corresponding parameter types
  return true;
}
bool is_type_compatible(DataType type1, DataType type2) {
  // Check if the types are equal
  if (type1 == type2) {
    return true;
  }

  // Check if one of the types is a numeric type and the other is a numeric type
  if (is_numeric_type(type1) && is_numeric_type(type2)) {
    return true;
  }

  // Check if one of the types is a string type and the other is a string type
  if (type1 == STRING && type2 == STRING) {
    return true;
  }

  // Return false if the types are not compatible
  return false;
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
bool is_boolean_expression(ASTNode *expression_node) {
  // Check if the expression type is a boolean type
  return expression_node->expression_type == BOOLEAN;
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

.
    \\\edited
