//
// Created by msobral on 08/12/2022.
//

#ifndef ARGPARSE_DEMO_ARGTOKENIZER_H
#define ARGPARSE_DEMO_ARGTOKENIZER_H

#include <string>
#include <string_view>
#include <optional>
#include <vector>
#include <regex>

class Arg {
public:
    Arg(const std::string & cmdline);
    Arg(const Arg & arg) = default;
    Arg(Arg && arg) = default;
    Arg() = default;
    ~Arg() = default;
    bool is_short() const;
    std::string name() const;
    virtual bool has_value() const = 0;
    virtual std::string value() const = 0;

    bool operator==(const Arg & arg) const;
    bool operator!=(const Arg & arg) const;
    bool operator<(const Arg & arg) const;

protected:
    std::string _name;

    virtual std::regex get_regex() const = 0;
    virtual int get_value_pos() const = 0;
    virtual void setup_default_value(const std::string & val) = 0;
};

class ArgSingle: public Arg {
public:
    ArgSingle(const std::string & cmdline);
    bool has_value() const;
    std::string value() const;

private:
    std::string _name;
    std::string _val;

    virtual constexpr std::regex get_regex() const;
    virtual constexpr int get_value_pos() const;
    virtual void setup_default_value(const std::string & val);
};

class ArgLong: public ArgSingle {
public:
    ArgLong(const std::string & cmdline);
    bool has_value() const;
    std::string value() const;

private:
    std::string _name;
    std::string _val;

    virtual constexpr std::regex get_regex() const;
    virtual constexpr int get_value_pos() const;
    virtual void setup_default_value(const std::string & val);
};

class ArgFlag: public Arg {
public:
    ArgFlag(const std::string & cmdline);
    bool has_value() const;
    std::string value() const;

private:
    std::string _name;
    std::string _val;

    virtual constexpr std::regex get_regex() const;
    virtual constexpr int get_value_pos() const;
    virtual void setup_default_value(const std::string & val);
};

class ArgMulti: public Arg {
public:
    ArgMulti(const std::string & cmdline);
    bool has_value() const;
    std::string value() const;

private:
    std::string _name;
    std::string _val;

    virtual constexpr std::regex get_regex() const;
    virtual constexpr int get_value_pos() const;
    virtual void setup_default_value(const std::string & val);
};

std::vector<Arg> make_args(const std::string_view  cmdline);
std::vector<Arg> make_args(const char * args[]);

#endif //ARGPARSE_DEMO_ARGTOKENIZER_H
