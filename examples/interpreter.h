enum Binop { plus, minus, times, divi };
enum Type {
  CompoundStm, // Stm -> Stm; Stm
  AssignStm, // Stm -> id := Expr
  PrintStm, // Stm -> print(ExprList)
  IdExp, // Expr -> id
  NumExp, // Expr -> num
  OpExp, // Expr -> Expr Binop Expr
  EseqExp, // Expr -> (Stm, Expr)
  PairExpList, // ExprList -> Expr, ExprList
  LastExpList // ExprList -> Expr
};

class Table {
public:
  virtual int lookup(char *key)=0;
  virtual Table *update(char *id, int value, Table *base)=0;
};

struct Result {
  int caculated;
  Table *table;
};

class Expression;
class ExprList;

class Statement {
  enum Type kind;
  union U {
    struct { Statement *stm1; Statement *stm2; } compound;
    struct { char *id; Expression *expr; } assign;
    struct { ExprList *exprs; } print;
  } u;

  Table *eval(Table *table);

public:
  friend class Expression;
  Statement(Statement *stm1, Statement *stm2);
  Statement(const char *id, Expression *expr);
  Statement(ExprList *exprs);

  int maxargs();
  void interpret();
};

class ExprList {
  enum Type kind;
  union {
    struct { Expression *head; ExprList *tail; } pair;
    Expression *last;
  } u;

public:
  friend class Statement;
  ExprList(Expression *head, ExprList *tail);
  ExprList(Expression *last);
  int length();
};

class Expression {
  enum Type kind;
  union {
    char *id;
    int num;
    struct { Expression *left; Binop oper; Expression *right; } op;
    struct { Statement *stm; Expression *expr; } eseq;
  } u;

  struct Result eval(Table *table);

public:
  friend class Statement;
  Expression(const char *id);
  Expression(int num);
  Expression(Expression *left, Binop oper, Expression *right);
  Expression(Statement *stm, Expression *expr);
};

class ListTable : public Table {
  char *id;
  int value;
  ListTable *tail;
public:
  ListTable();
  ListTable(char *id, int value, ListTable *tail);
  int lookup(char *key);
  Table *update(char *id, int value, Table *base);
};

class TreeTable : public Table {
  char *id;
  int value;
  TreeTable *left;
  TreeTable *right;
public:
//    TreeTable(char* id, int value, TreeTable* left, TreeTable* right);
//    int lookup(char* key);
//    TreeTable* update(char* id, int value, TreeTable* base);
};