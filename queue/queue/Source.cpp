#include<iostream>
#include<fstream>
#include<cstdlib>
#include<cstring>
#include<cstdio>
using namespace std;

struct Human {
	string name;
	string gender;
	int age;
	string work;
	string earnings;//заработок
	string target;//цель
};
struct elem {
	Human value;
	elem* next;
};
struct queue // стрктура, которая содержит два указателя, на голову и на хвост 
{
	elem* head = nullptr;
	elem* tail = nullptr;
	size_t length = 0;// тут храним размер очереди
};
void enqueue(queue& q, Human H) {
	elem* newel = new elem;
	newel->value = H;
	if (!q.tail) {
		q.head = q.tail = newel;
	}
	else {
		q.tail->next = newel;
		q.tail = newel;
	}
	q.length++;
};
bool dequeue(queue& q, Human& H) {
	if (!q.head)
		return false;
	H = q.head->value;
	elem* rem = q.head;
	q.head = q.head->next;
	delete rem;
	if (q.head) q.tail = nullptr;
	q.length--;
	return true;
};

void Queue(string filename) {
	//setlocale(LC_ALL, "Russian");
	ifstream People;
	People.open(filename);
	if (!People.is_open()) {
		cout << "File not found" << endl;
		return;
	}
	queue Kredit;
	queue Vklad;
	ofstream FileKredit("Kredit.txt");
	ofstream FileVklad("Vklad.txt");
	while (!People.eof()) {//пока не конец файла
		Human H;
		People >> H.name >> H.gender >> H.age >> H.work >> H.earnings >> H.target;
		if (H.target == "Kredit") {
			enqueue(Kredit, H);
			FileKredit << H.name << " " << H.gender << " " << H.age << " " << H.work << " " << H.earnings << endl;
		}
		else {
			enqueue(Vklad, H);
			FileVklad << H.name << " " << H.gender << " " << H.age << " " << H.work << " " << H.earnings << endl;
		}
	}
	cout << "Kredit :" << "<br>";
	while (Kredit.length > 0) {
		Human out;
		dequeue(Kredit, out);
		cout << out.name << " " << out.gender << " " << out.age << " " << out.work << " " << out.earnings << "<br>";
	}
	cout << "<br>";
	cout << "Vklad :" << "<br>";
	while (Vklad.length > 0) {
		Human out;
		dequeue(Vklad, out);
		cout << out.name << " " << out.gender << " " << out.age << " " << out.work << " " << out.earnings << "<br>";
	}
	People.close();
	FileKredit.close();
	FileVklad.close();
}


void str_decode(char*& dec_str, const char* enc_str) {
	char* res = new char[strlen(enc_str) + 1];
	int i = 0, j = 0;
	while (enc_str[i]) {
		if (enc_str[i] == '+') {
			res[j] = ' ';
		}
		else {
			if (enc_str[i] == '%') {
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
	cout << "<form method = 'get' action = 'queue.cgi'>";
	cout << "Прикрепите файл(.txt) : <input type = 'file' name = 'ochered' accept='*/txt'>"; //maxlength = '50' size = '50'><br>";  accept="image/*,image/jpeg"
	cout << "<input type = 'submit' value = 'Отправить'>";
	cout << "</form>";
	char* data = nullptr;
	get_form_data(data);
	if (data && strlen(data) > 0) {
		//cout << "<div>" << data << "</div>";
		char* value = nullptr;
		get_param_value(value, "ochered", data);
		//cout << "<div>" << value << "</div>";
		ifstream People;
		People.open("Data.txt");
		Queue("Data.txt"); //это функция кода задачи,должна быть функция queue	
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