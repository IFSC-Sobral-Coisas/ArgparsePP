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
    Argparse args("Demo");
      
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
    args.add_option<string>("f", "Nome do arquivo");
    args.add_option("num", "Linhas iniciais a apresentar", 10);
    args.add_multioption<string>("out", "Saída onde apresentar o resultado");

    for (auto j=1; argv[j] != NULL; j++) {
        std::cout << j << ": " << argv[j] << std::endl;
    }
    args.parse(argv);

    if (args.get_flag("h")) {
        auto help = args.help();
        std::cout << help << std::endl;
    }
    if (auto n = args.get_option<int>("num")) {
        std::cout << n.value() << " linhas" << std::endl;
    }
    if (auto n = args.get_option<string>("f")) {
        std::cout << n.value() << " arquivo" << std::endl;
    }
    if (auto n = args.get_multioption<string>("out")) {
        std::cout << "Out: ";
        for (auto &out: n.value()) {
            std::cout << out << ',';
        }
        std::cout << std::endl;
    }

    auto help = args.help();
    std::cout << help << std::endl;

    return 0;
}
