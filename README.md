# Introdução

O processamento dos argumentos de linha de comando é necessário em diferentes programas, e boa parte dele pode ser generalizada. De fato, existem bibliotecas que oferecem recursos para realizar o processamento dos argumentos:
* [getopt](http://manpages.ubuntu.com/manpages/xenial/man3/getopt.3.html): função da biblioteca C padrão
* [Biblioteca Program_options](http://www.boost.org/doc/libs/1_55_0/doc/html/program_options.html): biblioteca composta por classes e funções (parte da suite C++ boost)
  * [Um tutorial sobre como usar o processamento de argumentos com a biblioteca boost](https://chuckaknight.wordpress.com/2013/03/24/boost-command-line-argument-processing/)
  * [Uma comparação entre getopt e boost (com exemplos)](https://coderwall.com/p/y3xnxg/using-getopt-vs-boost-in-c-to-handle-arguments)


Ambas opções acima são bastante utilizadas e completas. No entanto, por se pretenderem genéricas, apresentam alguns detalhes a serem bem compreendidos para que possam ser utilizadas (veja os manuais). Uma alternativa é Argparse, uma pequena classe cujos objetos processam os argumentos de forma simplificada.

## Argparse

A classe Argparse é capaz de processar argumentos com opções curtas e longas, com ou sem valores complementares. Algumas definições podem esclarecer isso:
* **Opção curta**: informada com uma única letra e precedida por um hífen. Ex: -f
* **Opção longa**: informada com duas ou mais letras e precedida por dois hífens. Ex: --tipo
* **Opção com valor complementar**: a opção pede um valor. Ex: -p /home, --tipo arquivo
* **Opção sem valor complementar**: nenhum valor deve ser acrescentado à opção. Ex: -b, --path.

Valores complementares de opções são sempre do tipo string. Opções sem valor complementar, chamadas de flag, possuem valor bool (true se opção aparece na linha de comando, false caso contrário).


A classe Argparse possui esta interface (OBS: requer C++17 ou superior):

```c++
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
};
```

O programa a seguir ilustra o uso de Argparse, supondo um programa que tenha as opções sem valor complementar -L, -P, e as opções com valor complementar -e, -p, --type.

```c++
#include <cstdlib>
#include <iostream>
#include "Argparse.h"

/*
 * 
 */
void ajuda(const std::string & progname) {
    std::cout << "Uso: " << progname << "-h | [-e N][-L][-P][-p][--type TIPO] [extra...]" << std::endl;
    std::cout << "-h: mostra esta ajuda" << std::endl;
    std::cout << "-e N: define extensão (N = número)" << std::endl;
    std::cout << "-L: ativa largura" << std::endl;
    std::cout << "-P: ativa profundidade" << std::endl;
    std::cout << "-p path: define pathname" << std::endl;
    std::cout << "--type: tipo do arquivo" << std::endl;
}

int main(int argc, char** argv) {
    Argparse args;
      
    /* Demonstração do analisador de argumentos de linha de comando
     
     As opções contidas nessa demonstração são:
     
     -e EXTENSÃO 
     --type TIPO
     -p CAMINHO
     -L
     -P
     * 
     * Assim, o programa pode ser executado com qualquer combinação dessas opções,
     * as quais podem aparecer em qualquer ordem na linha de comando.
     */
    args.add_flag('h');
    args.add_option('e');
    args.add_flag('L');
    args.add_flag('P', true);
    args.add_option('p');
    args.add_option("type", "file");
    
    int n = args.parse(argv);
    
    if (args.get_flag('h')) { 
        ajuda(argv[0]);
        return 0;
    }
    std::cout << "Identificou " << n << " opções" << std::endl;
    std::cout << "ext=" << args['e'].value_or("não fornecido") << std::endl;
    std::cout << "alg: largura=" << args.get_flag('L');
    std::cout << ", profundidade=" << args.get_flag('P') << std::endl;    
    std::cout << "path=" << args['p'].value_or("não fornecido") << std::endl;
    std::cout << "tipo=" << args["type"].value_or("não fornecido") << std::endl;
    std::cout << "extra=" << args.get_extra() << std::endl;
          
    return 0;
}
```
