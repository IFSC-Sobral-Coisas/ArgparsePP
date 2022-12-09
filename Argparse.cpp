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
#include <algorithm>

Argparse::Argparse() {
}

Argparse::Argparse(const Argparse& orig) {
    opts = orig.opts;
}

Argparse::Argparse(std::string_view args) {
    auto v_args = make_args(args);
    setup(v_args);
}

Argparse::Argparse(const char **argv) {
    auto v_args = make_args(argv);
    setup(v_args);
}

void Argparse::setup(const std::vector<Arg> &args) {
    if (args.empty()) return;

    std::sort(args.begin(), args.end());
    auto & last = args[0];
    for (auto it = args.begin()+1; it != args.end(); it++) {

    }
}

void Argparse::setup_option(const Arg &arg) {
    if (arg.is_flag()) {
        flags.emplace(arg.name(), false);
    } else if (arg.is_multi())
}

Argparse::~Argparse() {    
}

void Argparse::add_multioption(const string& longoption) {    
    string option = normalize_option(longoption);
    if (multiopts.count(option) == 0) multiopts[option];
}

void Argparse::add_multioption(char shortoption) {    
    string op;
    op += shortoption;
    add_multioption(op);
}

void Argparse::add_option(const string& longoption) {    
    set_option(longoption, std::nullopt);
}

void Argparse::add_option(char shortoption) {
    string op;
    op += shortoption;
    set_option(op, std::nullopt);
}

string Argparse::normalize_option(const string& longoption) const {
    string option = longoption;
    
    if (option.size() == 1) option.insert(0, 1, '-');
    else option.insert(0, 2, '-');
    
    return option;
}

void Argparse::add_option(const string& longoption, const string& defval) {
    set_option(longoption, make_optional(defval));
}

void Argparse::add_option(char shortoption, const string& defval) {
    string op;
    op += shortoption;
    set_option(op, make_optional(defval));
}

void Argparse::set_option(const string &longoption, const optional<string> &val) {
    string option = normalize_option(longoption);

    if (opts.count(option) > 0) throw std::invalid_argument("opção "+longoption+" já definida"); // erro: opção já cadastrada
    opts[option] = val;
}

void Argparse::add_flag(const string& longoption) {
    add_flag(longoption, false);
}

void Argparse::add_flag(char shortoption) {
    add_flag(shortoption, false);
}

void Argparse::add_flag(const string& longoption, bool defval) {
    string option = longoption;
    
    if (option.size() == 1) option.insert(0, 1, '-');
    else option.insert(0, 2, '-');
    
    if (opts.count(option) > 0) throw std::invalid_argument(option + ": já definida"); // erro: opção já cadastrada
    flags[option] = defval;
}

void Argparse::add_flag(char shortoption, bool defval) {
    string op;
    op += shortoption;
    add_flag(op, defval);
}

bool Argparse::get_flag(char shortoption) const {
    string op;
    op += shortoption;

    return get_flag(op);
}

bool Argparse::get_flag(const string & longoption) const {
    string option = longoption;
    
    if (option.size() == 1) option.insert(0, 1, '-');
    else option.insert(0, 2, '-');

    try {
        return flags.at(option);
    } catch (...) {
        throw std::invalid_argument(option+": flag inválida"); // flag inválida
    }
}

optional<string> Argparse::get_option(char shortoption) const {
    string op;
    op += shortoption;

    return get_option(op);
}

optional<string> Argparse::get_option(const string & longoption) const {
    string option = normalize_option(longoption);
    
    try {
        auto & val = opts.at(option);
        return val;
    } catch (...) {
        return std::nullopt; // flag inválida
    }
}

vector<string> Argparse::get_multioption(char shortoption) const {
    string op;
    op += shortoption;

    return get_multioption(op);    
}

/*#include <iostream>

using std::cout;
using std::endl;
*/
vector<string> Argparse::get_multioption(const string & longoption) const {
    string op = normalize_option(longoption);

    try {
        //cout << op << ": " << multiopts.size() << endl;
        //for (auto s: multiopts.at(op)) cout << s << " ";
        //cout << endl;
        return multiopts.at(op);
    }catch (...) {
        vector<string> v;
        
        return v;
    }
}

optional<string> Argparse::operator [](const string& longoption) const {
    return get_option(longoption);
}

optional<string> Argparse::operator [](char shortoption) const {
    return get_option(shortoption);
}

int Argparse::parse(char* argv[]) {
    int n = 1, cnt = 0;
    int state = 1;
    string op;
    
    while (argv[n]) {
        switch (state) {
            case 1: { // espera uma opção
                op = argv[n];
                if (op[0] != '-') { // final das opções ???
                    int k = n;
                    for (; argv[k] != NULL; k++) {
                        extra += argv[k];
                        extra += ' ';
                    }
                    return n;
                }
                auto jt = flags.find(op);
                if (jt != flags.end()) { // encontrou uma flag
                    jt->second = true;
                } else {
                    auto it = opts.find(op);
                    if (it == opts.end()) {
                        if (multiopts.count(op) == 0) throw std::invalid_argument(op+": opçao inválida"); // opção inválida
                    }
                    state = 2;
                }
                break;
            }
            case 2: { // espera um argumento da opção anterior
                string val = argv[n];
                if (val[0] == '-') throw std::invalid_argument(val+": argumento não pode iniciar com hífen"); // argumento não pode iniciar com hifen
                auto p = opts.find(op);
                if (p == opts.end()) {
                    auto & v = multiopts[op];
                    v.push_back(val);
                    int i = v.size();
                    i = i+1;
                } else p->second = val;
                //opts[op] = val;
                state = 1;
                cnt++;
            }
        }
        n++;
    }
    return cnt;
}



