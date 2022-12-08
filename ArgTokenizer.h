//
// Created by msobral on 08/12/2022.
//

#ifndef ARGPARSE_DEMO_ARGTOKENIZER_H
#define ARGPARSE_DEMO_ARGTOKENIZER_H

#include <string>
#include <string_view>
#include <optional>
#include <vector>

class Arg {
public:
    Arg(const std::string & cmdline);
    Arg(const Arg & arg) = default;
    Arg(Arg && arg) = default;
    Arg() = default;
    ~Arg() = default;
    bool is_short() const;
    std::string name() const;
    bool has_value() const;
    std::string default_value() const;
private:
    std::string _name;
    std::string _val;
};

std::vector<Arg> make_args(const std::string_view  cmdline);
std::vector<Arg> make_args(const char * args[]);

#endif //ARGPARSE_DEMO_ARGTOKENIZER_H
