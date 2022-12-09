//
// Created by msobral on 08/12/2022.
//

#include "ArgTokenizer.h"
#include <regex>
#include <stdexcept>
#include <utility>

std::optional<std::pair<std::string, std::string>> extract_option(const std::string & tok, const std::regex & expr, int pos) {
    std::smatch sm;

    if (!std::regex_match(tok, sm, expr)) {
        auto name = sm[pos].str();
        auto val = sm[pos+2].str();
        return std::make_optional(std::make_pair(name, val));
    }

    return std::nullopt;
}

Arg::Arg(const std::string & tok) {
    auto expr = this->get_regex();
    auto val_pos = this->get_value_pos();
//    static std::regex expr1("-([a-zA-Z])(=[^\\b]{0,})?");
//    static std::regex expr2("--([a-zA-Z]{2,})(=[^\\b]{0,})?");
    std::smatch sm;

    if (auto op = extract_option(tok, expr, val_pos)) {
        auto info = op.value();
        _name  = info.first;
        this->setup_default_value(info.second);
    } else {
        throw std::invalid_argument("invalid argument option");
    }
}

bool Arg::is_short() const {
    return _name.size() == 1 ;
}

std::string Arg::name() const {
    return _name;
}

bool Arg::operator==(const Arg &arg) const {
    return (_name == arg._name);
}

bool Arg::operator!=(const Arg &arg) const {
    return (_name != arg._name);
}

bool Arg::operator<(const Arg &arg) const {
    return (_name < arg._name);
}

std::vector<Arg> make_args(std::string_view args) {
    static const std::string expr="((-[a-zA-Z]|--[a-zA-Z]{2,})(=[^ ]{0,})?|-[a-zA-Z]{2,})($|\\s)";

    std::regex e(expr);
    std::regex_iterator<std::string_view::iterator> it(args.begin(), args.end(), e);
    std::regex_iterator<std::string_view::iterator> rend;

    std::vector<Arg> res;
    std::transform(it, rend, std::back_inserter(res), [](auto & tok) { return Arg(tok);});

    return res;
}

std::vector<Arg> make_args(const char **argv) {
    std::string args;
    for (auto ** ptr = argv; *ptr != NULL; ptr++) {
        args += *ptr + ' ';
    }
    if (! args.empty()) {
        args.pop_back();
    }
    return make_args(args);
}

constexpr std::regex ArgSingle::get_regex() const {
    return std::__cxx11::regex("-([a-zA-Z])(=[^\\b]{0,})?");
}

constexpr int ArgSingle::get_value_pos() const {
    return 1;
}

void ArgSingle::setup_default_value(const std::string &val) {
    _val = val;
}
