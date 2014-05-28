

#include <iostream>
#include <string>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>


using std::string;
using std::cin;
using std::getline;
namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
using namespace boost::phoenix;

template <typename Iterator>
struct unescaped_string
    : qi::grammar<Iterator, std::string()>
{
    unescaped_string()
    : unescaped_string::base_type(unesc_str)
    {
    unesc_char.add("\\a", '\a')("\\b", '\b')("\\f", '\f')("\\n", '\n')
        ("\\r", '\r')("\\t", '\t')("\\v", '\v')
        ("\\\\", '\\')("\\\'", '\'')("\\\"", '\"')
        ;
    
        unesc_str = qi::lit('"')
        >> *(unesc_char | "\\x" >> qi::hex | qi::print - '"')
            >>  qi::lit('"')
        ;
    }
    
    qi::rule<Iterator, std::string()> unesc_str;
    qi::symbols<char const, char const> unesc_char;
};

template <typename Iterator>
struct identifier
    : qi::grammar<Iterator, std::string()>
{
    identifier()
    : identifier::base_type(id)
    {
    id = *(qi::alnum | qi::punct-")") ;
    }

    qi::rule<Iterator, std::string()> id;
};

template <typename Iterator>
struct list
    : qi::grammar<Iterator, std::list<std::string>()>
{
    list()
    :list::base_type(lst)
    {
    lst = qi::lit("(") >> id % ascii::space >> qi::lit(")");
    }
    
    identifier<Iterator> id;
    qi::rule<Iterator, std::list<std::string>()> lst;
};


int main()
{
    //  string str;
  
    typedef std::string::const_iterator iterator_type;
    
    std::string parsed;
    std::string str("\"string to unescape:\\x20\\n\\r\\t\\\"\\'\\x41\"");
    iterator_type iter = str.begin();
    iterator_type end = str.end();
    unescaped_string<iterator_type> p;
  
  
    qi::parse(iter, end, p, parsed);
    std::cout << parsed << std::endl;

    str = " id1 idd.3d sec 189 sesc-+-=|&^%:3y ";
    iter = str.begin();
    end = str.end();
    parsed = "";

    identifier<iterator_type> iq;

    while (++iter != end)
    {
    qi::parse(iter, end, iq, parsed);
    std::cout<< parsed <<std::endl;
    }

    str = "(a b e)";
    iter = str.begin();
    end = str.end();
    
    std::vector<std::string> vparsed;
    
    list<iterator_type> lq;
    qi::parse(iter, end, lq, vparsed);
    std::vector<std::string>::iterator i = vparsed.begin();
    while (i != vparsed.end())
    {
    std::cout << *i++ << ", " ;
    }
    /*
    while (getline(cin, str))
    {
    using qi::_1;
    
    double res = 0;
    if (str.empty() || str[0] == 'q') break;
    
    std::string::const_iterator b = str.begin();
    std::string::const_iterator e = str.end();
    
    qi::phrase_parse(b, e, qi::double_[ref(res)=_1], ascii::space);
    
      std::cout <<" result: " << res << std::endl;
      }*/
    return 0;
}
