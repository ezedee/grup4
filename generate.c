

void generate_code(struct AstNode* node) {
  if (node->type == TOKEN_INT) {
    printf("mov eax, %d\n", node->int_value);
  } else if (node->type == TOKEN_PLUS) {
    generate_code(node->left);
    printf("push eax\n");
    generate_code(node->right);
    printf("pop ebx\n");
    printf("add eax, ebx\n");
  } else if (node->type == TOKEN_MINUS) {
    generate_code(node->left);
    printf("push eax\n");
    generate_code(node->right);
    printf("pop ebx\n");
    printf("sub eax, ebx\n");
  } else if (node->type == TOKEN_MULTIPLY) {
    generate_code(node->left);
    printf("push eax\n");
    generate_code(node->right);
    printf("pop ebx\n");
    printf("imul eax, ebx\n");
  } else if (node->type == TOKEN_DIVIDE) {
    generate_code(node->left);
    printf("push eax\n");
    generate_code(node->right);
    printf("pop ebx\n");
    printf("mov edx, 0\n");
    printf("idiv ebx\n");
  }
}

int main(int argc, char** argv) {
  struct Lexer lexer;
  init_lexer(&lexer, "3 + 4 * 5");
  next_token(&lexer);
  struct AstNode* tree = parse_expression(&lexer);
  semantic_analysis(ASTNode *root)
  generate_code(tree);

  return
