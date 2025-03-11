#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include<unordered_map>
#include<unordered_set>
using namespace std;

class person{
    public:
        string name;
        float amtToReceive;
        float amtToPay;
        float amtPaid;
        person(){
            name = "";
            amtToReceive = 0;
            amtToPay = 0;
            amtPaid = 0;
        }
        friend ostream& operator<<(ostream&out,const person&p){
            out<<"Name:"<<p.name<<endl
                <<"Net Amount Paid: "<<p.amtPaid<<endl
                <<"Amount To Pay: "<<p.amtToPay<<endl
                <<"Amount To Receive: "<<p.amtToReceive<<endl
                <<"----------------"<<endl;
            return out;
        }
};

void toLowerCase(string&s){
    for(int i=0;i<s.length();i++){
        s[i] = tolower(s[i]);
    }
}

void displayAllPerson(vector<person>&p){
    for(person per : p){
        cout<<per;
    }
}

void DividePayment(vector<person>&p,float totalAmt){
    int n = p.size();
    if(totalAmt==0) return;

    float contri = 1.0*totalAmt/n;

    for(auto& per : p){
        if(per.amtPaid < contri){
            per.amtToPay = contri - per.amtPaid;
            per.amtToReceive = 0;
        }else if(per.amtPaid > contri){
            per.amtToPay = 0;
            per.amtToReceive = per.amtPaid - contri;
        }else{
            per.amtToPay = per.amtToReceive = 0;
        }
    }
}

void writeInFile(vector<person>&p,string grpName){
    ofstream out("Database.txt",ios::app);
    
    out<<"Group name: "<<grpName<<endl;
    for(int i=0;i<p.size();i++){
        out<<"name: "<<p[i].name<<endl
        <<"amtPaid: "<<p[i].amtPaid<<endl
        <<"amtToPay: "<<p[i].amtToPay<<endl
        <<"amtToReceive: "<<p[i].amtToReceive<<endl;
    }
    out<<"**"<<endl;
    out.close();
}

unordered_set<string> getGroups(){

    unordered_set<string> groups;
    
    ifstream in("Database.txt");
    if(!in){
        cout<<"Error: database file not found"<<endl;
        return groups;
    }
    string line;

    while(getline(in,line)){
        if(line.substr(0,11)=="Group name:"){
            groups.insert(line.substr(12));
        }
    }
    in.close();
    return groups;
}

void newGroup(){
    cout<<"Enter the name of the Group"<<endl;
    string grpName; 
    cin.ignore();
    getline(cin,grpName);
    toLowerCase(grpName);
    unordered_set<string> groups = getGroups();
    while(groups.find(grpName) != groups.end()){
        cout<<"Group Name Already Exist!!"<<endl;
        getline(cin,grpName);
        toLowerCase(grpName);
    }
    cout<<"Enter number of person:";
    int n; cin>>n;
    cin.ignore();
    if(n==0) return;

    vector<person> p(n);
    unordered_map<string,int> nameVsIdx;
    
    for(int i=0;i<n;i++){
        cout<<"Enter the name of person:";
        string name;
        getline(cin,name); 
        if(name==""){
            cout<<"Invaild...please enter the name!!"<<endl;
            continue;
        }

        toLowerCase(name);
        if(nameVsIdx.find(name) != nameVsIdx.end()){
            //name already exist
            cout<<"Name Already exist"<<endl;
        }else{
            p[i].name = name;
            nameVsIdx[p[i].name]=i;
        }
    }
    cout<<"Enter number of payments:";
    int payments; cin>>payments;
    cin.ignore();
    
    int j=0;
    float totalAmtPaid=0;
    while(j<payments){
        string name;
        float amt;
        cout<<"Enter person name and press enter : ";
        getline(cin,name);
        cout<<"Enter amount paid by them: ";
        cin>>amt;
        cin.ignore();
        
        toLowerCase(name);

        if(nameVsIdx.find(name) == nameVsIdx.end()){
            cout<<"Name Doesn't exist"<<endl;
        }else{
            int pIdx = nameVsIdx[name];
            p[pIdx].amtPaid+=amt;
            totalAmtPaid+=amt;
            j++;
        }
    }
    
    DividePayment(p,totalAmtPaid);
    cout<<endl<<"Group has been created:"<<endl<<endl;
    displayAllPerson(p);

    //write/append in file 
    writeInFile(p,grpName);

}

void displayGroups(unordered_set<string>&groups){
    cout<<"Available Groups: "<<endl;
    int i=1;
    for(auto grp : groups){
        cout<<i++<<". "<<grp<<endl;
    }
    cout<<endl;
}

void deleteGroup(unordered_set<string>& groups,string gName=""){
    if(gName==""){
        displayGroups(groups);
        cout<<"enter the name of the group: "<<endl;
        cin.ignore();
        getline(cin,gName);
        toLowerCase(gName);

        bool del;
        cout<<"Are you sure u want to delete "<<gName<<" group? (0 for no, 1 for yes)";
        cin>>del;
        if(!del){
            cout<<"Group was not deleted"<<endl;
            return;
        }else{
            cout<<"Group was deleted..."<<endl;
        }
    }
    
    groups.erase(gName);

    ifstream in("Database.txt");

    if(!in){
        cout<<"Error: database file not found";
        return;
    }

    bool skip=false;
    vector<string> fileLines;
    string line;

    while(getline(in,line)){
        if(line == "Group name: "+gName) skip = true;
        if(!skip) fileLines.push_back(line);
        if(skip && line=="**") skip = false;
    }
    in.close();
    
    ofstream out("Database.txt");

    for(auto l : fileLines){
        out<<l<<endl;
    }
    out.close();
}

float retrievePersonInfo(vector<person>&p,string grpName){
    //returns total amount paid by all 
    ifstream in("Database.txt");
    if(!in){
        cout<<"Error: database file not found"<<endl;
        exit(1);
    }
    string line;
    bool store=false;
    float totalAmt = 0;
    person per;

    while(getline(in,line)){
        if(line=="Group name: "+grpName) store=true;
        if(store){
            if(line.rfind("name") != string::npos){
                per.name = line.substr(6);
            }else if(line.rfind("amtPaid") != string::npos){
                per.amtPaid = stof(line.substr(9));
                totalAmt += per.amtPaid*1.0;
            }else if(line.rfind("amtToPay") != string::npos){
                per.amtToPay = stof(line.substr(10));
            }else if(line.rfind("amtToReceive") != string::npos){
                per.amtToReceive = stof(line.substr(14));
                p.push_back(per);
                per = person();
            }
        }
        if(store && line=="**") break;
    }
    return totalAmt;
}

void addPaymentsToExitingGroups(unordered_set<string>& groups){
    displayGroups(groups);
    cout<<"enter the name of the group: "<<endl;
    string gName;
    cin.ignore();
    getline(cin,gName);
    toLowerCase(gName);
    while(groups.find(gName) == groups.end()){
        cout<<"Group Name Doesn't Exist!!"<<endl;
        getline(cin,gName);
        toLowerCase(gName);
    }

    vector<person> p;
    float totalAmtPaid = retrievePersonInfo(p,gName);
    unordered_map<string,int> nameVsIdx;
    
    for(int i=0;i<p.size();i++){
        nameVsIdx[p[i].name] = i;
    }

    cout<<"Enter number of payments:";
    int payments; cin>>payments;
    cin.ignore();
    
    int j=0;
    while(j<payments){
        string name;
        float amt;
        cout<<"Enter person name and press enter : ";
        getline(cin,name);
        cout<<"Enter amount paid by them: ";
        cin>>amt;
        cin.ignore();
        
        toLowerCase(name);

        if(nameVsIdx.find(name) == nameVsIdx.end()){
            cout<<"Name Doesn't exist"<<endl;
        }else{
            int pIdx = nameVsIdx[name];
            p[pIdx].amtPaid += amt;
            totalAmtPaid += amt;
            j++;
        }
    }
    DividePayment(p,totalAmtPaid);
    deleteGroup(groups,gName);
    writeInFile(p,gName);
    groups.insert(gName);

    displayAllPerson(p);

}

void settlePayments(unordered_set<string>& groups){
    displayGroups(groups);
    cout<<"enter the name of the group: "<<endl;
    string gName;
    cin.ignore();
    getline(cin,gName);
    toLowerCase(gName);
    while(groups.find(gName) == groups.end()){
        cout<<"Group Name Doesn't Exist!!"<<endl;
        getline(cin,gName);
        toLowerCase(gName);
    }

    vector<person> p;
    retrievePersonInfo(p,gName);
    unordered_map<string,int> nameVsIdx;

    for(int i=0;i<p.size();i++){
        nameVsIdx[p[i].name] = i;
    }

    cout<<"How many settlements to make?: ";
    int numSettle; cin>>numSettle;
    cin.ignore();
    bool isUpdated = false;

    for(int j=0;j<numSettle;){
        cout<<"Enter the name of the person who wants to settle: ";
        string name;
        getline(cin,name);
        
        if(nameVsIdx.find(name) == nameVsIdx.end()){
            cout<<"Name Doesn't Exist!!"<<endl;
            continue;
        }

        int pIdx = nameVsIdx[name];
        
        if(p[pIdx].amtToPay == 0){
            cout<<"The Selected person has nothing to settle...Person Details:"<<endl;
            cout<<p[pIdx];
            continue;
        }

        for(auto& per : p){
            if(per.name == p[pIdx].name) {
                per.amtPaid += per.amtToPay;
                continue;
            }
            isUpdated = true;

            if(per.amtToReceive >= p[pIdx].amtToPay){
                per.amtPaid -= p[pIdx].amtToPay;
                per.amtToReceive -= p[pIdx].amtToPay;
                p[pIdx].amtToPay = 0;
                break;
            }else{
                per.amtPaid -= per.amtToReceive;
                p[pIdx].amtToPay -= per.amtToReceive;
                per.amtToReceive = 0;
            }
        }
        j++;
    }
    if(isUpdated){
        deleteGroup(groups,gName);
        writeInFile(p,gName);
        groups.insert(gName);
    
        displayAllPerson(p);
    }
}

void existingGroup(){
    unordered_set<string> groups = getGroups();
    if(groups.empty()){
        cout<<"No Existing Groups"<<endl;
        return;
    }

    int toDo;
    do{
        cout<<"----------------"<<endl;
        cout<<"Enter Your Choice: "<<endl
            <<"1.Add Payments"<<endl //done
            <<"2.Settle Payments"<<endl //done
            <<"3.Delete Group"<<endl //done 
            <<"4.Back To Main Menu"<<endl;
            cin>>toDo;
            
            
        switch(toDo){
            case 1 : addPaymentsToExitingGroups(groups); break;
            case 2 : settlePayments(groups); break;
            case 3 : deleteGroup(groups); break;
            case 4 : return;
            default : cout<<"Invaild input";break;
        }
        if(groups.empty()){
            cout<<"No More Groups"<<endl;
            return;
        }
    }while(toDo!=4);
}

void ViewExistingGroup(){
    unordered_set<string> groups = getGroups();
    if(groups.empty()){
        cout<<"No Existing Groups"<<endl;
        return;
    }
    for(auto gName : groups){
        vector<person> p;
        retrievePersonInfo(p,gName);
        cout<<"Group Name : "<<gName<<endl;
        for(auto per:p) cout<<per;
        cout<<"******************"<<endl;
    }
    return;
}

int main(){

    cout<<"Welcome!!"<<endl
        <<"This is a console based app for spliting the expenses among a group equally"<<endl;

    int choice;
    do{
        cout<<"----------------"<<endl;
        cout<<"Enter you choice"<<endl
            <<"1.Create a new Group"<<endl 
            <<"2.Update existing group"<<endl
            <<"3.View existing group"<<endl
            <<"4.Exit"<<endl;
        cin>>choice;
        switch(choice){
            case 1 : newGroup(); break; //done
            case 2 : existingGroup(); break; //done
            case 3 : ViewExistingGroup(); break; //done // addition view list of groups then view details of that group
            case 4 : cout<<"Exiting...."<<endl; break;
            default : cout<<"Invalid Input"<<endl<<endl; break;
        }
    }while(choice != 4);


    return 0;
}