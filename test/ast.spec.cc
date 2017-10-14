#include <assert.h>
#include <stdio.h>
#include "../src/utils/index.h"
#include "../src/lexical/index.h"
#include "../src/ast/index.h"
#include "../src/semantic/errcode.h"
#include "global.h"

extern int yyparse(void);

namespace ast {
int parse(const char *fname) {
  lexical::reset(fname);
  ast::clearErrorCode();
  if (yyparse()==0) {
    return 0;
  } else {
    return 1;
  }
}

void debug(const char *file) {
  verboseError();
  printf("\n~~~~~~ DEBUGGING AST STARTED ~~~~~~\n\n");

  assert(ast::parse(file)==0);

  if (ast::AST_ROOT==nullptr) {
    printf("⚠️  %s has no content", TIGER_DEFAULT_INPUT);
  } else {
    ast::AST_ROOT->print();
    ast::AST_ROOT->semantic(ast::makeBaseEnvTable());
  }

  printf("\n\n~~~~~~ DEBUGGING AST ENDED ~~~~~~\n\n");
}

void debug() {
  debug(TIGER_DEFAULT_INPUT);
}
}

int CODES[] = {
    NO_ERROR, // test1.tig
    NO_ERROR,
    NO_ERROR,
    NO_ERROR,
    NO_ERROR,
    NO_ERROR, // test6.tig
    NO_ERROR,
    NO_ERROR,
    IF_EXPR_ERROR1,
    NO_ERROR,
    NO_ERROR, // test11.tig
    NO_ERROR,
    OPERATOR_DIFF_TYPE,
    OPERATOR_DIFF_TYPE,
    NO_ERROR,
    NO_ERROR, // test16.tig
    NO_ERROR,
    NO_ERROR,
    VAR_NOT_DEFINED,
    VAR_NOT_DEFINED,
    NO_ERROR, // test21.tig
    FILED_ERR1,
    NOT_ASSIGNABLE,
};

static int __TIGER_UNIT_TEST = describe("ast tree", [] {
//  return ast::debug("../mocks/test23.tig");
  slientError();

  it("should parse ast correct", [] {
    for (auto i: TIGS) {
      assert(ast::parse(i.c_str())==0);
    }
  });

  it("should semantic check correct", [] {
      auto base_env = ast::makeBaseEnvTable();
      auto index = 0;
      auto length = sizeof(CODES) / sizeof(int);

      for (auto i: TIGS) {
        ast::parse(i.c_str());

        if (ast::AST_ROOT) {
          ast::AST_ROOT->semantic(base_env);
          assert(ast::getErrorCode() == CODES[index]);
        }

        index++;

        if (index >= length) break;
      }
  });

});
