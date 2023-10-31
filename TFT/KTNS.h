#ifndef KTNS_H
#define KTNS_H

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

extern std::vector<int> ferramentasJob;
extern std::vector<std::vector<int>> candidateList;
extern unsigned trocasPorEstagio[130];


long KTNS(const vector<int>processos, int maquina, bool debug=false) {
	extern std::vector<std::vector<int>> matrixFerramentas;
	extern unsigned t; // ferramentas
	extern vector<unsigned> c; // capacidade do magazine
	vector<int> carregadas(t,0);
	int u=0; // ferramentas no magazine
	int prioridades[t][processos.size()];
	int magazine[t][processos.size()];
	for (int i=0;i<processos.size();++i){
		if (ferramentasJob[processos[i]]>c[maquina]){
			trocasPorEstagio[0] = 10000000;
			trocasPorEstagio[1] = 10000000; // coloquei para evitar valores inválidos no TFT
			return 10000000; // inválido
		}
	}
	trocasPorEstagio[0] = c[maquina];
	for(int i=1;i<processos.size();++i){
		trocasPorEstagio[i] = 0;
	}
	if (debug) {
	std::cout << std::endl << "Matriz de Ferramentas no KTNS" << std::endl;
			for (unsigned j = 0; j<t; j++){
				for (unsigned i = 0; i<8; ++i){
					std::cout<<matrixFerramentas[j][i] << " ";
				}
				std::cout<<std::endl;
			}
			std::cout << " --------------------- " <<std::endl;
	std::cout << "Processos" << std::endl;
	for (unsigned i =0; i<processos.size(); i++) {
		std::cout<<processos[i] << " ";
	}
	std::cout << endl;
	std::cout << endl;
	}

	for (unsigned j=0; j<t; j++) {
		carregadas[j]=matrixFerramentas[j][processos[0]];
		if (matrixFerramentas[j][processos[0]]==1)
			++u;

		for (unsigned i =0; i<processos.size(); i++) {
				magazine[j][i] = matrixFerramentas[j][processos[i]];
				if (debug) {
					cout << magazine[j][i] << " ";
				}
			}
			if (debug) {
			 cout << endl;
			}
	}
	// Preenche a matriz de prioridades
	for (unsigned i=0; i<t; ++i){
		for (unsigned j=0; j < processos.size(); ++j){
			if (magazine[i][j]==1)
				prioridades[i][j] = 0;
			else {
				int proxima = 0;
				bool usa = false;
				for (unsigned k=j+1;k<processos.size();++k){
					++proxima;
					if (magazine[i][k]==1){
						usa = true;
						break;
					}
				}
				if (usa)
					prioridades[i][j]=proxima;
				else
					prioridades[i][j]=-1;
			}
		}
	}
	if (debug) {

	for (unsigned j=0; j<t; j++) {
		for (unsigned i =0; i<processos.size(); i++) {
				cout << prioridades[j][i] << " ";
			}
			cout << endl;
	}

	cout << "Ferramentas carregadas: " << endl;
	for (unsigned j=0; j<t; j++) {
		if (carregadas[j]==33) exit(0);
				cout << carregadas[j] << endl;
	}
	}


	// Calcula as trocas
	if (debug) {
	 cout << u << " carregadas na primeira tarefa" << endl;
	}
	int trocas = 0;
	for (unsigned i=1; i<processos.size(); ++i) {
		for (unsigned j=0; j<t; ++j){
			if ((magazine[j][i]==1) && (carregadas[j]==0)){
				carregadas[j]=1;
				++u;
			}
		}
		if (debug) {
			cout << u << " Ferramentas carregadas" << endl;
		}
		while (u>c[maquina]){
			int maior = 0;
			int pMaior = -1;
			for (unsigned j=0; j<t; ++j) {
				if (magazine[j][i]!=1){ // Ferramenta não utilizada pelo processo atual
					if ((carregadas[j]==1) && (prioridades[j][i] == -1)) { // Essa ferramenta não será mais utilizada e é um excelente candidato a remoção
						pMaior = j;
						break;
					} else {
						if ((prioridades[j][i]>maior) && carregadas[j]==1) {
							maior = prioridades[j][i];
							pMaior = j;
						}
					}
				}
			}
			carregadas[pMaior] = 0;
			if (debug) {
				cout << "Retirou " << i << ":" << pMaior << endl;
			}
			--u;
			trocasPorEstagio[i]++;
			++trocas;
			if (debug) {
				cout << trocas << " trocas " << endl;
			}
		}
		if (debug) {

		cout << "Ferramentas carregadas: " << endl;
		for (unsigned j=0; j<t; j++) {
				cout << carregadas[j] << endl;
		}
	}
	}
	if (debug) {
	 cout << ": " << trocas << "trocas" << endl;
	}
	return trocas;
}
double completionTime(std::vector<std::vector<unsigned>> tProcessamento, std::vector<int >& tarefas, int maquina){
	extern std::vector<unsigned> tempoTroca;
	extern vector<unsigned> c; // capacidade do magazine
	double tPr = 0;
	for (std::vector<int>::const_iterator i = tarefas.begin(); i!=tarefas.end(); ++i){
		if (ferramentasJob[*i]>c[maquina])
			return 1000000; // inválido
		tPr+= tProcessamento[maquina][*i];
	}
	long nTrocas = KTNS(tarefas,maquina);
	double tTrocas = nTrocas*tempoTroca[maquina];
	return (tPr+tTrocas);
}
double sumCompletionTime(std::vector < std::vector <int> >& maquinas, std::vector<std::vector<unsigned>> tProcessamento){
	double tot = 0;
	int maq = 0;
	std::vector<int> maquinaAux;
	for(std::vector<std::vector<int>>::const_iterator i = maquinas.begin(); i!=maquinas.end(); ++i){
		maquinaAux = *i;
		tot += completionTime(tProcessamento, maquinaAux, maq);
		maq++;
	}
	return tot;
}


long total_trocas(std::vector<double> chromosome){
	long totTrocas = 0;
	extern int m;
	std::vector<std::vector<int>> maquinas;			  // Todas as máquinas
	std::vector<std::pair<double, int>> idx_maquinas; // completionTime, machine
	int maquina = 0;
	int at = 0;
	double tPr = 0;
	double makespan = 0.0;
	std::vector<std::pair<double, unsigned>> ranking(chromosome.size());
	for (unsigned i = 0; i < chromosome.size(); ++i)
	{
		ranking[i] = std::pair<double, unsigned>(chromosome[i], i);
	}

	std::sort(ranking.begin(), ranking.end());
	std::vector<int> processos;
	int nTrocas = 0;
	long tTrocas = 0;
	extern std::vector<unsigned> tempoTroca;
	tPr = 0.0;
	for (std::vector<std::pair<double, unsigned>>::const_iterator i = ranking.begin(); i != ranking.end(); ++i)
	{
		at = (int)i->first; // A parte inteira do alelo representa a máquina
		if (maquina == 0)
		{
			maquina = at;
			processos.clear();
		}
		if (maquina == at)
		{
			processos.push_back(i->second);
		}
		else
		{
			nTrocas = KTNS(processos, maquina - 1);
			totTrocas += nTrocas;

			maquinas.push_back(processos);

			maquina = at;
			processos.clear();
			processos.push_back(i->second);
		}
	}
	nTrocas = KTNS(processos, maquina - 1);
	totTrocas+=nTrocas;
	
	return totTrocas;
}

long flowTime(std::vector<double> chromosome, std::vector<std::vector<unsigned>> tProcessamento){
	
	extern std::vector<unsigned> tempoTroca;
	std::vector<long>flowTimes;
	std::vector<std::vector<int>> maquinas;			  // Todas as máquinas
	int maquina = 0;
	int at = 0;
	maquinas.clear();
	long completeFlowTime = 0;
	std::vector<std::pair<double, unsigned>> ranking(chromosome.size());
	for (unsigned i = 0; i < chromosome.size(); ++i)
	{
		ranking[i] = std::pair<double, unsigned>(chromosome[i], i);
	}

	std::sort(ranking.begin(), ranking.end());
	std::vector<int> processos;

	
	for (std::vector<std::pair<double, unsigned>>::const_iterator i = ranking.begin(); i != ranking.end(); ++i)
	{
		at = (int)i->first; // A parte inteira do alelo representa a máquina
		if (maquina == 0)
		{
			maquina = at;
			processos.clear();
		}
		if (maquina == at)
		{
			processos.push_back(i->second);
		}
		else
		{
			maquinas.push_back(processos);

			maquina = at;
			processos.clear();
			processos.push_back(i->second);
		}
	}
	maquinas.push_back(processos);

	flowTimes.clear();
	maquina = 0;
	for (std::vector<std::vector<int>>::const_iterator i = maquinas.begin(); i != maquinas.end(); ++i)
	{
		std::vector<int> maquinaAux = *i;
		KTNS(maquinaAux,maquina);
		flowTimes.push_back(tProcessamento[maquina][maquinaAux[0]]);
		for (unsigned j = 1; j < maquinaAux.size(); ++j)
		{
			flowTimes.push_back(trocasPorEstagio[j]*tempoTroca[maquina] + tProcessamento[maquina][maquinaAux[j]] + flowTimes[j-1]);
		}
		long flwt = 0;
		for (unsigned j=0;j<flowTimes.size();j++)
			flwt+=flowTimes[j];
		completeFlowTime += flwt;
		maquina++;
		flowTimes.clear();
	}


	return completeFlowTime;
}


#endif