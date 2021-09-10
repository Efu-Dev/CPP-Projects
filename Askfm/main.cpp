#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<vector>
#include<algorithm>
#include<sstream>
#include<string.h>
using namespace std;

/***********************************************Global Function Declaration**************************************************/
int getLastId(bool forUser);
void updateLastIds();
void incrementId(bool isUser);
void load_users();
void printUsers();
void clearUsers();
/***********************************************Global Variables***********************************************************/
vector< pair<int, pair<string, string> > > users; //Will contain the users currently registered in the RAM.
/***********************************************LOGIN MENU****************************************************************/
struct Question{
    int askerid, receiverid, qid, fatherid, answerer;
    string question, answer;
    bool answered, priv;
    Question(){}

    Question(int _askerid, int _receiverid, int _qid, int _fatherid, int _answererid, string _question, string _ans, bool _answered, bool _priv){
        askerid = _askerid;
        receiverid = _receiverid;
        qid = _qid;
        question = _question;
        priv = _priv;
        answer = _ans;
        fatherid = _fatherid;
        answered = _answered;
        answerer = _answererid;
    }

    void printQuestion(){
        load_users();
        cout << "(" << qid << ")Asked to " << findUsernameWithId(receiverid) << " by ";
        if(!priv)
            cout << findUsernameWithId(askerid);
        else
            cout << "(hidden)";
        cout << ":\nQ: " << question << endl;
        if(answered){
            cout << "Answered by ";
            if(priv)
                cout << "(hidden)";
            else
                cout << findUsernameWithId(answerer);
            cout << ":\nA: " << answer << "\n\n";
        }
        else
            cout << "(Not Answered yet)\n\n";
    }

    string findUsernameWithId(int _id){
        for(auto user : users)
                if(user.first == _id)
                    return user.second.first;
        return "";
    }

    void printAsChild(){
        load_users();
        cout << "\t(" << qid << ")Asked by (last replier)";
        cout << ":\n\tQ: " << question << endl;
        if(answered){
            cout << "\tAnswered";
            cout << ":\n\tA: " << answer << "\n\n";
        }
        else
            cout << "\n\t(Not Answered yet)\n\n";
    }
};

struct askFm{
    int id;
    string user;
    vector<Question> questions;
    askFm(){}

    askFm(string _username){
        system("CLS");
        user = _username;
        id = searchId(user);
        menu();
    }

    int searchId(string usern){
        int searchingid;
        load_users();
        for(auto user: users){
            if(user.second.first.compare(usern) == 0){
                searchingid = user.first;
                break;
            }

        }
        return searchingid;
    }

    void menu(){
        short option;
        do{
            load_users();
            cout << "Welcome, " << user << " (" << id << ")!\nThese are the options:\n";
            cout << "1. Print questions for me.\n2. Print questions from me.\n3. Answer a question.\n4. Delete question.";
            cout << "\n5. Ask question.\n6. List System Users.\n7. Feed.\n8. Logout.\n\nYour option: ";
            cin >> option;
            system("CLS");
            switch(option){
                case 1:
                    printQuestionsForUser();
                    break;
                case 2:
                    printQuestionsFromUser();
                    break;
                case 3:
                    answerQuestion();
                    break;
                case 4:
                    deleteQuestion();
                    break;
                case 5:
                    askQuestion();
                    break;
                case 6:
                    load_users();
                    printUsers();
                    break;
                case 7:
                    feed();
                    break;
                case 8:
                    system("CLS");
                    cout << "Bye " << user << "!\n";
                    break;
                default:
                    system("CLS");
                    cout << "Option not recognized.\n";
                    break;
            }
            system("PAUSE");
            system("CLS");
        }while(option != 8);

    }

    void printQuestions(bool forUser){
        updateQuestions();
        vector<int> questionsId;
        for(Question question : questions)
            if(find(questionsId.begin(), questionsId.end(), question.qid) == questionsId.end()) {
                if(forUser && question.receiverid == id){
                    question.printQuestion();
                    if(question.fatherid != -1){
                        vector<Question> childQ = childrenQuestions(question);
                        if(childQ.empty())
                            continue;
                        cout << "{ Thread:\n";

                            for(Question que : childQ){
                                que.printAsChild();
                                questionsId.push_back(que.qid);
                            }
                        cout << "} End of this Thread.\n\n";
                    }
                }
                else if(!forUser && question.askerid == id){
                    question.printQuestion();
                    if(question.fatherid != -1){
                        cout << "{ Thread:\n";
                        vector<Question> childQ = childrenQuestions(question);
                        if(childQ.empty())
                            continue;
                        for(Question que : childQ){
                            que.printAsChild();
                            questionsId.push_back(que.qid);
                        }
                        cout << "} End of this Thread.\n\n";
                    }
                }
            }

    }

    void printQuestionsForUser(){ //Option 1: Prints questions asked to the user. DONE.
        cout << "******* Questions for You *******\n";
        printQuestions(true);
    }

    void printQuestionsFromUser(){ //Option 2: Prints questions asked by the user. DONE.
        cout << "******* Your Questions *******\n";
        printQuestions(false);
    }

    void answerQuestion(){ //Option 3: User answers a question. DONE.
        cout << "******* Answer a Question *******\n";
        updateQuestions();
        for(Question question : questions)
            if(question.receiverid == id && !question.answered)
                question.printQuestion();

        bool flag = false;
        int answerid;
        string answer;
        while(!flag){
            cout << "Enter the id of the question you want to answer (-1 if  you'd rather not answer): ";
            cin >> answerid;
            if(answerid == -1)
                return;
            for(Question &question : questions)
                if(answerid == question.qid){
                    flag = true;
                    cout << "Enter the answer:  ";
                    cin.ignore(); getline(cin, answer);
                    question.answer = answer;
                    question.answered = true;
                    break;
                }
        }
        updateQuestionsFile();
        cout << "Question answered successfully!\n";
    }

    void deleteQuestion(){ //Option 4: User deletes one of their questions.
        cout << "******* Delete a Question *******\nThese are the questions you have asked:\n";
        updateQuestions();
        vector<Question> newQuestions{questions};

        for(Question q : questions)
            if(q.askerid == id)
                q.printQuestion();

        int deleteId;
        bool continueloop = true;
        vector<int> questionsToEliminate{};
        while(continueloop){
            cout << "Delete question with ID: ";
            cin >> deleteId;

            for(Question q : questions)
                if(deleteId == q.qid && id == q.askerid){
                    questionsToEliminate.push_back(deleteId);
                    vector<Question> children{childrenQuestions(q)};
                    for(Question cq:children)
                        questionsToEliminate.push_back(cq.qid);
                    continueloop = false;
                    break;
                }
            if(continueloop)
                cout << "Invalid ID\n";
        }

        vector<Question> newQuestion;
        for(Question q : questions)
            if(find(questionsToEliminate.begin(), questionsToEliminate.end(), q.qid) == questionsToEliminate.end()) {
                newQuestion.push_back(q);
            }
        questions = newQuestion;
        updateQuestionsFile();
    }

    void askQuestion(){ //Option 5: User asks a new question.
        Question newQ;
        updateQuestions();
        cout << "******* Ask a New Question *******\nThese are the users available: ";
        for(auto cuser : users)
            cout << cuser.first << " " << cuser.second.first << endl;

        string name; int receiverid;
        bool continueloop = true;
        while(continueloop){
            cout << "Enter the name of the user you want to ask: ";
            cin >> name;

            for(auto user : users)
                if(user.second.first.compare(name) == 0){
                    continueloop = false;
                    receiverid = user.first;
                }

            if(continueloop)
                cout << name << " is not registered.";
        }
        system("CLS");

        while(true){
            int opt; string newQue; bool priv;
            cout << "Start a new thread (1) or continue a current one? (2): ";
            cin >> opt;
            if(opt == 1){
                cout << "Question: ";
                cin.ignore(); getline(cin, newQue);
                cout << "Private? (0 = no/1 = yes) ";
                cin >> priv;
                newQ = Question(id,receiverid, (questions[questions.size()-1].qid+1), -1, -1, newQue, " ", false, priv);
                break;
            }
            else if(opt == 2){
                vector<Question> available;
                for(Question q : questions)
                    if(q.answered && q.askerid == id && q.receiverid == receiverid){
                        q.printQuestion();
                        available.push_back(q);
                    }
                if(!available.empty()){
                    int idToThread;
                    Question qThread;
                    cout << "Enter the id of the question you want to thread: ";
                    cin >> idToThread;
                    for(Question &que : available)
                        if(que.qid == idToThread){
                            qThread = que;
                            if(que.fatherid == -1)
                                que.fatherid = -2;
                        }


                    cout << "Question: ";
                    cin.ignore(); getline(cin, newQue);
                    newQ = Question(id, receiverid, questions[questions.size() - 1].qid + 1, qThread.qid, -1, newQue, " ", 0, qThread.priv);
                    break;
                }
                else{
                    cout << "No questions available for this.";
                }
            }
            else{
                cout << "Invalid option!\n";
                continue;
            }
        }

        questions.push_back(newQ);
        updateQuestionsFile();
    }

    void printUsers(){ //Option 6: List Users
        cout << "****** Users List: *******\n";
        for(auto cuser : users)
            cout << cuser.first << " " << cuser.second.first << endl;
    }

    void feed(){ //Option 7: loads the feed.
        cout << "******* Feed *******\n";
        updateQuestions();
        for(Question q : questions)
            q.printQuestion();
    }

    void updateQuestions(){
        ifstream qStream("questions.data");
        if(qStream.fail()){
            cout << "Something went wrong when reading the users.txt file";
            return;
        }
        emptyQuestions();
        string line;
        bool meta = true, question = false, answer = false;

        int askerid, receiverid, qid, fatherid, answererid;
        bool answered, priv;
        string que, ans;

        while(getline(qStream, line)){
            if(meta){
                istringstream iss(line);
                iss >> askerid >> receiverid >> qid >> fatherid >> answererid >> answered >> priv;
                meta = false;
                question = true;
            }
            else if(question){
                que = line;
                question = false;
                answer = true;
            }
            else if(answer){
                ans = line;
                Question q(askerid, receiverid, qid, fatherid, answererid, que, ans, answered, priv);
                questions.push_back(q);
                answer = false;
                meta = true;
            }
        }
        qStream.close();
    }

    void emptyQuestions(){
        while(!questions.empty())
            questions.pop_back();
    }

    void updateQuestionsFile(){
        auto status = ios::in | ios::out | ios::trunc;
        fstream qStream("questions.data", status);

        if(qStream.fail()){
            cout << "Please turn off the program. An error has ocurred.";
            return;
        }

        for(Question q : questions){
            qStream << q.askerid << " " << q.receiverid << " " << q.qid << " " << q.fatherid << " " << q.answerer << " " << q.answered << " " << q.priv << "\n";
            qStream << q.question << "\n";
            qStream << q.answer << "\n";
        }
    }

    vector<Question> childrenQuestions(Question question){
        vector<Question> childrenQuestions;
        int curfather = question.qid;
        for(Question q:questions)
            if(q.fatherid == curfather){
                childrenQuestions.push_back(q);
                curfather = q.qid;
            }
        return childrenQuestions;
    }

};

struct loginSigninMenu{ //Contains everything before you enter the system: log in and sign in.
    //Constructor:
    loginSigninMenu(){
        loginMenu();
    }

    //Methods:

    void loginMenu(){
        short option;
        do
        {
            cout << "Press 1 to log in. Press 2 to sign in. Press 3 to finish the program: ";
            cin >> option;

            if(option < 1 && option > 3)
                cout << option << " is not valid option";

        }while(option < 1 && option > 3);

        if(option == 1)
            login();
        else if(option == 2)
            signin();
        else if(option == 3)
            return;

        system("CLS");
        loginMenu();
    }

    /*Sign In Functions*/

    void signin(){
        cout << "---New User Creation---\n";
        string newUsername, password;
        cout << "New Username: ";
        cin >> newUsername;

        if(existsUsername(newUsername)){
            cout << "\nThis user can't be created because that name already exists!\n";
            return;
        }

        cout << "Password: ";
        cin >> password;
        createUser(newUsername, password);
    }


    void createUser(string name, string password){
        long currentLastUserId = getLastId(true);
        auto users_status = ios::in | ios::out | ios::app;

        string users_path = "users.data";
        fstream users_handler(users_path.c_str(), users_status);

        if (users_handler.fail()) {
            cout << "Can't open the users file\n";
            return;
        }
        currentLastUserId++;
        users_handler << currentLastUserId << " " << name << " " << password << "\n";
        users_handler.close();
    }

    /*LogIn Functions*/

    void login(){
        string username, password;
        load_users();

        if(users.empty()){
            cout << "There are no users registered.\n";
            return;
        }
        while(true){
            cout << "Username: ";
            cin >> username;
            if(!existsUsername(username)){
                cout << "Username doesn't exist!\n";
                return;
            }
            else{
                cout << "Password: ";
                cin >> password;
                if(!validPassword(password)){
                    cout << "Invalid password!\n";
                    return;
                }
                else{
                    cout << "--- Logged In successfully!---\n";
                    askFm instance = askFm(username);
                    return;
                }
            }
        }

    }

    bool existsUsername(string username){
        for(auto user : users){
            string currentusername = user.second.first;
            if(username.compare(currentusername) == 0)
                return true;
        }
        return false;
    }

    bool validPassword(string password){
        for(auto user : users){
            string currentpass = user.second.second;
            if(currentpass.compare(password) == 0)
                return true;
        }
        return false;
    }

};

/***********************************************Global Functions*********************************************************/
int getLastId(bool forUser){
    if(forUser){
        load_users();
        if(!users.empty())
            return users[users.size()-1].first;
    }
    return 0;
}

void load_users(){

    if(!users.empty())
        clearUsers();

    ifstream usersStream;
    usersStream.open("users.data");

    if(usersStream.fail()){
        cout << "Something went wrong when reading the users.txt file";
        return;
    }
    string line;
    while(getline(usersStream, line)){
        istringstream iss(line);
        int currentId;
        string currentname, currentpassword;
        iss >> currentId >> currentname >> currentpassword;
        users.push_back(make_pair(currentId, make_pair(currentname, currentpassword)));
    }
    usersStream.close();
}

void clearUsers(){
    auto it = users.begin();
    while(it != users.end())
        users.pop_back();
}

/***********************************************Main Function**********************************************************/
int main(){
    loginSigninMenu();
    return 0;
}
