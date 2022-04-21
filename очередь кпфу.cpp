//список
#include<iostream>
#include<fstream>
using namespace std;
struct Human{
	string name;
	string gender;
	int age;
	string work;
	string earnings;//заработок
	string target;//цель
};
struct elem{
	Human value;
	elem* next;
};
struct queue // стрктура, которая содержит два указателя, на голову и на хвост 
{
	elem* head = nullptr;
	elem* tail = nullptr;
	size_t length = 0;// тут храним размер очереди
};
void enqueue(queue& q, Human H){
	elem* newel = new elem;
	newel -> value = H;
	if(!q.tail){
		q.head = q. tail = newel;
	}
	else{
		q.tail -> next = newel;
		q.tail = newel;
	}
	q.length++;
};
bool dequeue(queue& q, Human& H){
	if(!q.head)
		return false;
	H = q.head -> value;
	elem* rem = q.head;
	q.head = q.head -> next;
	delete rem;
	if(q.head) q.tail = nullptr;
	q.length--;
	return true;
};

int main(){
	setlocale(LC_ALL, "Russian");
	ifstream People;
	People.open("Data.txt");
	if(!People.is_open()){
		cout << "File not found" << endl;
		return 0;
	}
	queue Kredit;
	queue Vklad;
	ofstream FileKredit("Kredit.txt");
	ofstream FileVklad("Vklad.txt");
	while(!People.eof()){//пока не конец файла
		Human H;
		People >> H.name >> H.gender >> H.age >> H.work >> H.earnings >> H.target;
		if(H.target == "Kredit"){
			enqueue(Kredit, H);
			FileKredit << H.name << " "<< H.gender << " " << H.age << " " << H.work << " " << H.earnings << endl;
		}
		else{
			enqueue(Vklad, H);
			FileVklad << H.name << " " << H.gender << " " << H.age << " " << H.work << " " << H.earnings << endl;
		}
	}
	People.close();
	FileKredit.close();
	FileVklad.close();
	//cout << "Kredit.txt" << endl << "Vklad.txt";
	return 0;
}
