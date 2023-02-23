#include<stdio.h>
#include<string.h>
#include<stdlib.h>

typedef struct node{
	char ma[10];
	int diem;
	struct node *left;
	struct node *right;
}NODE;

typedef NODE* TREE;

void initTree(TREE *t){
	*t = NULL;
}

void addNodeToTree(TREE *t, char ma[10]){
	if (*t == NULL){
		TREE p = (NODE*)malloc(sizeof(NODE));
		if (p == NULL){
			printf("Bo nho co van de");
			exit(-1);
		}
		p->diem = 0;
		strcpy(p->ma,ma);
		p->left = NULL;
		p->right = NULL;
		*t = p;
		printf("Dia chi nut them vao: %p\n",t);
	}
	else {
		if (strcmp((*t)->ma,ma) > 0)
			addNodeToTree(&(*t)->left,ma);
		else if (strcmp((*t)->ma,ma) < 0)
			addNodeToTree(&(*t)->right,ma);
	}
}

TREE searchInTree(TREE t, char ma[10]){
	if (t == NULL || strcmp(ma,t->ma) == 0) return t;
	else{
		if (strcmp(ma,t->ma) > 0) 
			return searchInTree(t->right,ma);
		if (strcmp(ma,t->ma) < 0)
			return searchInTree(t->left,ma);
	}
}

int compareGoal(int goal1, int goal2){
	if (goal1 > goal2) return 1;
	else if (goal1 < goal2) return -1;
	else return 0;
}

TREE readFile(FILE *fp){
	TREE t;
	initTree(&t);
	fp = fopen("bongda.txt","r");
	if (fp == NULL){
		printf("Khong doc duoc file!\n");
		exit(-1);
	}
	while(!feof(fp)){
		char ma1[100];
		char ma2[100];
		int goal1;
		int goal2;
		fscanf(fp,"%s\t%s\t%d\t%d\n",ma1,ma2,&goal1,&goal2);
		printf("%s %s %d %d\n",ma1,ma2,goal1,goal2);
		int k = compareGoal(goal1,goal2);
		//printf("%d",k);
		NODE *p = searchInTree(t,ma1); 
	
		if (p == NULL){
			addNodeToTree(&t,ma1);
			p = searchInTree(t,ma1); 
		}
		NODE *q = searchInTree(t,ma2);
		if (q == NULL){
			addNodeToTree(&t,ma2);
			q = searchInTree(t,ma2);
	}
	if (k == 1) p->diem = p->diem + 3;
		else if (k == -1) q->diem = q->diem + 3;
		else {
			q->diem = q->diem +1;
			p->diem = p->diem +1;
		}
		//printf("%s %d\n%s %d\n",p->ma,p->diem,q->ma,q->diem);
	}
	fclose(fp);
	return t;
}

void printTree(TREE t){
	if (t== NULL) return;
	printTree(t->left);
	printf("%s %d\n",t->ma,t->diem);
	printTree(t->right);
}

TREE mostLeftChild(TREE t){
	TREE temp = t;
	while (temp && temp->left != NULL)
		temp = temp->left;
	return temp;
}

TREE deleteNode(TREE t, char ma[10]){
	if (t == NULL) return t;
	else if (strcmp(t->ma,ma) < 0)
		t->right = deleteNode(t->right,ma);
	else if (strcmp(t->ma,ma) > 0)
		t->left = deleteNode(t->left,ma);
	else {
		if (t->left == NULL){
			TREE temp = t->right;
			free(t);
			return temp;
		}
		else if (t->right == NULL){
			TREE temp = t->left;
			free(t);
			return temp;
		}
		TREE temp = mostLeftChild(t->right);
		strcpy(t->ma,temp->ma);
		t->diem = temp->diem;
		t->right =  deleteNode(t->right,temp->ma);
	}
	return t;
}

TREE duyetXuongHang(TREE t, int xh){
	TREE temp = t;
	if (temp == NULL) return NULL;
	if (temp->diem <= xh){
		char s[10];
		strcpy(s,temp->ma);
		temp = deleteNode(temp,s);
		printf("Doi %s da xuong hang\n", s);
	}
	temp->left = duyetXuongHang(temp->left,xh);
	temp->right = duyetXuongHang(temp->right,xh);
	return temp;
}

void writeFile(TREE t,FILE *fp){
	if (t!=NULL){
		writeFile(t->left,fp);
		fprintf(fp,"%s\t%d\n",t->ma, t->diem);
		writeFile(t->right,fp);
	}
}

int main(){
	TREE t;
	initTree(&t);
	FILE *fp;
	int luachon;
	while(1){
		printf("=========MENU==========\n1.Tao cay\n2.Hien thi ket qua\n3.Tim kiem\n4.Xuong hang\n5.Xuat file\n");
		printf("\nNhap lua chon: ");
		scanf("%d",&luachon);
		if (luachon < 0 || luachon > 5){
			printf("\nLua chon khong hop le. Xin kiem tra lai !\n");
			system("pause"); // dung màn hình
		} else if (luachon == 1){
				t = readFile(fp);
				system("pause");
			}
			else if (luachon == 2){
				printTree(t);
				system("pause");
			} else if (luachon == 3){
				char ma[10];
				printf("Nhap ma doi: ");fflush(stdin);gets(ma);
				TREE p = searchInTree(t,ma);
				if (p == NULL){
					printf("Khong co ma doi nay!\n");
				}
				printf("%s\t%d\n",p->ma,p->diem);
				system("pause");
			} else if (luachon == 4){
				int xh;
				printf("Nhap so diem xuong hang: ");scanf("%d",&xh);
				t = duyetXuongHang(t,xh);
				printTree(t);
			}else if (luachon == 5){
				FILE *fout;
				fout = fopen("ketqua.txt","w+");
				writeFile(t,fout);
				fclose(fout);
				break;
			}
		}
		return 0;
}
