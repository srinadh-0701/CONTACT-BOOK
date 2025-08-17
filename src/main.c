#include <bits/stdc++.h>
using namespace std;

struct Node {
    string name, phone, email;
    Node* next;
    Node(const string& n, const string& p, const string& e): name(n), phone(p), email(e), next(nullptr) {}
};

string lowerStr(string s){ for(char& c: s) c = tolower((unsigned char)c); return s; }

class ContactList {
    Node* head = nullptr;

    static int cmpName(const string& a, const string& b){
        string la = lowerStr(a), lb = lowerStr(b);
        if (la < lb) return -1;
        if (la > lb) return 1;
        return 0;
    }

public:
    ~ContactList(){ clear(); }

    void clear(){
        while(head){ Node* t=head; head=head->next; delete t; }
    }

    void insertSorted(const string& name, const string& phone, const string& email){
        Node* n = new Node(name, phone, email);
        if(!head || cmpName(name, head->name) < 0){
            n->next = head; head = n; return;
        }
        Node* cur=head;
        while(cur->next && cmpName(cur->next->name, name) <= 0){
            cur = cur->next;
        }
        n->next = cur->next;
        cur->next = n;
    }

    Node* findByName(const string& name){
        string target = lowerStr(name);
        for(Node* cur=head; cur; cur=cur->next){
            if(lowerStr(cur->name) == target) return cur;
        }
        return nullptr;
    }

    Node* findByPhone(const string& phone){
        for(Node* cur=head; cur; cur=cur->next){
            if(cur->phone == phone) return cur;
        }
        return nullptr;
    }

    bool eraseByName(const string& name){
        if(!head) return false;
        string target = lowerStr(name);
        if(lowerStr(head->name) == target){
            Node* t=head; head=head->next; delete t; return true;
        }
        Node* cur=head;
        while(cur->next && lowerStr(cur->next->name) != target) cur=cur->next;
        if(cur->next){
            Node* t=cur->next; cur->next=t->next; delete t; return true;
        }
        return false;
    }

    void listAll(){
        cout << "\n--- Contacts ---\n";
        if(!head){ cout << "(empty)\n"; return;}
        for(Node* cur=head; cur; cur=cur->next){
            cout << cur->name << " | " << cur->phone << " | " << cur->email << "\n";
        }
        cout << "----------------\n";
    }

    void loadCSV(const string& path){
        ifstream in(path);
        if(!in) return;
        string line;
        while(getline(in, line)){
            if(line.empty()) continue;
            stringstream ss(line);
            string name, phone, email;
            if(!getline(ss, name, ',')) continue;
            if(!getline(ss, phone, ',')) phone="";
            if(!getline(ss, email, ',')) email="";
            insertSorted(name, phone, email);
        }
    }

    void saveCSV(const string& path){
        ofstream out(path);
        for(Node* cur=head; cur; cur=cur->next){
            out << cur->name << "," << cur->phone << "," << cur->email << "\n";
        }
    }
};

class App {
    ContactList list;
    string dataFile = "contacts.csv";
public:
    void run(){
        list.loadCSV(dataFile);
        while(true){
            cout << "\n==== Contact Book ====\n";
            cout << "1. Add contact\n";
            cout << "2. List contacts\n";
            cout << "3. Search by name\n";
            cout << "4. Search by phone\n";
            cout << "5. Update contact (by name)\n";
            cout << "6. Delete contact (by name)\n";
            cout << "0. Save & Exit\n";
            cout << "Choose: ";
            int ch; if(!(cin>>ch)) return;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if(ch==1){
                string n,p,e;
                cout<<"Name: "; getline(cin,n);
                cout<<"Phone: "; getline(cin,p);
                cout<<"Email: "; getline(cin,e);
                list.insertSorted(n,p,e);
                cout<<"Added.\n";
            } else if(ch==2){
                list.listAll();
            } else if(ch==3){
                string n; cout<<"Enter name: "; getline(cin,n);
                auto* node=list.findByName(n);
                if(node) cout<<node->name<<" | "<<node->phone<<" | "<<node->email<<"\n";
                else cout<<"Not found.\n";
            } else if(ch==4){
                string p; cout<<"Enter phone: "; getline(cin,p);
                auto* node=list.findByPhone(p);
                if(node) cout<<node->name<<" | "<<node->phone<<" | "<<node->email<<"\n";
                else cout<<"Not found.\n";
            } else if(ch==5){
                string n; cout<<"Enter existing name: "; getline(cin,n);
                auto* node=list.findByName(n);
                if(!node){ cout<<"Not found.\n"; continue; }
                string nn=node->name, pp=node->phone, ee=node->email;
                cout<<"New name (enter to keep '"<<nn<<"'): "; string x; getline(cin,x); if(!x.empty()) nn=x;
                cout<<"New phone (enter to keep '"<<pp<<"'): "; getline(cin,x); if(!x.empty()) pp=x;
                cout<<"New email (enter to keep '"<<ee<<"'): "; getline(cin,x); if(!x.empty()) ee=x;
                // if name changed, remove and reinsert to keep sort
                bool nameChanged = (lowerStr(nn)!=lowerStr(node->name));
                if(nameChanged){
                    list.eraseByName(node->name);
                    list.insertSorted(nn,pp,ee);
                } else {
                    node->name=nn; node->phone=pp; node->email=ee;
                }
                cout<<"Updated.\n";
            } else if(ch==6){
                string n; cout<<"Enter name to delete: "; getline(cin,n);
                bool ok=list.eraseByName(n);
                cout<<(ok?"Deleted.\n":"Not found.\n");
            } else if(ch==0){
                list.saveCSV(dataFile);
                cout<<"Saved to "<<dataFile<<". Bye!\n";
                return;
            } else {
                cout<<"Invalid choice.\n";
            }
        }
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    App app; app.run();
    return 0;
}
