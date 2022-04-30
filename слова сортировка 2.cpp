#include<iostream>
#include<string>
#include<sstream>
// односвязный список, отсортировать
using namespace std;
struct List{
	string word;
	List* next = nullptr;
};
void add_by_order(List*& head, string new_word){
	List* new_elem = new List; 
    new_elem->word = new_word; 
    if (!head) { 
        head = new_elem; 
        return; 
    } 
    List* prev = head; 
	if(strcmp(new_word.c_str(), prev->word.c_str()) == 0){//проверка элемента на равенство с head
		return;
	}
    if (strcmp(new_word.c_str(), prev->word.c_str()) == -1) { 
        new_elem->next = prev; 
        head = new_elem; 
        return; 
    }  
    while (prev->next) { 
		if((strcmp(new_word.c_str(), prev->next->word.c_str()) == 0)){
			return;
		}
        if ((strcmp(new_word.c_str(), prev->next->word.c_str()) == -1) && 
            (strcmp(new_word.c_str(), prev->word.c_str()) == 1)){ 
            new_elem->next = prev->next; 
            prev->next = new_elem; 
            return; 
        } 
        prev = prev->next; 
    } 
    prev->next = new_elem; 
}
void show(List* head){
	List* cur = head;
	while(cur){
		cout << cur -> word << " ";
		cur = cur -> next;
	}
	cout << endl;
}
int main(){
	List* head = nullptr;
	string word;
	getline(cin, word);//считываем строку string
	stringstream s(word);//сюда кладём эту строку string для считывания
	string w;
	while(s >> w){// из строки s считываем слова в w
		add_by_order(head, w);
	} 
	show(head);
	return 0;
}
