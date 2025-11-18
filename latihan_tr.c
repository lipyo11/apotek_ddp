#include<stdio.h>
#include<string.h>
#include<stdlib.h>

struct mahasiswa{
	int nim;
	char nama[100];
};

struct mahasiswa mhs[100];
int total = 0;

void addMhs(int nim, char nama[]){
	mhs[total].nim = nim;
	strcpy(mhs[total].nama,nama);
	total++;
}

void tambahMhs(){
	char nama[100];
	int nim;
	printf("Masukkan nim : ");
	scanf("%d", &nim);
	printf("Masukkan nama :");
	getchar();
	fgets(nama, 100, stdin);
	addMhs(nim,nama);
	
	printf("mahasiswa berhasil ditambahkan!!");
}

void showAll(){
	for(int i = 0; i < total; i++){
		printf("\n%d\n",mhs[i].nim);
		printf("%s\n",mhs[i].nama);
	}
}

int searchIndex(int nim){
	for(int i = 0; i < total; i++){
		if (mhs[i].nim == nim){
			return i;
		}
	}
	return -1;
}

void menu(){
	
}

int main(){
	int nim;
	addMhs(672025111,"pak adi nugroho");
	addMhs(672025112,"nikolas");
	tambahMhs();
	showAll();
	
	printf("\n\nMasukkan nim dicari :");
	scanf("%d", &nim);
	int index = searchIndex(nim);
	printf("\n%d\n",mhs[index].nim);
	printf("%s\n",mhs[index].nama);
	printf("hello dunia");
}