#include<iostream>
#include<fstream>
using namespace std;

void show_header() {
	cout << "Homework";
}
void show_content() {
	cout << "Тут будет поле для отправки ответа ^_^";
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