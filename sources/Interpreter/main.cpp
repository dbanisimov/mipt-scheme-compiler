#include "interpreter_dep.hpp"
#include "context.hpp"
#include "lambda.hpp"
#include "std_forms.hpp"
#include "environment.hpp"
#include "evaluator.hpp"
#include "qquote_expander.hpp"
#include "pretty_printer.hpp"
#include "interpreter_exception.hpp"

using namespace parser;
using namespace parser::ast;
using namespace interpreter;
using namespace lambda;

void repl( std::istream &in, std::ostream &out, std::ostream &err, bool interactive)
{
    StringTable strs;
    Parser p( &strs, &in);
    Context ctx( &strs);
    registerStdForms( &ctx, &strs);
    FunctionManager fm;
    MacroManager mm;
    Gensym gs( &strs);
    QQuoteExpander qq( &strs);
    ctx.regSpec( strs.id( "lambda"), fm);
    ctx.regSpec( strs.id( "macros"), mm);
    ctx.regSpec( strs.id( "quasiquote"), qq);
    ctx.regSpec( strs.id( "gensym"), gs);

    Evaluator e( &ctx);
    AstPrinter pr( out);
    PrettyPrinter pp( out, &strs);

    if ( interactive)
        out <<"Scheme interpreter 0.5. Use with caution." <<std::endl;
    while (!p.eof())
    {
        try
        {
            bool end = false;
            SExprp exp = p.read( &end);
            if ( end) break;
            pp.print( e.eval( exp, false));
            if ( interactive)
                out <<std::endl <<"\033[0;32m8-0\033[0m> ";
        }
        catch (interpreter::Exception& e)
        {
            e.backtrace().print( err, &strs);
            err <<"\nYour bad: " << e.what() <<" in: ";
            PrettyPrinter pp( err, &strs);
            pp.print( e.problem());

            if ( interactive)
                out <<std::endl <<"\033[0;31m%~#\033[0m> ";
        }
        catch (std::exception& e)
        {
            err << "My bad: " << e.what();
            if ( interactive)
                out <<std::endl <<"\033[0;31m%~#\033[0m> ";
        }
    }
}

int main()
{
    repl( std::cin, std::cout, std::cerr, true);
}
