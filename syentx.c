

struct AstNode* parse_primary(struct Lexer* lexer) {
  struct AstNode* node = malloc(sizeof(struct AstNode));
  node->type = lexer->current_token.type;
  node->int_value = lexer->current_token.int_value;
  next_token(lexer);
  return node;
}

struct AstNode* parse_expression(struct Lexer* lexer);

struct AstNode* parse_term(struct Lexer* lexer) {
  struct AstNode* node = parse_primary(lexer);

  while (lexer->current_token.type == TOKEN_MULTIPLY || lexer->current_token.type == TOKEN_DIVIDE) {
    struct AstNode* op = malloc(sizeof(struct AstNode));
    op->type = lexer->current_token.type;
    op->left = node;
    next_token(lexer);
    op->right = parse_primary(lexer);
    node = op;
  }

  return node;
}

struct AstNode* parse_expression(struct Lexer* lexer) {
  struct AstNode* node = parse_term(lexer);

  while (lexer->current_token.type == TOKEN_PLUS || lexer->current_token.type == TOKEN_MINUS) {
    struct AstNode* op = malloc(sizeof(struct AstNode));
    op->type = lexer->current_token.type;
    op->left = node;
    next_token(lexer);
    op->right = parse_term(lexer);
    node = op;
  }

  return node;
}

int main(int argc, char** argv) {
  struct Lexer lexer;
  init_lexer(&lexer, "3 + 4 * 5");
  next_token(&lexer);
  struct AstNode* tree = parse_expression(&lexer);
  // tree now contains the parse tree for the expression "3 + 4 * 5"
}