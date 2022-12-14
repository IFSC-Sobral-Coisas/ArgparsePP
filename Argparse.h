/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Argparse.h
 * Author: msobral
 *
 * Created on 22 de Agosto de 2017, 16:46
 */

#ifndef ARGPARSE_H
#define ARGPARSE_H

#include <string>
#include <map>
#include <vector>
#include <optional>
#include <stdexcept>
#include <variant>

using std::optional;
using std::string;
using std::vector;
using std::map;
using std::variant;
using std::string_view;

class Argparse {
public:
    Argparse();
    Argparse(string_view titulo);
    Argparse(const Argparse& orig);
    virtual ~Argparse();

    // Métodos para adicionar uma opção do tipo flag. Tal tipo de opção
    // não requer um valor a ser informado na linha de comando.
    // Ela é do tipo bool: se aparece na linha de comando seu valor é true,
    // e, caso não apareça, seu valor é false.
    void add_flag(const string & nome, string_view ajuda);

    // Métodos para adicionar uma opção comum. Tal tipo de opção
    // requer um valor a ser informado na linha de comando.
    // Ela é do tipo string: se aparece na linha de comando, a string que a sucede
    // se torna seu valor. Caso essa opção não apareça, seu valor é uma string vazia.
    // se "multi" for true, a opçoã pode ser usada múltiplas vezes.
    template <typename T> void add_option(string_view nome, string_view ajuda, const T & defval);
    template <typename T> void add_option(string_view nome, string_view ajuda);
    template <typename T> void add_multioption(string_view nome, string_view ajuda);

    // Métodos para obter o valor de uma opção comum
    template <typename T> optional<vector<T>> get_multioption(const string& nome) const;
    template <typename T> optional<T> get_option(const string& nome) const;

    // Métodos para obter o valor de uma opção do tipo flag
    bool get_flag(const string & name) const;

    bool has_value(const string& name) const;

    // Método para obter os argumentos que não foram processados como
    // opções.
    string get_extra() const { return extra;}
    
    // Método para analisar os argumentos de linha de comando e extrair
    // as opções e seus valores contidos nesses argumentos. O vetor argv
    // corresponde ao parâmetro argv da função main. A análise das opções
    // é efetuada a partir da segunda posição do vetor (argv[1]).
    int parse(char * argv[]);
    int parse(string_view args);

    // Gera um texto de ajuda, com base nas opções cadastradas
    string help() const;
private:
    using OptionTypes = variant<optional<int>,optional<float>,optional<string>,optional<bool>>;
    using MultiopTypes = variant<vector<int>,vector<float>,vector<string>>;

    // dict: associa nome da opção a um par (tipo/default, ajuda)
    map<string,std::pair<OptionTypes, string>> opts;
    map<string,std::pair<MultiopTypes, string>> multiopts;
    string extra; // resto da linha de comando ...
    string titulo;

    template <typename T> optional<bool> has_value_of(const string & name) const;
    template <typename T, typename U, typename ...Args> bool has_some_value(const string & name) const;
    template <typename T> bool has_some_value(const string & name) const;
//    bool has_some_value(const string & name) const;
    bool set_option(const string & name, const string & val);
    bool set_multioption(const string & name, const string & val);
    template <typename F> optional<bool> set_option2(const string &name, const string &val, F conv);
    template <typename F> optional<bool> set_multioption2(const string &name, const string &val, F conv);
};

template<typename T>
void Argparse::add_option(string_view nome, string_view ajuda, const T &defval) {
    opts.emplace(nome, std::make_pair(OptionTypes(std::make_optional<T>(defval)), ajuda));
}

template<typename T>
void Argparse::add_option(string_view nome, string_view ajuda) {
    opts.emplace(nome, std::make_pair(OptionTypes(optional<T>{}), ajuda));
}

template<typename T>
void Argparse::add_multioption(string_view nome, string_view ajuda) {
    multiopts.emplace(nome, std::make_pair(MultiopTypes{vector<T>{}}, ajuda));
}

template<typename T>
optional<vector<T>> Argparse::get_multioption(const string& nome) const {
    try {
        auto op = multiopts.at(nome);
        if (auto * p = std::get_if<vector<T>>(&op.first)) {
            return std::make_optional(*p);
        }
    } catch(...) {}

    return std::nullopt;
}

template<typename T>
optional<T> Argparse::get_option(const string& nome) const {
    try {
        auto op = opts.at(nome);
        if (auto * p = std::get_if<optional<T>>(&op.first)) {
            return *p;
        }
    } catch(...) {}

    return std::nullopt;
}

template<typename T>
optional<bool> Argparse::has_value_of(const string &name) const {
    try {
        auto op = opts.at(name);
        if (auto * p = std::get_if<optional<T>>(&op.first)) {
            return std::make_optional(p->has_value());
        }
    } catch(...) {}

    try {
        auto op = multiopts.at(name);
        if (auto * p = std::get_if<vector<T>>(&op.first)) {
            return std::make_optional(! p->empty());
        }
    } catch(...) {}


    return std::nullopt;
}

template <typename T>
bool Argparse::has_some_value(const string & name) const {
    return has_value_of<T>(name).value_or(false);
}

template <typename T, typename U, typename ...Args> bool Argparse::has_some_value(const string & name) const {
    return has_value_of<T>(name).value_or(has_some_value<U,Args...>(name));
//    if (auto op = has_value_of<T>(name)) {
//        return op.value();
//    }
//    return has_some_value<Args...>(name);
}

template <typename F> optional<bool> Argparse::set_option2(const string &name, const string &val, F conv) {
    using T = typename std::result_of<F(const std::string&)>::type;
    try {
        auto & op = opts.at(name);
        if (auto * p = std::get_if<optional<T>>(&op.first)) {
            try {
                auto opval = conv(val);
                op.first = std::make_optional(opval);
                return std::make_optional(true);
            } catch(...) {
                return std::make_optional(false);
            }
        }
    } catch(...) {}
    return std::nullopt;
}

template <typename F> optional<bool> Argparse::set_multioption2(const string &name, const string &val, F conv) {
    using T = typename std::result_of<F(const std::string&)>::type;
    try {
        auto & op = multiopts.at(name);
        if (auto * p = std::get_if<vector<T>>(&op.first)) {
            try {
                p->push_back(conv(val));
                return std::make_optional(true);
            } catch(...) {
                return std::make_optional(false);
            }
        }
    } catch(...) {}
    return std::nullopt;
}


#endif /* ARGPARSE_H */

