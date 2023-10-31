#include <dirent.h>
#include <cstdlib>
#include <string>
#include <fstream>

#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {

	std::string nomeDir = argv[1];
	DIR *dir = 0;
    struct dirent *entrada = 0;
    unsigned char isFile = 0x8;

    dir = opendir (nomeDir.c_str());

    if (dir == 0) {
        std::cerr << "Nao foi possível abrir diretorio com as instâncias." << std::endl;
        exit (1);
    }

    nomeDir+="solucoes/";
    std::cout << nomeDir;
	dir = opendir (nomeDir.c_str());
	// Gerando o acumulado
	std::ifstream file;
	std::ofstream fileR;
	std::string dado, dado2, dado3, dado4, dado5, dado6, dado7, dado8, dado9, dado10, dado11,dado12, dado13, dado14;
	fileR.open(nomeDir+"BRKGA_RESUMO.txt");
	while ((entrada = readdir (dir))){
        if (entrada->d_type == isFile){
			 std::string nomeArq = entrada->d_name;
			 if (nomeArq.compare("BRKGA_RESUMO.txt")!=0){
				 file.open(nomeDir+entrada->d_name, std::ifstream::in);
				 file >> dado;
				 file >> dado2;
         file >> dado3;
         file >> dado4;
         file >> dado5;
         file >> dado6;
         file >> dado7;
         file >> dado8;
         file >> dado9;
         file >> dado10;
         file >> dado11;
         file >> dado12;
         file >> dado13;
		 file >> dado14;
				 fileR << entrada->d_name << " " << dado << " " << dado2 << " " << dado3 << " " << dado4 << " " << dado5 << " " << dado6 << " " << dado7 << " " << dado8 << " " << dado9 << " " << dado10 <<  " " << dado11 <<  " " << dado12 << " " << dado13 <<  " " << dado14 << std::endl;
				 file.close();
			 }
		}
	}
	fileR.close();
	closedir(dir);
	return 0;
}
