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

using std::optional;
using std::string;
using std::vector;
using std::map;

class Argparse {
public:
    Argparse();
    Argparse(const Argparse& orig);
    virtual ~Argparse();

    // Métodos para adicionar uma opção do tipo flag. Tal tipo de opção
    // não requer um valor a ser informado na linha de comando.
    // Ela é do tipo bool: se aparece na linha de comando seu valor é true,    
    // e, caso não apareça, seu valor é false.
    void add_flag(char shortoption);
    void add_flag(const string & longoption);
    
    // Métodos para adicionar uma opção do tipo flag com um valor
    // predefinido
    void add_flag(char shortoption, bool defval);
    void add_flag(const string & longoption, bool defval);
    
    
    // Métodos para adicionar uma opção comum. Tal tipo de opção
    // requer um valor a ser informado na linha de comando.
    // Ela é do tipo string: se aparece na linha de comando, a string que a sucede
    // se torna seu valor. Caso essa opção não apareça, seu valor é uma string vazia.
    // se "multi" for true, a opçoã pode ser usada múltiplas vezes. 
    void add_option(char shortoption);
    void add_option(const string & longoption);
    void add_multioption(const string & longoption);
    void add_multioption(char shortoption);
    
    // Métodos para adicionar uma opção comum com um valor
    // predefinido    
    void add_option(char shortoption, const string & defval);
    void add_option(const string & longoption, const string & defval);
    
    // Métodos para obter o valor de uma opção comum
    optional<string> get_option(char shortoption) const;
    optional<string> get_option(const string & longoption) const;
    optional<string> operator[](const string & longoption) const;
    optional<string> operator[](char shortoption) const;
    vector<string> get_multioption(char shortoption) const;
    vector<string> get_multioption(const string & longoption) const;
    
    // Métodos para obter o valor de uma opção do tipo flag
    bool get_flag(char shortoption) const ;
    bool get_flag(const string & longoption) const;
    
    // Método para obter os argumentos que não foram processados como
    // opções.
    string get_extra() const { return extra;}
    
    // Método para analisar os argumentos de linha de comando e extrair
    // as opções e seus valores contidos nesses argumentos. O vetor argv
    // corresponde ao parâmetro argv da função main. A análise das opções
    // é efetuada a partir da segunda posição do vetor (argv[1]).
    int parse(char * argv[]);
private:
    map<string,optional<string>> opts;
    map<string,vector<string>> multiopts;
    map<string,bool> flags;
    string extra; // resto da linha de comando ...
    
    string normalize_option(const string& longoption) const;
    void set_option(const string & longoption, const optional<string> & val);
};

#endif /* ARGPARSE_H */

