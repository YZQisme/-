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
	
}//这个函数需要补充
char* selectlabel(char* str) {
	char selection[3];
	int a;
	char string[7][6] = { "家人", "知己", "亲戚","朋友","同事","领导","偶遇" };
	interface_selectlabel();
	while (strinput(selection, 3, "请选择：") == NULL) { PRINT_RED(不可为空); }
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
		return (strinput(str, LABEL_SIZE, "请输入标注："));
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
		while (strinput(add->people.name, NAME_SIZE, "请输入名字：") == NULL) { PRINT_RED(不可为空);}
		while (strinput(add->people.phonenumber, PHONENUMBER_SIZE, "请输入电话号码：") == NULL) { PRINT_RED(不可为空); }
		while (strinput(add->people.adress, ADRESS_SIZE, "请输入居住地址：") == NULL) { PRINT_RED(不可为空); }
		while (strinput(add->people.email, EMAIL_SIZE, "请输入电子邮件：") == NULL) { PRINT_RED(不可为空); }
		while (selectlabel(add->people.label)==NULL){ PRINT_RED(不可为空); }
		add->next = NULL;
		lastnode->next = add;
	}
	else {
		return NULL; 
	}
}
struct LIST* People_change(struct LIST* node) {
	while (strinput(node->people.name, NAME_SIZE, "请输入新的姓名：") == NULL) { PRINT_RED(不可为空); }
	while (strinput(node->people.phonenumber, PHONENUMBER_SIZE, "请输入新的电话号码：") == NULL) { PRINT_RED(不可为空); }
	while (strinput(node->people.adress,ADRESS_SIZE, "请输入新的居住地址：") == NULL) { PRINT_RED(不可为空); }
	while (strinput(node->people.email,EMAIL_SIZE, "请输入新的电子邮件：") == NULL) { PRINT_RED(不可为空); }
	while (selectlabel(node->people.label) == NULL) { PRINT_RED(不可为空); }
}
void People_display(const struct People* people) {
	printf("\033[1;32m");
	printf("%10s  %s\n","姓名", people->name);
	printf("%10s  %s\n", "电话号码", people->phonenumber);
	printf("%10s  %s\n", "居住地址", people->adress);
	printf("%10s  %s\n", "电子邮件", people->email);
	printf("%10s  %s\n", "标注", people->label);
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
		"欢迎进入通讯录\n"
		"A 列出联系人\n"
		"B 查找联系人\n"
		"C 添加联系人\n"
		"D 修改联系人\n"
		"E 删除联系人\n"
		"F 退出\n"
	);
}
void interface_search(void) {}
void interface_change(void) {}
void interface_del(void) {}

int main() {
	//程序在运行前People.txt必须存在，否则会报错
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
			PRINT_RED(打开文件失败);
			break;
		case CONTENT_ERROR:
			PRINT_RED(文件内容错误);
			break;
		case MEMORYREQUEST_FAILURE:
			PRINT_RED(内存申请失败);
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
		while (!strinput(home_selection, 3, "请选择操作：")) { PRINT_RED(不可为空); };
		system("cls");
		switch (home_selection[0]) {
		case'A':
		case'a':
			//列出联系人
			node_current = node_first.next;
			node_previous = &node_first;
			while (node_current) {
				People_display(&(node_current->people));
				search_found = TRUE;
				node_previous = node_current;
				node_current = node_previous->next;
			}
			if (!search_found) { PRINT_WHITE(没有联系人); }
			break;
		case'B':
		case'b':
			//查找联系人
			while (strinput(search_keyword, KEYWORD_SIZE, "请输入简单的关键词：") == NULL) { PRINT_RED(不可为空); }
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
			if (!search_found) { PRINT_WHITE(找不到包含关键词的联系人); }
			break;
		case'C':
		case'c':
			//添加联系人
			node_current = node_first.next;
			node_previous = &node_first;
			while (node_current) {
				node_previous = node_current;
				node_current = node_previous->next;
			}
			if (People_add(node_previous)) {
				PRINT_GREEN(已添加新的联系人);
			}
			else {
				PRINT_RED(无法添加);
			}
			break;
		case'D':
		case'd':
			//修改联系人
			while (strinput(People_change_name, NAME_SIZE, "请输入目标联系人的名字：") == NULL) { PRINT_RED(不可为空); }
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
				PRINT_RED(是否修改上述联系人的信息);
				strinput(isPeoplechange, 3, "[Y]是    [N]否");
				if (*isPeoplechange == 'Y' || *isPeoplechange == 'y') { People_change(node_current); }
			}
			else {
				PRINT_WHITE(找不到联系人);
			}
			break;
		case'E':
		case'e':
			//删除联系人
			while (strinput(People_del_name, NAME_SIZE, "请输入目标联系人的名字：") == NULL) { PRINT_RED(不可为空); }
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
				PRINT_RED(是否删除上述联系人 ? );
				strinput(isPeopledel, 3, "[Y]是    [N]否");
				if (*isPeopledel == 'Y' || *isPeopledel == 'y') { People_del(node_current, node_previous); }
			}
			else {
				PRINT_WHITE(找不到联系人);
			}
			break;
		default:
			//退出 f
			exit = TRUE;
			break;
		}
		system("pause");
		system("cls");
	}

	switch (People_save(&node_first, DATAFILE)) {
		case DATA_ERROR:
			PRINT_RED(数据错误);
			break;
		case OPENFILE_FAILURE:
			PRINT_RED(文件无法打开);
			break;
		default:
			PRINT_GREEN(已保存);
			break;
	}

	People_unload(&node_first);
	return 0;
}