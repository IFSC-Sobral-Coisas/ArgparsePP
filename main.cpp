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
    args.add_flag("h", "Mostra uma ajuda");
    args.add_option("f", "Nome do arquivo");
    args.add_option("num", "Linhas iniciais a apresentar", 10);
    args.add_multioption<string>("out", "Saída onde apresentar o resultado");

    args.parse(argv);

    if (args.has_value("num")) {
        auto n = args.get_option<int>("num");
    } else {
        auto help = args.ajuda();
        std::cout << help << std::endl;
    }
    auto arq = args.get_multioption<string>("out");

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
