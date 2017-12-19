#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#define tam 1000 

int main(){
	char c[10]; //vetor de char para pegarmos a informação "P1" do cabeçalho da imagem
	int y, x, status, status2; // variaveis inteiras
	int a[tam][tam],b[tam][tam]; // matrizes usadas para pegar as informações dos pixels das imagens
	FILE *fp, *file, *file2; //ponteiros para FILE para nos referirmos a imagem
	fp = fopen("washington.ascii.pbm", "r"); // abrir imagem
	fscanf(fp, "%s", c) ; //pegar informação "P1"
    fscanf(fp, "%d %d", &y, &x); // pega informação do tamanho da imagem
    // pegar os pixels da imagem original
    for(int i = 0;i < x;i++){
		for(int j = 0;j < y;j++){
			fscanf(fp, "%d", &a[i][j]);
		}
	}
	fclose(fp); // fechar imagem original
	pid_t pid = fork(); // criar um filho
	if(pid < 0){
		printf("error\n");
		exit(1);
	}
	// processo filho (filtro vertical)
	else if(pid == 0){
		printf("eu sou o processo filho (filtro vertical): %d\n", getpid());
		file = fopen("filtroVertical.ascii.pbm", "w"); //cria uma nova imagem
		fprintf(file, "%s\n", c); //imprimi a informação "P1"
		fprintf(file, "%d %d\n", y, x); //imprimi o tamanho da imagem
		// aplica o filtro
		for(int i = 0;i < x;i++){
			for(int j = 0;j < y;j++){
				// filtro (vertical)
				b[i][j]=(a[i-1][j-1]+a[i][j-1]+a[i+1][j-1]-(a[i-1][j+1]+a[i][j+1]+a[i+1][j+1]));
				if(b[i][j] != 0)
					fprintf(file, "%d ", 1);
				else
					fprintf(file, "%d ", 0);
			}
		}
	fclose(file);
	exit(0);
	}
	// processo pai
	else{
		pid_t pid2 = fork();
		if(pid2 < 0){
			printf("error\n");
			exit(1);
		}
		// processo filho 2
		else if(pid2 == 0){
			printf("eu sou o processo filho (filtro horizontal): %d\n", getpid());
			file = fopen("filtroHorizontal.ascii.pbm", "w");
			fprintf(file, "%s\n", c);
			fprintf(file, "%d %d\n", y, x);
			for(int i = 0;i < x;i++){
				for(int j = 0;j < y;j++){
					// filtro (horizontal)
					b[i][j]=(a[i-1][j-1]+a[i-1][j]+a[i-1][j+1]-(a[i+1][j-1]+a[i+1][j]+a[i+1][j+1]));
					if(b[i][j] != 0)
						fprintf(file, "%d ", 1);
					else
						fprintf(file, "%d ", 0);
				}
			}
			fclose(file);
			exit(0);
		}
		else{
			int ret1, ret2;
			printf("eu sou o processo pai: %d\n", getpid());
			waitpid(pid, &status, 0);
			waitpid(pid2, &status2, 0);
			file = fopen("filtroVertical.ascii.pbm", "r");
			file2 = fopen("filtroHorizontal.ascii.pbm", "r");
			fp = fopen("resultado.ascii.pbm", "w");
			fscanf(file, "%s", c);
    		fscanf(file, "%d %d", &y, &x);
    		for(int i = 0;i < x;i++){
				for(int j = 0;j < y;j++){
					fscanf(file, "%d", &a[i][j]);
				}
			}
			fclose(file);
			fscanf(file2, "%s", c);
    		fscanf(file2, "%d %d", &y, &x);
    		for(int i = 0;i < x;i++){
				for(int j = 0;j < y;j++){
					fscanf(file2, "%d", &b[i][j]);
				}
			}
			fclose(file2);
			fprintf(fp, "%s\n", c);
			fprintf(fp, "%d %d\n", y, x);
			for(int i=0; i < x; i++){
		    	for(int j=0;j < y;j++){
		   			if(a[i][j]+b[i][j] != 0)
		   				fprintf(fp, " %d ", 1);
		   			else
		        		fprintf(fp, " %d ", 0);
		   		}
			}
			fclose(fp) ;
		}
	}
	exit(0);
}