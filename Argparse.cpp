/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Argparse.cpp
 * Author: msobral
 * 
 * Created on 22 de Agosto de 2017, 16:46
 */

#include "Argparse.h"
#include <regex>

Argparse::Argparse() {
}

Argparse::Argparse(const Argparse& orig) {
    opts = orig.opts;
}

Argparse::~Argparse() {    
}

void Argparse::add_flag(const string& name, string_view ajuda) {
    opts.emplace(name, std::make_pair(std::make_optional(false), ajuda));
}

bool Argparse::get_flag(const string & name) const {
    auto op = get_option<bool>(name);
    return op.value_or(false);
}

bool Argparse::has_value(const string &name) const {
    return has_some_value<int,string,float,bool>(name);

//    if (auto op = has_value_of<int>(name)) {
//        return op.value();
//    }
//    if (auto op = has_value_of<string>(name)) {
//        return op.value();
//    }
//    if (auto op = has_value_of<float>(name)) {
//        return op.value();
//    }
//    if (auto op = has_value_of<bool>(name)) {
//        return op.value();
//    }
//    return false;
}

//int Argparse::parse(char* argv[]) {
//    int n = 1, cnt = 0;
//    int state = 1;
//    string op;
//
//    while (argv[n]) {
//        switch (state) {
//            case 1: { // espera uma opção
//                op = argv[n];
//                if (op[0] != '-') { // final das opções ???
//                    int k = n;
//                    for (; argv[k] != NULL; k++) {
//                        extra += argv[k];
//                        extra += ' ';
//                    }
//                    return n;
//                }
//                auto jt = flags.find(op);
//                if (jt != flags.end()) { // encontrou uma flag
//                    jt->second = true;
//                } else {
//                    auto it = opts.find(op);
//                    if (it == opts.end()) {
//                        if (multiopts.count(op) == 0) throw std::invalid_argument(op+": opçao inválida"); // opção inválida
//                    }
//                    state = 2;
//                }
//                break;
//            }
//            case 2: { // espera um argumento da opção anterior
//                string val = argv[n];
//                if (val[0] == '-') throw std::invalid_argument(val+": argumento não pode iniciar com hífen"); // argumento não pode iniciar com hifen
//                auto p = opts.find(op);
//                if (p == opts.end()) {
//                    auto & v = multiopts[op];
//                    v.push_back(val);
//                    int i = v.size();
//                    i = i+1;
//                } else p->second = val;
//                //opts[op] = val;
//                state = 1;
//                cnt++;
//            }
//        }
//        n++;
//    }
//    return cnt;
//}

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

static auto f_string = [](const std::string & s){return s;};
static auto f_int = [](const std::string & s){return std::stoi(s);};
static auto f_float = [](const std::string & s){return std::stof(s);};
static auto f_bool = [](const std::string & s){return true;};

bool Argparse::set_option(const string &name, const string &val) {
    return set_option2(name, val, f_string).value_or(
            set_option2(name, val, f_int).value_or(
                    set_option2(name, val, f_float).value_or(
                            set_option2(name, val, f_bool).value_or(false)
                            )
                    )
            );
}

bool Argparse::set_multioption(const string &name, const string &val) {
    return set_multioption2(name, val, f_string).value_or(
            set_multioption2(name, val, f_int).value_or(
                    set_multioption2(name, val, f_float).value_or(false)
                    )
            );
}

int Argparse::parse(std::string_view args) {
    static const string expr="(-[a-zA-Z]|--[a-zA-Z]{2,})(\\s+[^-\\s]{0,})?";
    auto extract_option = [](const string & tok, std::regex & e) -> std::optional<std::pair<std::string, std::string>> {
        std::smatch sm;
        std::regex_match(tok, sm,  e);
        if (sm.size() > 1) {
            return std::make_optional(std::make_pair(sm[1], sm[2]));
        }
        return std::nullopt;
    };

    std::regex e(expr);
    std::regex_iterator<std::string_view::iterator> it(args.begin(), args.end(), e);
    std::regex_iterator<std::string_view::iterator> rend;
    int cnt = 0;

    for (; it != rend; ++it, cnt++) {
        auto op = extract_option(it->str(), e);
        if (op) {
            auto [opname, opval] = op.value();
            if (! set_option(opname, opval)) {
                if (!set_multioption(opname, opval)) {
                    throw std::invalid_argument("opção e/ou valor inválidos: opção="+opname+", valor="+opval);
                }
            }
        }
    }

    return cnt;
}

int Argparse::parse(char* argv[]) {
    std::string args;
    auto ** ptr = argv;
    for (ptr++; *ptr != NULL; ptr++) {
        args += *ptr + ' ';
    }
    if (! args.empty()) {
        args.pop_back();
    }
    return parse(args);
}

