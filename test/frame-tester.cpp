#include "../src/parser.hpp"
#include "../src/frame.hpp"
#include "tester.hpp"

using namespace std;
using namespace ice_language;

#define PRE AST::interpretive() = true; \
            auto frame = make_shared<Frame>();\
            auto code = make_shared<Code>(); \
            auto ast = Parser(ss).gen_statements();\
            ast->codegen(*code);\
            frame->execute_code(code);

TEST_CLASS(Frame)
    TEST_METHOD(SimpleRun)
        istringstream ss(R"(
a: 1;
b: 2;
b: a : 3;
a + b;
        )");
        PRE;
        ASSERT(frame->pop()->to_str() == "6");
    TEST_END

    TEST_METHOD(SimpleFunc)
        istringstream ss(R"(
@adddd: @(a): @(b): @(c): @(d): a + b + c + d;
adddd(1)(2)(3)(4);
        )");
        PRE;
        ASSERT(frame->pop()->to_str() == "10");
    TEST_END

    TEST_METHOD(SimpleIfElseStmt)
        istringstream ss(R"(
a: 1;
@foo(x)
{
    a : 1;
    if x
    {
        return a: 2;
    }
    else
    {
        return a: 3;
    };
};
foo(1);
foo(0);
        )");
        PRE;
        ASSERT(frame->pop()->to_str() == "3");
        ASSERT(frame->pop()->to_str() == "2");
    TEST_END

    TEST_METHOD(Y)
        istringstream ss(R"(
@Y(f):
  (@(x): f(delay x(delay x)))
  (@(x): f(delay x(delay x)));

@fact(f):
  @(n): n ? (n * f(n-1)) , 1;

@result: Y(fact)(5);
        )");
        PRE;
        // code.print();
        ASSERT(frame->pop()->to_str() == "120");
    TEST_END
TEST_END
