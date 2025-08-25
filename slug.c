#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>

// ------- debugging ------
void panic(const char* msg) {
	fprintf(stderr, "runtime error: %s\n", msg);
	exit(EXIT_FAILURE);
}

void panicf(const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	fprintf(stderr,"runtime error: ");
	vfprintf(stderr, fmt, args);
	fprintf(stderr, "\n");
	va_end(args);
	exit(EXIT_FAILURE);
}

// ------ tokenizer -------
typedef enum {
	TOKEN_IDENTIFIER, TOKEN_NUMBER, TOKEN_BOOLEAN,
	TOKEN_LET, TOKEN_CONST, TOKEN_IF, TOKEN_ELSE, TOKEN_ELSEIF,
	TOKEN_WHILE, TOKEN_FUNCTION, TOKEN_ARROW,
	TOKEN_OUTNUMBER,
	TOKEN_SEMICOLON,
	TOKEN_LCURLY, TOKEN_RCURLY, TOKEN_LPAREN, TOKEN_RPAREN,
	TOKEN_PLUS, TOKEN_MINUS, TOKEN_MULTIPLY, TOKEN_DIVIDE, TOKEN_MODULUS,
	TOKEN_GT, TOKEN_GEQ, TOKEN_EQEQ, TOKEN_LEQ, TOKEN_LT, TOKEN_NEQ,
	TOKEN_ANDAND, TOKEN_OROR, TOKEN_COMMA,
	TOKEN_NOT, TOKEN_EQUAL,
	TOKEN_EOF
} TokenType;

typedef struct {
	TokenType type;
	union {
		int intValue;
		char* stringValue;
	};
} Token;

typedef struct {
	Token* tokens;
	size_t count;
	size_t capacity;
} TokenArray;

void tokenArrayInit(TokenArray* arr) {
	arr->tokens = NULL;
	arr->count = 0;
	arr->capacity = 0;
}

// debug print function for tokens
void print_token(Token t) {
	switch(t.type) {
	case TOKEN_LET:
		printf("TOKEN_LET\n");
		break;
	case TOKEN_CONST:
		printf("TOKEN_CONST\n");
		break;
	case TOKEN_IF:
		printf("TOKEN_IF\n");
		break;
	case TOKEN_ELSE:
		printf("TOKEN_ELSE\n");
		break;
	case TOKEN_ELSEIF:
		printf("TOKEN_ELSEIF\n");
		break;
	case TOKEN_WHILE:
		printf("TOKEN_WHILE\n");
		break;
	case TOKEN_FUNCTION:
		printf("TOKEN_FUNCTION\n");
		break;
	case TOKEN_ARROW:
		printf("TOKEN_ARROW\n");
		break;
	case TOKEN_OUTNUMBER:
		printf("TOKEN_OUTNUMBER\n");
		break;
	case TOKEN_SEMICOLON:
		printf("TOKEN_SEMICOLON\n");
		break;
	case TOKEN_LCURLY:
		printf("TOKEN_LCURLY '{'\n");
		break;
	case TOKEN_RCURLY:
		printf("TOKEN_RCURLY '}'\n");
		break;
	case TOKEN_LPAREN:
		printf("TOKEN_LPAREN '('\n");
		break;
	case TOKEN_RPAREN:
		printf("TOKEN_RPAREN ')'\n");
		break;
	case TOKEN_PLUS:
		printf("TOKEN_PLUS '+'\n");
		break;
	case TOKEN_MINUS:
		printf("TOKEN_MINUS '-'\n");
		break;
	case TOKEN_MULTIPLY:
		printf("TOKEN_MULTIPLY '*'\n");
		break;
	case TOKEN_DIVIDE:
		printf("TOKEN_DIVIDE '/'\n");
		break;
	case TOKEN_MODULUS:
		printf("TOKEN_MODULUS '%%'\n");
		break;
	case TOKEN_GT:
		printf("TOKEN_GT '>'\n");
		break;
	case TOKEN_GEQ:
		printf("TOKEN_GEQ '>='\n");
		break;
	case TOKEN_EQEQ:
		printf("TOKEN_EQEQ '=='\n");
		break;
	case TOKEN_LEQ:
		printf("TOKEN_LEQ '<='\n");
		break;
	case TOKEN_LT:
		printf("TOKEN_LT '<'\n");
		break;
	case TOKEN_NEQ:
		printf("TOKEN_NEQ '!='\n");
		break;
	case TOKEN_ANDAND:
		printf("TOKEN_ANDAND '&&'\n");
		break;
	case TOKEN_OROR:
		printf("TOKEN_OROR '||'\n");
		break;
	case TOKEN_COMMA:
		printf("TOKEN_COMMA ','\n");
		break;
	case TOKEN_NOT:
		printf("TOKEN_NOT '!'\n");
		break;
	case TOKEN_EQUAL:
		printf("TOKEN_EQUAL '='\n");
		break;
	case TOKEN_NUMBER:
		printf("TOKEN_NUMBER %d\n", t.intValue);
		break;
	case TOKEN_BOOLEAN:
		printf("TOKEN_BOOLEAN '%s'\n", t.stringValue);
		break;
	case TOKEN_IDENTIFIER:
		printf("TOKEN_IDENTIFIER '%s'\n", t.stringValue);
		break;
	case TOKEN_EOF:
		printf("TOKEN_EOF\n");
		break;
	default:
		printf("TOKEN_UNKNOWN %d\n", t.type);
		break;
	}
}

void tokenArrayPush(TokenArray* arr, Token t) {
	if(arr->count == arr->capacity) {
		arr->capacity = arr->capacity == 0 ? 8 : arr->capacity * 2;
		arr->tokens = realloc(arr->tokens, arr->capacity * sizeof(Token));
	}
	arr->tokens[arr->count++] = t;
	print_token(t);
}

void tokenArrayFree(TokenArray* arr) {
	for (size_t i = 0; i < arr->count; i++) {
		if (arr->tokens[i].type == TOKEN_IDENTIFIER || arr->tokens[i].type == TOKEN_BOOLEAN) {
			free(arr->tokens[i].stringValue);
		}
	}
	free(arr->tokens);
}

bool isDigit(char c) {
	return c >= '0' && c <= '9';
}

bool isAlpha(char c) {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

void tokenize(const char* src, TokenArray* tokens) {
	size_t pos = 0;
	size_t len = strlen(src);
	while(pos < len) {
		char c = src[pos];
		if(c == ' ' || c == '\t' || c == '\r' || c == '\n') {
			pos ++;
			continue;
		}
		if(isDigit(c)) {
			int val = 0;
			while(pos < len && isDigit(src[pos])) {
				val = val * 10 + (src[pos] - '0');
				pos ++;
			}
			tokenArrayPush(tokens, (Token) {
				TOKEN_NUMBER, .intValue = val
			});
			continue;
		}
		if(isAlpha(c)) {
			size_t start = pos ++;
			while(pos < len && (isAlpha(src[pos]) || isDigit(src[pos]))) pos ++;
			size_t size = pos - start;
			char* str = malloc(size + 1);
			memcpy(str, &src[start], size);
			str[size] = '\0';
			TokenType type = TOKEN_IDENTIFIER;
			if(strcmp(str, "let") == 0) type = TOKEN_LET;
			else if(strcmp(str, "const") == 0) type = TOKEN_CONST;
			else if(strcmp(str, "if") == 0) type = TOKEN_IF;
			else if(strcmp(str, "elif") == 0) type = TOKEN_ELSEIF;
			else if(strcmp(str, "else") == 0) type = TOKEN_ELSE;
			else if(strcmp(str, "while") == 0) type = TOKEN_WHILE;
			else if(strcmp(str, "func") == 0) type = TOKEN_FUNCTION;
			else if(strcmp(str, "true") == 0 || strcmp(str, "false") == 0) type = TOKEN_BOOLEAN;
			else if(strcmp(str, "outn") == 0) type = TOKEN_OUTNUMBER;
			if(type == TOKEN_IDENTIFIER || type == TOKEN_BOOLEAN) {
				tokenArrayPush(tokens, (Token) {
					type, .stringValue = str
				});
			} else {
				free(str);
				tokenArrayPush(tokens, (Token) {
					type
				});
			}
			continue;
		}
		switch(c) {
		case '(':
			tokenArrayPush(tokens, (Token) {
				TOKEN_LPAREN
			});
			pos++;
			break;
		case ')':
			tokenArrayPush(tokens, (Token) {
				TOKEN_RPAREN
			});
			pos++;
			break;
		case '{':
			tokenArrayPush(tokens, (Token) {
				TOKEN_LCURLY
			});
			pos++;
			break;
		case '}':
			tokenArrayPush(tokens, (Token) {
				TOKEN_RCURLY
			});
			pos++;
			break;
		case '+':
			tokenArrayPush(tokens, (Token) {
				TOKEN_PLUS
			});
			pos++;
			break;
		case '-':
			tokenArrayPush(tokens, (Token) {
				TOKEN_MINUS
			});
			pos++;
			break;
		case '*':
			tokenArrayPush(tokens, (Token) {
				TOKEN_MULTIPLY
			});
			pos++;
			break;
		case '/':
			tokenArrayPush(tokens, (Token) {
				TOKEN_DIVIDE
			});
			pos++;
			break;
		case '%':
			tokenArrayPush(tokens, (Token) {
				TOKEN_MODULUS
			});
			pos++;
			break;
		case ';':
			tokenArrayPush(tokens, (Token) {
				TOKEN_SEMICOLON
			});
			pos++;
			break;
		case ',':
			tokenArrayPush(tokens, (Token) {
				TOKEN_COMMA
			});
			pos++;
			break;
		case '>':
			if(pos+1 < len && src[pos+1] == '=') {
				tokenArrayPush(tokens, (Token) {
					TOKEN_GEQ
				});
				pos+=2;
			} else {
				tokenArrayPush(tokens, (Token) {
					TOKEN_GT
				});
				pos++;
			}
			break;
		case '<':
			if(pos+1 < len && src[pos+1] == '=') {
				tokenArrayPush(tokens, (Token) {
					TOKEN_LEQ
				});
				pos+=2;
			} else {
				tokenArrayPush(tokens, (Token) {
					TOKEN_LT
				});
				pos++;
			}
			break;
		case '=':
			if(pos+1 < len && src[pos+1] == '=') {
				tokenArrayPush(tokens, (Token) {
					TOKEN_EQEQ
				});
				pos+=2;
			} else if(pos+1 < len && src[pos+1] == '>') {
				tokenArrayPush(tokens, (Token) {
					TOKEN_ARROW
				});
				pos+=2;
			} else {
				tokenArrayPush(tokens, (Token) {
					TOKEN_EQUAL
				});
				pos++;
			}
			break;
		case '!':
			if(pos+1 < len && src[pos+1] == '=') {
				tokenArrayPush(tokens, (Token) {
					TOKEN_NEQ
				});
				pos+=2;
			} else {
				tokenArrayPush(tokens, (Token) {
					TOKEN_NOT
				});
				pos++;
			}
			break;
		case '|':
			if(pos+1 < len && src[pos+1] == '|') {
				tokenArrayPush(tokens, (Token) {
					TOKEN_OROR
				});
				pos+=2;
			} else pos++;
			break;
		case '&':
			if(pos+1 < len && src[pos+1] == '&') {
				tokenArrayPush(tokens, (Token) {
					TOKEN_ANDAND
				});
				pos+=2;
			} else pos++;
			break;
		default:
			pos++;
			break;
		}
	}
	tokenArrayPush(tokens, (Token) {
		TOKEN_EOF
	});
}

// -------- ast and types ---------
typedef struct ASTNode ASTNode;
typedef struct Environment Environment;
typedef struct Closure Closure;

typedef enum {
	VAL_NUM, VAL_BOOL, VAL_FUNC, VAL_NULL
} ValueType;

typedef struct {
	ValueType type;
	union {
		int numValue;
		bool boolValue;
		Closure* funcValue;
	};
} Value;

struct Closure {
	ASTNode* func;
	Environment* env;
};

typedef enum {
	AST_IDENTIFIER,
	AST_NUMBER_LITERAL,
	AST_BOOLEAN_LITERAL,
	AST_LET,
	AST_CONST,
	AST_ASSIGN,
	AST_BINOP,
	AST_UNOP,
	AST_BLOCK,
	AST_IFELSE,
	AST_WHILE,
	AST_FUNCTION_DEC,
	AST_FUNCTION_CALL,
	AST_BUILTIN_CALL,
	AST_SEMICOLON
} ASTType;

typedef enum {
	BINOP_ADD,
	BINOP_SUBTRACT,
	BINOP_MULTIPLY,
	BINOP_DIVIDE,
	BINOP_MODULUS,
	BINOP_LT,
	BINOP_LEQ,
	BINOP_GT,
	BINOP_GEQ,
	BINOP_EQUAL,
	BINOP_NEQ,
	BINOP_AND,
	BINOP_OR
} BinOpType;

typedef enum {
	UNOP_NEGATE,
	UNOP_NOT
} UnOpType;

typedef enum {
	BUILTIN_OUTNUMBER
} BuiltInFunction;

struct ASTNode {
	ASTType type;
	union {
		struct {
			char* name;
			bool constant;
		} identifier;
		struct {
			int value;
		} numberLiteral;
		struct {
			bool value;
		} booleanLiteral;
		struct {
			ASTNode* identifier;
			ASTNode* expr;
			bool constant;
		} letNode;
		struct {
			ASTNode* identifier;
			ASTNode* expr;
		} assignNode;
		struct {
			BinOpType op;
			ASTNode* left;
			ASTNode* right;
		} binOpNode;
		struct {
			UnOpType op;
			ASTNode* expr;
		} unOpNode;
		struct {
			ASTNode* expr;
		} blockNode;
		struct {
			ASTNode** conds;
			ASTNode** ifTrues;
			size_t condCount;
			ASTNode* elseBlock;
		} ifElseNode;
		struct {
			ASTNode* cond;
			ASTNode* body;
		} whileNode;
		struct {
			ASTNode** params;
			size_t paramCount;
			ASTNode* expr;
		} functionDecNode;
		struct {
			ASTNode* function;
			ASTNode** params;
			size_t paramCount;
		} functionCallNode;
		struct {
			BuiltInFunction builtin;
			ASTNode** params;
			size_t paramCount;
		} builtInCallNode;
		struct {
			ASTNode* left;
			ASTNode* right;
		} semicolonNode;
	};
};

// ------- environment -----------
struct EnvEntry {
	char* name;
	Value value;
	bool constant;
	struct EnvEntry* next;
};

struct Environment {
	struct EnvEntry* values;
	Environment* parent;
};

Environment* create_env(Environment* parent) {
	Environment* env = malloc(sizeof(Environment));
	env->parent = parent;
	env->values = NULL;
	return env;
}

struct EnvEntry* env_find(Environment* env, const char* name) {
	for(struct EnvEntry* e = env->values; e; e = e->next) {
		if(strcmp(e->name, name) == 0) return e;
	}
	if(env->parent) return env_find(env->parent, name);
	return NULL;
}

void env_define(Environment* env, const char* name, Value val, bool constant) {
	struct EnvEntry* prev = env_find(env, name);
	if(prev && prev->constant) {
		fprintf(stderr, "cannot assign to constant %s\n", name);
		exit(EXIT_FAILURE);
	}
	if(prev) {
		prev->value = val;
		prev->constant = constant;
	} else {
		struct EnvEntry* e = malloc(sizeof(struct EnvEntry));
		e->name = strdup(name);
		e->value = val;
		e->constant = constant;
		e->next = env->values;
		env->values = e;
	}
}

bool env_assign(Environment* env, const char* name, Value val){
	struct EnvEntry* e = env_find(env, name);
	if(e) {
		if(e->constant) {
			fprintf(stderr, "Cannot assign to constant %s\n", name);
			exit(EXIT_FAILURE);
		}
		e->value = val;
		return true;
	}
	if(env->parent) return env_assign(env->parent, name, val);
	return false;
}

Value* env_get(Environment* env, const char* name){
	struct EnvEntry* e = env_find(env, name);
	if(e) return &e->value;
	return NULL;
}

// ------- value helpers ---------

Value make_num(int v) {
	return (Value) {
		VAL_NUM, .numValue = v
	};
}

Value make_bool(bool v) {
	return (Value) {
		VAL_BOOL, .boolValue = v
	};
}

Value make_null() {
	return (Value) {
		VAL_NULL
	};
}

Value make_func(ASTNode* func, Environment* env) {
	Closure* closure = malloc(sizeof(Closure));
	closure->func = func;
	closure->env = env;
	return (Value) {
		VAL_FUNC, .funcValue = closure
	};
}

// ------- forward declarations --------

typedef struct Parser {
	TokenArray* tokens;
	size_t current;
} Parser;

Token* peek(Parser* p);
bool check(Parser* p, TokenType t);
Token* advance(Parser* p);
bool match(Parser* p, TokenType t);
void consume(Parser* p, TokenType t, const char* msg);

ASTNode* parse_expression(Parser* p);
ASTNode* parse_statement(Parser* p);
ASTNode* parse_block(Parser* p);
ASTNode* parse_function(Parser* p);
ASTNode* parse_program(Parser* p);

Token* peek(Parser* p) {
	if(p->current >= p->tokens->count) return NULL;
	return &p->tokens->tokens[p->current];
}

bool check(Parser* p, TokenType t) {
	Token* tkn = peek(p);
	return tkn && tkn->type == t;
}

Token* advance(Parser* p) {
	if(p->current < p->tokens->count)
		return &p->tokens->tokens[p->current++];
	return NULL;
}

bool match(Parser* p, TokenType t) {
	if(check(p, t)) {
		advance(p);
		return true;
	}
	return false;
}

void consume(Parser* p, TokenType t, const char* msg) {
	if(!check(p, t)) {
		fprintf(stderr, "Parse error: %s at token %zu\n", msg, p->current);
		exit(EXIT_FAILURE);
	}
	advance(p);
}

// ------- parser -------

ASTNode* parse_primary(Parser* p) {
	if (check(p, TOKEN_NUMBER)) {
		Token* t = advance(p);
		ASTNode* node = malloc(sizeof(ASTNode));
		node->type = AST_NUMBER_LITERAL;
		node->numberLiteral.value = t->intValue;
		return node;
	}
	if (check(p, TOKEN_BOOLEAN)) {
		Token* t = advance(p);
		bool val = strcmp(t->stringValue, "true") == 0;
		ASTNode* node = malloc(sizeof(ASTNode));
		node->type = AST_BOOLEAN_LITERAL;
		node->booleanLiteral.value = val;
		return node;
	}
	if (check(p, TOKEN_IDENTIFIER)) {
		Token* t = advance(p);
		ASTNode* node = malloc(sizeof(ASTNode));
		node->type = AST_IDENTIFIER;
		node->identifier.name = strdup(t->stringValue);
		node->identifier.constant = false;
		return node;
	}
	if (check(p, TOKEN_LPAREN)) {
		advance(p);
		ASTNode* expr = parse_expression(p);
		consume(p, TOKEN_RPAREN, "Expected closing )");
		return expr;
	}
	Token* t = peek(p);
	fprintf(stderr, "Unexpected token in primary: type %d", t ? (int)t->type : -1);
	if(t && (t->type == TOKEN_IDENTIFIER || t->type == TOKEN_BOOLEAN)) {
		fprintf(stderr, " ('%s')", t->stringValue);
	}
	fprintf(stderr, "\n");
	exit(EXIT_FAILURE);
}

ASTNode* parse_unary(Parser* p) {
	if(match(p, TOKEN_NOT)) {
		ASTNode* expr = parse_unary(p);
		ASTNode* node = malloc(sizeof(ASTNode));
		node->type = AST_UNOP;
		node->unOpNode.op = UNOP_NOT;
		node->unOpNode.expr = expr;
		return node;
	}
	if(match(p, TOKEN_MINUS)) {
		ASTNode* expr = parse_unary(p);
		ASTNode* node = malloc(sizeof(ASTNode));
		node->type = AST_UNOP;
		node->unOpNode.op = UNOP_NEGATE;
		node->unOpNode.expr = expr;
		return node;
	}
	return parse_primary(p);
}

int get_precedence(TokenType t) {
	switch(t) {
	case TOKEN_OROR:
		return 1;
	case TOKEN_ANDAND:
		return 2;
	case TOKEN_EQEQ:
	case TOKEN_NEQ:
		return 3;
	case TOKEN_LT:
	case TOKEN_LEQ:
	case TOKEN_GT:
	case TOKEN_GEQ:
		return 4;
	case TOKEN_PLUS:
	case TOKEN_MINUS:
		return 5;
	case TOKEN_MULTIPLY:
	case TOKEN_DIVIDE:
	case TOKEN_MODULUS:
		return 6;
	default:
		return 0;
	}
}

BinOpType token_to_binop(TokenType t) {
	switch(t) {
	case TOKEN_PLUS:
		return BINOP_ADD;
	case TOKEN_MINUS:
		return BINOP_SUBTRACT;
	case TOKEN_MULTIPLY:
		return BINOP_MULTIPLY;
	case TOKEN_DIVIDE:
		return BINOP_DIVIDE;
	case TOKEN_MODULUS:
		return BINOP_MODULUS;
	case TOKEN_LT:
		return BINOP_LT;
	case TOKEN_LEQ:
		return BINOP_LEQ;
	case TOKEN_GT:
		return BINOP_GT;
	case TOKEN_GEQ:
		return BINOP_GEQ;
	case TOKEN_EQEQ:
		return BINOP_EQUAL;
	case TOKEN_NEQ:
		return BINOP_NEQ;
	case TOKEN_ANDAND:
		return BINOP_AND;
	case TOKEN_OROR:
		return BINOP_OR;
	default:
		panic("Unknown binary operator");
		return 0;
	}
}

ASTNode* parse_binop_rhs(Parser* p, int expr_prec, ASTNode* lhs) {
	while(true) {
		Token* t = peek(p);
		int tok_prec = get_precedence(t->type);
		if(tok_prec < expr_prec)
			return lhs;
		TokenType op = t->type;
		advance(p);

		ASTNode* rhs = parse_unary(p);
		Token* next = peek(p);
		int next_prec = get_precedence(next->type);
		if(tok_prec < next_prec) {
			rhs = parse_binop_rhs(p, tok_prec+1, rhs);
		}
		ASTNode* new_lhs = malloc(sizeof(ASTNode));
		new_lhs->type = AST_BINOP;
		new_lhs->binOpNode.op = token_to_binop(op);
		new_lhs->binOpNode.left = lhs;
		new_lhs->binOpNode.right = rhs;
		lhs = new_lhs;
	}
}

ASTNode* parse_expression(Parser* p) {
	ASTNode* lhs = parse_unary(p);
	return parse_binop_rhs(p, 0, lhs);
}

ASTNode* parse_function(Parser* p) {
	consume(p, TOKEN_LPAREN, "expected '(' after func");
	ASTNode** params = NULL;
	size_t paramCount = 0;
	if(!check(p, TOKEN_RPAREN)) {
		do {
			Token* t = peek(p);
			if(t->type != TOKEN_IDENTIFIER) {
				fprintf(stderr, "expected identifier as parameter\n");
				exit(1);
			}
			params = realloc(params, sizeof(ASTNode*)*(paramCount+1));
			ASTNode* param = malloc(sizeof(ASTNode));
			param->type = AST_IDENTIFIER;
			param->identifier.name = strdup(t->stringValue);
			param->identifier.constant = false;
			params[paramCount++] = param;
			advance(p);
		} while(match(p, TOKEN_COMMA));
	}
	consume(p, TOKEN_RPAREN, "expected ')' after parameter list");
	consume(p, TOKEN_ARROW, "expected '=>' after params");
	ASTNode* body = parse_statement(p);
	ASTNode* node = malloc(sizeof(ASTNode));
	node->type = AST_FUNCTION_DEC;
	node->functionDecNode.params = params;
	node->functionDecNode.paramCount = paramCount;
	node->functionDecNode.expr = body;
	return node;
}

ASTNode* parse_block(Parser* p) {
	consume(p, TOKEN_LCURLY, "expected '{'");
	ASTNode* first = NULL;
	ASTNode* current = NULL;

	while(!check(p, TOKEN_RCURLY) && !check(p, TOKEN_EOF)) {
		ASTNode* stmt = parse_statement(p);
		if(!first) {
			first = stmt;
			current = stmt;
		} else {
			ASTNode* newnode = malloc(sizeof(ASTNode));
			newnode->type = AST_SEMICOLON;
			newnode->semicolonNode.left = current;
			newnode->semicolonNode.right = stmt;
			current = newnode;
		}
	}
	consume(p, TOKEN_RCURLY, "expected '}'");
	return first ? current : NULL;
}

ASTNode* parse_statement(Parser* p) {
	if(match(p, TOKEN_LET) || match(p, TOKEN_CONST)) {
		bool constant = p->tokens->tokens[p->current - 1].type == TOKEN_CONST;
		Token* id = peek(p);
		if(id->type != TOKEN_IDENTIFIER) {
			fprintf(stderr, "expected identifier after let or const\n");
			exit(1);
		}
		ASTNode* idNode = malloc(sizeof(ASTNode));
		idNode->type = AST_IDENTIFIER;
		idNode->identifier.name = strdup(id->stringValue);
		idNode->identifier.constant = constant;
		advance(p);
		consume(p, TOKEN_EQUAL, "expected '=' after identifier");
		ASTNode* expr = parse_expression(p);
		consume(p, TOKEN_SEMICOLON, "expected ';' after variable assignment");

		ASTNode* node = malloc(sizeof(ASTNode));
		node->type = AST_LET;
		node->letNode.identifier = idNode;
		node->letNode.expr = expr;
		node->letNode.constant = constant;

		return node;
	}
	if(check(p, TOKEN_IDENTIFIER)) {
		Token* id = peek(p);
		if((p->current + 1) < p->tokens->count && p->tokens->tokens[p->current + 1].type == TOKEN_EQUAL) {
			ASTNode* idNode = malloc(sizeof(ASTNode));
			idNode->type = AST_IDENTIFIER;
			idNode->identifier.name = strdup(id->stringValue);
			idNode->identifier.constant = false;
			advance(p);
			advance(p);
			ASTNode* expr = parse_expression(p);
			consume(p, TOKEN_SEMICOLON, "expected ; after assignment");
			ASTNode* node = malloc(sizeof(ASTNode));
			node->type = AST_ASSIGN;
			node->assignNode.identifier = idNode;
			node->assignNode.expr = expr;
			return node;
		}
	}

	if(match(p, TOKEN_FUNCTION)) {
		return parse_function(p);
	}

	if(match(p, TOKEN_OUTNUMBER)) {
		consume(p, TOKEN_LPAREN, "expected '(' after outn");
		ASTNode* arg = parse_expression(p);
		consume(p, TOKEN_RPAREN, "expected ')' after outn argument");
		consume(p, TOKEN_SEMICOLON, "expected ';' after outn statement");

		ASTNode** params = malloc(sizeof(ASTNode*));
		params[0] = arg;
		ASTNode* node = malloc(sizeof(ASTNode));
		node->type = AST_BUILTIN_CALL;
		node->builtInCallNode.builtin = BUILTIN_OUTNUMBER;
		node->builtInCallNode.params = params;
		node->builtInCallNode.paramCount = 1;
		return node;
	}

	if(match(p, TOKEN_LCURLY)) {
		p->current--;
		return parse_block(p);
	}
	ASTNode* expr = parse_expression(p);
	consume(p, TOKEN_SEMICOLON, "expected ';' after expression");
	return expr;
}

ASTNode* parse_program(Parser* p) {
	ASTNode* first = NULL;
	ASTNode* current = NULL;

	while(!check(p, TOKEN_EOF)) {
		ASTNode* stmt = parse_statement(p);
		if(!first) {
			first = stmt;
			current = stmt;
		} else {
			ASTNode* newnode = malloc(sizeof(ASTNode));
			newnode->type = AST_SEMICOLON;
			newnode->semicolonNode.left = current;
			newnode->semicolonNode.right = stmt;
			current = newnode;
		}
	}
	return first ? current : NULL;
}

// ------- interpreter -------

Environment* create_child_env(Environment* parent) {
	return create_env(parent);
}

Value interpret(ASTNode* node, Environment* env);
void checkNumber(Value v, const char* op) {
	if(v.type != VAL_NUM) panicf("operator '%s' expected number", op);
}

void checkBoolean(Value v, const char* op) {
	if(v.type != VAL_BOOL) panicf("operator '%s' expected boolean", op);
}

Value interpret(ASTNode* node, Environment* env) {
	if(!node) return make_null();
	switch(node->type) {
	case AST_NUMBER_LITERAL:
		return make_num(node->numberLiteral.value);
	case AST_BOOLEAN_LITERAL:
		return make_bool(node->booleanLiteral.value);
	case AST_IDENTIFIER: {
		Value* val = env_get(env, node->identifier.name);
		if(!val) panicf("undefined variable %s", node->identifier.name);
		return *val;
	}
	case AST_LET: {
		Value val = interpret(node->letNode.expr, env);
		env_define(env, node->letNode.identifier->identifier.name, val, node->letNode.constant);
		return val;
	}
	case AST_ASSIGN: {
		Value val = interpret(node->assignNode.expr, env);
		if(!env_assign(env, node->assignNode.identifier->identifier.name, val))                     panicf("assign to undefined variable %s", node->assignNode.identifier->identifier.name);
		return val;
	}
	case AST_BINOP: {
		Value left = interpret(node->binOpNode.left, env);
		Value right = interpret(node->binOpNode.right, env);
		switch(node->binOpNode.op) {
		case BINOP_ADD:
			checkNumber(left, "+");
			checkNumber(right, "+");
			return make_num(left.numValue + right.numValue);
		case BINOP_SUBTRACT:
			checkNumber(left, "-");
			checkNumber(right, "-");
			return make_num(left.numValue - right.numValue);
		case BINOP_MULTIPLY:
			checkNumber(left, "*");
			checkNumber(right, "*");
			return make_num(left.numValue * right.numValue);
		case BINOP_DIVIDE: {
			checkNumber(left, "/");
			checkNumber(right, "/");
			if(right.numValue == 0) panic("division by zero");
			return make_num(left.numValue / right.numValue);
		}
		case BINOP_MODULUS: {
			checkNumber(left, "%");
			checkNumber(right, "%");
			if(right.numValue == 0) panic("modulus by zero");
			return make_num(left.numValue % right.numValue);
		}
		case BINOP_LT:
			checkNumber(left, "<");
			checkNumber(right, "<");
			return make_bool(left.numValue < right.numValue);
		case BINOP_LEQ:
			checkNumber(left, "<=");
			checkNumber(right, "<=");
			return make_bool(left.numValue <= right.numValue);
		case BINOP_GT:
			checkNumber(left, ">");
			checkNumber(right, ">");
			return make_bool(left.numValue > right.numValue);
		case BINOP_GEQ:
			checkNumber(left, ">=");
			checkNumber(right, ">=");
			return make_bool(left.numValue >= right.numValue);
		case BINOP_EQUAL: {
			if(left.type != right.type) return make_bool(false);
			if(left.type == VAL_NUM) return make_bool(left.numValue == right.numValue);
			if(left.type == VAL_BOOL) return make_bool(left.boolValue == right.boolValue);
			return make_bool(false);
		}
		case BINOP_NEQ: {
			if(left.type != right.type) return make_bool(true);
			if(left.type == VAL_NUM) return make_bool(left.numValue != right.numValue);
			if(left.type == VAL_BOOL) return make_bool(left.boolValue != right.boolValue);
			return make_bool(true);
		}
		case BINOP_AND:
			checkBoolean(left, "&&");
			checkBoolean(right, "&&");
			return make_bool(left.boolValue && right.boolValue);
		case BINOP_OR:
			checkBoolean(left, "||");
			checkBoolean(right, "||");
			return make_bool(left.boolValue || right.boolValue);
		}
		panic("unknown binary operator (interpret)");
	}
	case AST_UNOP: {
		Value val = interpret(node->unOpNode.expr, env);
		switch(node->unOpNode.op) {
		case UNOP_NEGATE:
			checkNumber(val, "-");
			return make_num(-val.numValue);
		case UNOP_NOT:
			checkBoolean(val, "!");
			return make_bool(!val.boolValue);
		default:
			panic("unknown unary operator");
		}
	}
	case AST_SEMICOLON:
		interpret(node->semicolonNode.left, env);
		return interpret(node->semicolonNode.right, env);
	case AST_BLOCK:
		return interpret(node->blockNode.expr, env);
	case AST_BUILTIN_CALL: {
		if(node->builtInCallNode.builtin == BUILTIN_OUTNUMBER) {
			Value val = interpret(node->builtInCallNode.params[0], env);
			switch(val.type) {
			case VAL_NUM:
				printf("%d\n", val.numValue);
				break;
			case VAL_BOOL:
				printf("%s\n", val.boolValue ? "true" : "false");
				break;
			case VAL_FUNC:
				printf("<function>\n");
				break;
			default:
				printf("null\n");
			}
			return make_bool(true);
		}
		panic("unknown builtin");
	}
	default:
		panic("not implemented AST node interpretation");
	}
	return make_null();
}

// ------- main ----------
int main(int argc, char** argv) {
	if(argc != 2) {
		printf("usage: ./slug <script.slg>\n");
		return 1;
	}
	FILE* f = fopen(argv[1], "r");
	if(!f) {
		perror("cannot open file");
		return 1;
	}
	fseek(f, 0, SEEK_END);
	long filesize = ftell(f);
	fseek(f, 0, SEEK_SET);
	char* source = malloc(filesize + 1);
	fread(source, 1, filesize, f);
	fclose(f);
	source[filesize] = 0;
	TokenArray tokens;
	tokenArrayInit(&tokens);
	tokenize(source, &tokens);
	free(source);
	Parser parser = { &tokens, 0 };
	ASTNode* program = NULL;
	if(check(&parser, TOKEN_LCURLY)) {
		program = parse_block(&parser);
	} else {
		program = parse_program(&parser);
	}
	Environment* global_env = create_env(NULL);
	interpret(program, global_env);
	tokenArrayFree(&tokens);
	return 0;
}
