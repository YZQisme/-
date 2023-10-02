#define _CRT_SECURE_NO_WARNINGS 1
#pragma warning( disable : 4789)
#define NAME_SIZE 32
#define PHONENUMBER_SIZE 22
#define ADRESS_SIZE 82
#define EMAIL_SIZE 32
#define LABEL_SIZE 22
#define KEYWORD_SIZE 12
#define TRUE 1
#define FALSE 0
#define LIST_MAXLENGTH 50
#define OPENFILE_FAILURE -1
#define CONTENT_ERROR -2
#define MEMORYREQUEST_FAILURE -3
#define DATA_ERROR -2
#define SAVE_MEANINGLESS -3
#define DATAFILE "People.txt"
#define PRINT_RED(STR) printf("\033[1;31m" #STR "\n")
#define PRINT_GREEN(STR) printf("\033[1;32m" #STR "\n")
#define PRINT_WHITE(STR) printf("\033[1;37m" #STR "\n")
#define PRINT_YELLOW(STR) printf("\033[1;33m" #STR "\n")
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct People {
	char name[NAME_SIZE];
	char phonenumber[PHONENUMBER_SIZE];
	char adress[ADRESS_SIZE];
	char email[EMAIL_SIZE];
	char label[LABEL_SIZE];
};
struct LIST {
	struct People people;
	struct LIST* next;
};
const int sizeof_People = sizeof(struct People);
const int sizeof_LIST = sizeof(struct LIST);
char* strinput(char* str, int size, const char* guidance) {
	char ch;
	int i = 0;
	printf("\033[1;33m");
	printf(guidance);
	while ((ch = getchar()) != '\n') {
		if (i < size - 2) {
			str[i++] = ch;
		}
	}
	if (i != 0) {
		str[i] = '\0';
		str[size - 1] = '\n';
		return str;
	}
	return NULL;
}
void interface_selectlabel(void) {
	
}//���������Ҫ����
char* selectlabel(char* str) {
	char selection[3];
	int a;
	char string[7][6] = { "����", "֪��", "����","����","ͬ��","�쵼","ż��" };
	interface_selectlabel();
	while (strinput(selection, 3, "��ѡ��") == NULL) { PRINT_RED(����Ϊ��); }
	switch (*selection) {
	case 'A':
	case 'a':
		a = 0;
		break;
	case 'B':
	case 'b':
		a = 1;
		break;
	case 'C':
	case 'c':
		a = 2;
		break;
	case 'D':
	case 'd':
		a = 3;
		break;
	case 'E':
	case 'e':
		a = 4;
		break;
	case 'F':
	case 'f':
		a = 5;
		break;
	case 'G':
	case 'g':
		a = 6;
		break;
	default:
		return (strinput(str, LABEL_SIZE, "�������ע��"));
	}
	for (int i = 0; i < 4; i++) { str[i] = string[a][i]; }
	str[4] = '\0';
	str[LABEL_SIZE - 1] = '\n';
	return str;
}
struct LIST* People_load(const char* Filename,const struct LIST* list_head,int* error) {
	FILE* datafile;
	struct LIST* current=NULL;
	struct LIST*  previous=list_head;
	if ((datafile = fopen(Filename, "rb"))==NULL) {
		*error=OPENFILE_FAILURE;
		return NULL;
	}
	for (int i = 0; i < LIST_MAXLENGTH+1;i++) {
		if ((current=(struct LIST*)malloc(sizeof_LIST))!=NULL) {
			fseek(datafile, i * sizeof_People, SEEK_SET);
			if (fread(&(current->people), sizeof_People, 1, datafile) ==1) {
				previous->next = current;
				current->next = NULL;
				previous = current;
			}
			else {
				free(current);
				if (!feof(datafile)) {
					*error = CONTENT_ERROR;
					fclose(datafile);
					return NULL;
				}
				break;
			}
		}
		else {
			*error=MEMORYREQUEST_FAILURE;
			fclose(datafile);
			return NULL;
		}
	}
	fclose(datafile);
	return list_head;
}
void People_unload(struct LIST* list_head) {
	struct LIST* current;
	struct LIST* previous;
	if ((current = list_head->next) != NULL) {
		while (current->next != NULL) {
			previous = current;
			current = current->next;
			free(previous);
		}
		free(current);
	}
}
int People_save(const struct LIST* node_first,const char* Filename) {
	int number = 0;
	int result = 0;
	struct LIST* node_current, * node_previous;
	FILE* file;
	if ((file = fopen(Filename, "wb")) == NULL) { return OPENFILE_FAILURE; }
	if ((node_current = node_first->next) != NULL) {
		do {
			fseek(file, sizeof_People * number++, SEEK_SET);
			if (fwrite(&(node_current->people), sizeof_People, 1, file) != 1) { 
				result= DATA_ERROR;
				break;
			}
			node_previous = node_current;
			node_current = node_previous->next;
		} while (node_current && number < LIST_MAXLENGTH);
	}
	else {
		result = SAVE_MEANINGLESS;
	}
	fclose(file);
	return result;
}
void People_del(struct LIST* node_aim,struct LIST* node_previous) {
	struct LIST* result;
	node_previous->next = node_aim->next;
	free(node_aim);
}
struct LIST* People_add(struct LIST* lastnode) {
	struct LIST* add;
	if ((add = (struct LIST*)malloc(sizeof_LIST)) != NULL) {
		while (strinput(add->people.name, NAME_SIZE, "���������֣�") == NULL) { PRINT_RED(����Ϊ��);}
		while (strinput(add->people.phonenumber, PHONENUMBER_SIZE, "������绰���룺") == NULL) { PRINT_RED(����Ϊ��); }
		while (strinput(add->people.adress, ADRESS_SIZE, "�������ס��ַ��") == NULL) { PRINT_RED(����Ϊ��); }
		while (strinput(add->people.email, EMAIL_SIZE, "����������ʼ���") == NULL) { PRINT_RED(����Ϊ��); }
		while (selectlabel(add->people.label)==NULL){ PRINT_RED(����Ϊ��); }
		add->next = NULL;
		lastnode->next = add;
	}
	else {
		return NULL; 
	}
}
struct LIST* People_change(struct LIST* node) {
	while (strinput(node->people.name, NAME_SIZE, "�������µ�������") == NULL) { PRINT_RED(����Ϊ��); }
	while (strinput(node->people.phonenumber, PHONENUMBER_SIZE, "�������µĵ绰���룺") == NULL) { PRINT_RED(����Ϊ��); }
	while (strinput(node->people.adress,ADRESS_SIZE, "�������µľ�ס��ַ��") == NULL) { PRINT_RED(����Ϊ��); }
	while (strinput(node->people.email,EMAIL_SIZE, "�������µĵ����ʼ���") == NULL) { PRINT_RED(����Ϊ��); }
	while (selectlabel(node->people.label) == NULL) { PRINT_RED(����Ϊ��); }
}
void People_display(const struct People* people) {
	printf("\033[1;32m");
	printf("%10s  %s\n","����", people->name);
	printf("%10s  %s\n", "�绰����", people->phonenumber);
	printf("%10s  %s\n", "��ס��ַ", people->adress);
	printf("%10s  %s\n", "�����ʼ�", people->email);
	printf("%10s  %s\n", "��ע", people->label);
	printf("\n");
}
_Bool struct_contain_str(const struct People* people, const char* string) {
	if ((strstr(people->name, string) != NULL)) { return TRUE; }
	if ((strstr(people->phonenumber, string) != NULL)) { return TRUE; }
	if ((strstr(people->adress, string) != NULL)) { return TRUE; }
	if ((strstr(people->email, string) != NULL)) { return TRUE; }
	if ((strstr(people->label, string) != NULL)) { return TRUE; }
	return FALSE;
}

void interface_home(void) {
	printf("\033[1;33m");
	printf(
		"��ӭ����ͨѶ¼\n"
		"A �г���ϵ��\n"
		"B ������ϵ��\n"
		"C �����ϵ��\n"
		"D �޸���ϵ��\n"
		"E ɾ����ϵ��\n"
		"F �˳�\n"
	);
}
void interface_search(void) {}
void interface_change(void) {}
void interface_del(void) {}

int main() {
	//����������ǰPeople.txt������ڣ�����ᱨ��
	struct LIST node_first = { {""""""""""},NULL };
	int load_error = 0;
	char home_selection[3];
	_Bool exit = FALSE;
	struct LIST* node_previous, * node_current, * newPeople;
	_Bool search_found = FALSE;
	char search_keyword[KEYWORD_SIZE];
	char People_del_name[NAME_SIZE];
	char People_change_name[NAME_SIZE];
	char isPeoplechange[3];
	char isPeopledel[3];

	if (!People_load(DATAFILE, &node_first, &load_error)) {
		switch (load_error) {
		case OPENFILE_FAILURE:
			PRINT_RED(���ļ�ʧ��);
			break;
		case CONTENT_ERROR:
			PRINT_RED(�ļ����ݴ���);
			break;
		case MEMORYREQUEST_FAILURE:
			PRINT_RED(�ڴ�����ʧ��);
			break;
		}
		People_unload(&node_first);
		return 0;
	}

	while (exit == FALSE) {

		node_current = NULL;
		node_previous = NULL;
		search_found = FALSE;
		home_selection[3] = "";
		search_keyword[KEYWORD_SIZE] = "";
		People_del_name[NAME_SIZE] = "";
		People_change_name[NAME_SIZE] = "";
		isPeoplechange[3] = "";
		isPeopledel[3] = "";

		interface_home();
		while (!strinput(home_selection, 3, "��ѡ�������")) { PRINT_RED(����Ϊ��); };
		system("cls");
		switch (home_selection[0]) {
		case'A':
		case'a':
			//�г���ϵ��
			node_current = node_first.next;
			node_previous = &node_first;
			while (node_current) {
				People_display(&(node_current->people));
				search_found = TRUE;
				node_previous = node_current;
				node_current = node_previous->next;
			}
			if (!search_found) { PRINT_WHITE(û����ϵ��); }
			break;
		case'B':
		case'b':
			//������ϵ��
			while (strinput(search_keyword, KEYWORD_SIZE, "������򵥵Ĺؼ��ʣ�") == NULL) { PRINT_RED(����Ϊ��); }
			node_current = node_first.next;
			node_previous = &node_first;
			while (node_current) {
				if (struct_contain_str(&(node_current->people), search_keyword)) {
					People_display(&(node_current->people));
					search_found = TRUE;
				}
				node_previous = node_current;
				node_current = node_previous->next;
			}
			if (!search_found) { PRINT_WHITE(�Ҳ��������ؼ��ʵ���ϵ��); }
			break;
		case'C':
		case'c':
			//�����ϵ��
			node_current = node_first.next;
			node_previous = &node_first;
			while (node_current) {
				node_previous = node_current;
				node_current = node_previous->next;
			}
			if (People_add(node_previous)) {
				PRINT_GREEN(������µ���ϵ��);
			}
			else {
				PRINT_RED(�޷����);
			}
			break;
		case'D':
		case'd':
			//�޸���ϵ��
			while (strinput(People_change_name, NAME_SIZE, "������Ŀ����ϵ�˵����֣�") == NULL) { PRINT_RED(����Ϊ��); }
			node_current = node_first.next;
			node_previous = &node_first;
			while (node_current) {
				if (strstr(node_current->people.name, People_del_name)) {
					People_display(&(node_current->people));
					search_found = TRUE;
					break;
				}
				node_previous = node_current;
				node_current = node_previous->next;
			}
			if (search_found) {
				PRINT_RED(�Ƿ��޸�������ϵ�˵���Ϣ);
				strinput(isPeoplechange, 3, "[Y]��    [N]��");
				if (*isPeoplechange == 'Y' || *isPeoplechange == 'y') { People_change(node_current); }
			}
			else {
				PRINT_WHITE(�Ҳ�����ϵ��);
			}
			break;
		case'E':
		case'e':
			//ɾ����ϵ��
			while (strinput(People_del_name, NAME_SIZE, "������Ŀ����ϵ�˵����֣�") == NULL) { PRINT_RED(����Ϊ��); }
			node_current = node_first.next;
			node_previous = &node_first;
			while (node_current) {
				if (strstr(node_current->people.name, People_del_name)) {
					People_display(&(node_current->people));
					search_found = TRUE;
					break;
				}
				node_previous = node_current;
				node_current = node_previous->next;
			}
			if (search_found) {
				PRINT_RED(�Ƿ�ɾ��������ϵ�� ? );
				strinput(isPeopledel, 3, "[Y]��    [N]��");
				if (*isPeopledel == 'Y' || *isPeopledel == 'y') { People_del(node_current, node_previous); }
			}
			else {
				PRINT_WHITE(�Ҳ�����ϵ��);
			}
			break;
		default:
			//�˳� f
			exit = TRUE;
			break;
		}
		system("pause");
		system("cls");
	}

	switch (People_save(&node_first, DATAFILE)) {
		case DATA_ERROR:
			PRINT_RED(���ݴ���);
			break;
		case OPENFILE_FAILURE:
			PRINT_RED(�ļ��޷���);
			break;
		default:
			PRINT_GREEN(�ѱ���);
			break;
	}

	People_unload(&node_first);
	return 0;
}