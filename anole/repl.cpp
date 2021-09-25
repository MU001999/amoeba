#include "repl.hpp"
#include "anole.hpp"

#ifdef __linux__
    #include <readline/readline.h>
    #include <readline/history.h>

    #include <setjmp.h>
    #include <unistd.h>
    #include <signal.h>
#else
    #error "only support linux"
#endif

#include <cstdio>
#include <sstream>
#include <iostream>

namespace anole
{
namespace // local
{
sigjmp_buf localEnv;

String read_line(const char * str)
{
    char *temp = readline(str);
    if (!temp)
    {
        exit(0);
    }
    String line = temp;
    free(temp);
    return line;
}

void handle_sigint(int)
{
    std::cout << "\b \b\b \b\nKeyboard Interrupt\n";
    rl_on_new_line();
    rl_replace_line("", 0);
    siglongjmp(localEnv, 1);
}
}

void replrun::run()
{
    signal(SIGINT, handle_sigint);
    rl_bind_key('\t', rl_insert);
    // read_history(NULL);

    printf("\
    _                _" R"(
   / \   _ __   ___ | | ___
  / _ \ | '_ \ / _ \| |/ _ \
 / ___ \| | | | (_) | |  __/   %s
/_/   \_\_| |_|\___/|_|\___|)""\n\n", Version::literal);

    String line;
    std::istringstream ss;
    Parser parser(ss, "<stdin>");
    /**
     * theWorkingPtah is specific for here
    */
    auto code = std::make_shared<Code>("<stdin>", theWorkingPath);
    theCurrContext = std::make_shared<Context>(code);

    parser.set_resume_action([&ss, &parser]
    {
        auto line = read_line(".. ");
        if (!line.empty())
        {
            add_history(line.c_str());
            // write_history(line.c_str());
        }
        ss.clear();
        ss.str(line += '\n');
        parser.resume();
    });

    sigsetjmp(localEnv, 1);
    for (;;) try
    {
        do
        {
            line = read_line(">> ");
        } while (line.empty());
        add_history(line.c_str());
        // write_history(line.c_str());
        ss.clear();
        ss.str(line + '\n');
        parser.reset();

        theCurrContext->pc() = code->size();

        Ptr<AST> stmt = parser.gen_statement();
        if (stmt == nullptr)
        {
            continue;
        }

        if (stmt->is_expr_stmt())
        {
            ArgumentList args;
            args.emplace_back(move(reinterpret_cast<ExprStmt *>(stmt.get())->expr), false);
            stmt = std::make_unique<ParenOperatorExpr>(
                std::make_unique<IdentifierExpr>("println"),
                std::move(args)
            );
        }
        stmt->codegen(*code);

      #ifdef _DEBUG
        code->print(std::cerr);
      #endif

        Context::execute();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}
}
