#include<iostream>
#include<fstream>
#include<cstdlib>
#include<cstring>
#include<cstdio>
using namespace std;

struct elem
{
	char value;
	elem* next = nullptr;
};
void push(elem*& head, char val) {
	elem* newel = new elem;
	newel->value = val;
	newel->next = head;
	head = newel;
}
char* peek(elem* head) {
	if (!head) return nullptr;
	return &head->value;
}
bool pop(elem*& head, char& val) {
	if (!head) return false;
	elem* rem = head;
	val = head->value;
	head = head->next;
	delete rem;
	return true;
}
void clear(elem*& head) {
	while (head) {
		elem* rem = head;
		head = rem->next;
		delete rem;
	}
}
void fill_stack(elem*& head, char*& a)
{
	for (int i = 0; i < strlen(a); i++) {
		if (a[i] == '(' || a[i] == '[' || a[i] == '{' || a[i] == '<')
		{
			push(head, a[i]);
		}
		if (a[i] == ')') {
			char val = *peek(head);
			if (val == '(') {
				pop(head, val);
			}
			else {
				cout << i + 1;
				return;
			}
		}
		if (a[i] == ']') {
			char val = *peek(head);
			if (val == '[') {
				pop(head, val);
			}
			else {
				cout << i + 1;
				return;
			}
		}
		if (a[i] == '}') {
			char val = *peek(head);
			if (val == '{') {
				pop(head, val);
			}
			else {
				cout << i + 1;
				return;
			}
		}
		if (a[i] == '>') {
			char val = *peek(head);
			if (val == '<') {
				pop(head, val);
			}
			else {
				cout << i + 1;
				return;
			}
		}
	}
	if (*peek(head) == '!') {
		cout << "Sequence of brackets is legal";
	}
	else {
		cout << "Sequence of brackets is not legal";
	}
}
void brackets(char* value) {
	elem* head = new elem;
	head->value = '!';//создалась голова, имеет символ !
	fill_stack(head, value);
	clear(head);
}

void str_decode(char*& dec_str, const char* enc_str) {
	char* res = new char[strlen(enc_str) + 1];
	int i = 0, j = 0;
	while (enc_str[i]) {
		if (enc_str[i] == '+') {
			res[j] = ' ';
		}
		else {
			if(enc_str[i] == '%') {
				char ch[3] = { enc_str[i + 1], enc_str[i + 2], 0 };
				int c;
				sscanf_s(ch, "%X", &c);
				res[j] = c;
				i += 2;
			}
			else {
				res[j] = enc_str[i];
			}
		}
		i++;
		j++;
	}
	res[j] = 0;
	size_t len = strlen(res) + 1;
	delete[] dec_str;
	dec_str = new char[len];
	strcpy_s(dec_str, len, res);
	delete[] res;
}
void get_param_value(char*& value, const char* param_name, const char* data) {
	delete[] value;
	value = nullptr;
	value = nullptr;
	char* str = _strdup(data);
	char* tmp = str;
	char* cont;
	while (char* part = strtok_s(tmp, "&", &cont)) {
		tmp = nullptr;
		char* val;
		char* key = strtok_s(part, " = ", &val);
		if (!_strcmpi(param_name, key)) {
			str_decode(value, val);
			//value = _strdup(val);
			delete[] str;
			return;
		}
	}
	delete[] str;
	value = new char[1];
	value[0] = 0;
}
size_t get_content_length() {
	size_t realsize;
	char* value = new char[11];
	getenv_s(&realsize, value, 11, "CONTENT_LENGTH");
	size_t size;
	if (!realsize) size = 0;
	else sscanf_s(value, "%d", &size);
	delete[] value;
	return size;
}
bool is_get() {
	size_t realsize;
	char* value = new char[5];
	getenv_s(&realsize, value, 5, "REQUEST_METHOD");
	bool res = !_strcmpi(value, "GET");
	delete[] value;
	return res;
}
void get_form_data(char*& data) {
	delete[] data;
	if (is_get()) {
		size_t realsize;
		char* value = new char[65536];
		getenv_s(&realsize, value, 65536, "QUERY_STRING");
		data = new char[realsize + 1];
		strcpy_s(data, realsize + 1, value);
		delete[] value;
	}
	else {
		size_t buf_size = get_content_length();
		std::cout << buf_size;
		data = new char[buf_size + 1];
		fread_s(
			data,
			buf_size + 1,
			sizeof(char),
			buf_size,
			stdin
		); 
		data[buf_size] = 0;
	}
}
void show_header() {
	cout << "Homework";
}
void show_content() {
	cout << "<form method = 'get' action = 'second.cgi'>";
	cout << "Введите последовательность скобок : <input type = 'text' name = 'bracket' maxlength = '50' size = '50'><br>";
	cout << "<input type = 'submit' value = 'Отправить'>";
	cout << "</form>";
	char* data = nullptr;
	get_form_data(data);
	if (data && strlen(data) > 0) {
		//cout << "<div>" << data << "</div>";
		char* value = nullptr;
		get_param_value(value, "bracket", data);
		cout << "<div>" << value << "</div>";
		brackets(value);
	}
	delete[] data;
}
void show_menu() {
	ifstream f("menu");
	if (f.is_open()) {
		auto sz = 1024;
		auto buf = new char[sz];
		while (!f.eof()) {
			f.getline(buf, sz);
			char* name;
			char* addrr = strtok_s(buf, " ", &name);
			cout << "<div class = 'menu-item'>"
				"<a href='" << addrr << "'>" <<
				name << "</a>"
				"</div>";
		}
	}
	f.close();
}

int main() {
	cout << "Content-type: text/html; charset=Windows-1251\n\n";
	ifstream f("_index");
	if (f.is_open()) {
		auto sz = 65536;
		auto buf = new char[sz];
		while (!f.eof()) {
			f.getline(buf, sz);
			if (strcmp(buf, "<!--#CONTENT#-->") == 0) {
				show_content();
			}
			else if (!strcmp(buf, "<!--#MENU#-->")) {
				show_menu();
			}
			else if (!strcmp(buf, "<!--#Header#-->")) {
				show_header();
			}
			cout << buf;
		}
		delete[] buf;
		f.close();
	}
	else {
		cout << "Не удалось вывести страницу :( ):";
	}
	return 0;
}