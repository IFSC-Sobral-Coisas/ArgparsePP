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



