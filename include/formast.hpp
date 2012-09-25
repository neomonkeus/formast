#ifndef FORMAST_HPP_INCLUDED
#define FORMAST_HPP_INCLUDED

#include <boost/cstdint.hpp>
#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>
#include <list>
#include <string>
#include <vector>

#include "formast/detail/ast.hpp"

namespace formast
{

typedef formast::detail::ast::Top Top;
typedef formast::detail::ast::Stats Stats;
typedef formast::detail::ast::Expr Expr;

typedef std::string Doc;

struct Attr {
    std::string class_name;
    std::string name;
    boost::optional<Doc> doc;
};

struct Class {
    std::string name;
    boost::optional<std::string> base_name;
    boost::optional<Doc> doc;
    boost::optional<Stats> stats;
};

struct If {
    Expr expr;
    Stats stats;
};

struct IfElifsElse {
    std::vector<If> ifs_;
    boost::optional<Stats> else_;
};

class Parser
{
public:
    Parser();
    virtual Expr parse_stream(std::istream & is) = 0;
    Expr parse_string(std::string const & s);
    // TODO: new parser API
    //virtual void parse_stream(std::istream & is, Scope & scope) = 0;
    //void parse_string(std::string const & s, Scope & scope);
};

class XmlParser : public Parser
{
public:
    XmlParser();
    virtual Expr parse_stream(std::istream & is);
    // TODO: new parser API
    //virtual void parse_stream(std::istream & is, Scope & scope);
};

class Visitor
{
public:
    Visitor();
    virtual ~Visitor();

    virtual void top(Top const & top);
    virtual void top_class(Class const & class_);

    virtual void stats(Stats const & stats);
    virtual void stats_attr(Attr const & attr);
    virtual void stats_if_elifs_else(IfElifsElse const & ifelifselse);

    virtual void expr(Expr const & e);
    virtual void expr_uint(boost::uint64_t const & n);
    virtual void expr_id(std::string const & i);
    virtual void expr_pos(Expr const & right);
    virtual void expr_neg(Expr const & right);
    virtual void expr_add(Expr const & left, Expr const & right);
    virtual void expr_sub(Expr const & left, Expr const & right);
    virtual void expr_mul(Expr const & left, Expr const & right);
    virtual void expr_div(Expr const & left, Expr const & right);
private:
    // pimpl idiom
    class ExprVisitor;
    boost::shared_ptr<ExprVisitor> _expr_visitor;
    class ScopeVisitor;
    boost::shared_ptr<ScopeVisitor> _scope_visitor;
};

} // namespace formast

#endif
