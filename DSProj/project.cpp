#include <iostream>
#include "csv.hpp"
#include <map>
#include <unordered_map>
#include<queue> 
#include<stdio.h>
#include <conio.h>
#include <string.h>
#include <iomanip>
#include <ctime>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>




using namespace csv;
using namespace std;

const long int S = 50;
const int minGames = 30;


class Probability {
public: 
    string opening;
    int wins;
    int loss;
    int draw;
    double winProbability;
    int totalGamesPlayed;

    Probability() {
        opening = "";
        wins = 0;
        loss = 0;
        draw = 0;
        winProbability = 0.00;
        totalGamesPlayed = 0;
    }

    Probability(string o,int w,int l,int d,double Wp,int g) {
        opening = o;
        wins = w;
        loss = l;
        draw = d;
        winProbability = Wp;
        totalGamesPlayed = g;
    }



};

class Openings {
public:
    string openings;
    int frequency;
   

    Openings() {
        openings = "";
        frequency = 0;
    }

    Openings(string o, int f) {
        openings = o;
        frequency = f;
    }

    
};


class OpeningData {
public:
    string openingName;
    unordered_map<string, int>freqevents;
    int wins;
    int loss;
    int draw;

    OpeningData() {
        openingName = "";
        wins = 0;
        loss = 0;
        draw = 0;
    }

    OpeningData(string m,int w,int l,int d) {
        openingName = m;
        wins = w;
        loss = l;
        draw = d;
    }
};

class UserData {
public:

    string userId;
    unordered_map<string,int> eventsPlayed; //its a hashtable. searching time is O(1).
    int wins;
    int loss;
    int draw;
    unordered_map<string, int> openings;
    int playedAsWhite;
    int playedAsBlack;
 
    UserData() {
        userId = "";
        wins = 0;
        loss = 0;
        draw = 0;
        playedAsWhite = 0;
        playedAsBlack = 0;

    } 
};

class RowData{
public:
    string Event;
    string White;
    string Black;
    string Result;
    int WhiteElo;
    int BlackElo;
    int WhiteRatingDiff;
    int BlackRatingDiff;
    string ECO;
    string Opening;
    string Termination;

 

    RowData() {
        this->Event = "";
        this->White = "";
        this->Black = "";
        this->Result = "";
        this->WhiteElo = 0;
        this->BlackElo = 0;
        this->WhiteRatingDiff = 0;
        this->BlackRatingDiff = 0;
        this->ECO = "";
        this->Opening = "";
        this->Termination = "";  
    }

    RowData(string Event, string White,string Black,string Result,int WhiteElo,int BlackElo,int WhiteRatingDiff,int BlackRatingDiff,string ECO,string Opening,string Termination) {
        this->Event = Event;
        this->White = White;
        this->Black = Black;
        this->Result = Result;
        this->WhiteElo = WhiteElo;
        this->BlackElo = BlackElo;
        this->WhiteRatingDiff = WhiteRatingDiff;
        this->BlackRatingDiff = BlackRatingDiff;
        this->ECO = ECO;
        this->Opening = Opening;
        this->Termination = Termination;

    }


};



bool operator < (const Openings& o1, const Openings& o2) {

    return o1.frequency < o2.frequency;

}

void getPlayersData(string player,unordered_map<string, UserData>& userMapping);
void openingsCommonELO(map<int, map<string, int>>& rangeMapping, vector<RowData>& rowobjects);
void probability(map<string, Probability>& ProbMapping);
void getOpeningData(unordered_map<string, OpeningData>& DataMap, string open);
double dotProduct(vector<int>& X, vector<double>& weights);
double sigmoid(double z);
double costFunction(vector<double>& y, vector<double>& y_hat);
void gradients(vector<vector<int>>& X, vector<double>& y, vector<double>& y_hat);
void train(vector<vector<int>>& X, vector<double>& y, int epochs, double lr);
void printsortedData(vector<RowData>& rowobjects);
void mergeSort(vector<RowData>& vec, int left, int right);
void merge(vector<RowData>& array, int const left, int const mid, int const right);


void ReadFileandStore(vector<RowData>& rowobjects) {
    int i = 0;
    CSVReader reader("chessData.csv");

    for (CSVRow& row : reader) {
        if (i == S)
            break;

        try {
            rowobjects[i].Event = row["Event"].get<string>();
            rowobjects[i].White = row["White"].get<string>();
            rowobjects[i].Black = row["Black"].get<string>();
            rowobjects[i].Result = row["Result"].get<string>();
            rowobjects[i].WhiteElo = row["WhiteElo"].get<int>();
            rowobjects[i].BlackElo = row["BlackElo"].get<int>();
            rowobjects[i].WhiteRatingDiff = row["WhiteRatingDiff"].get<int>();
            rowobjects[i].BlackRatingDiff = row["BlackRatingDiff"].get<int>();
            rowobjects[i].ECO = row["ECO"].get<string>();
            rowobjects[i].Opening = row["Opening"].get<string>();
            rowobjects[i].Termination = row["Termination"].get<string>();
        }
        catch (exception e) {
            cout << "Exception Occured at Row: " << i + 2 << "      "; 
            std::cerr << e.what();
            cout << endl;
            cout << "---------------------------------------------------------------------------------------------------------------" << endl;
            i--;
            system("CLS");
        }

        i++;
    } 

    cout << "Rows Read: " << i << endl<<endl;
}

/* for (auto str : reader.get_col_names())
       cout << str << "\t\t";

   for (int i = 0; i < 5000; i++) {
       cout << rowobjects[i].Event << "\t\t"
           << rowobjects[i].White << "\t\t"
           << rowobjects[i].Black << "\t\t"
           << rowobjects[i].WhiteElo << "\t\t"
           << rowobjects[i].BlackElo << "\t\t"
           << rowobjects[i].WhiteRatingDiff << "\t\t"
           << rowobjects[i].BlackRatingDiff << "\t\t"
           << rowobjects[i].ECO << "\t\t"
           << rowobjects[i].Opening << "\t"
           << rowobjects[i].Termination << "\t"
           << endl;
   }*/

int main()
{
    int i = 0;
    int max = -9999;

    CSVReader reader("chessData.csv");

    vector<RowData> rowobjects(S);

    unordered_map<string, UserData> userMapping;

    map<int, map<string, int>> rangeMapping;

    map<string, int> openingMapping;

    map<string, Probability> ProbMapping;

    unordered_map<string, OpeningData> openingDataMapping;

    vector<int> Xtemp;
    vector<vector<int>> X;
    vector<double> y;
   

     ReadFileandStore(rowobjects);  //we are reading all rows and storing its data in Class object RowData.

    for (int i = 0; i < rowobjects.size(); i++) {

        //<----------Mapping UserData---------->

        if (rowobjects[i].Result == "Jan-00") {
            userMapping[rowobjects[i].White].wins++;
            userMapping[rowobjects[i].Black].loss++;
        }
        if (rowobjects[i].Result == "0-1") {
            userMapping[rowobjects[i].Black].wins++;
            userMapping[rowobjects[i].White].loss++;
        }
        if (rowobjects[i].Result == "1/2-1/2") {
            userMapping[rowobjects[i].White].draw++;
            userMapping[rowobjects[i].Black].draw++;
        }

        userMapping[rowobjects[i].White].eventsPlayed[rowobjects[i].Event]++;
        userMapping[rowobjects[i].Black].eventsPlayed[rowobjects[i].Event]++;
        userMapping[rowobjects[i].White].playedAsWhite++;
        userMapping[rowobjects[i].Black].playedAsBlack++;
        userMapping[rowobjects[i].White].userId = rowobjects[i].White;
        userMapping[rowobjects[i].Black].userId = rowobjects[i].Black;
        userMapping[rowobjects[i].White].openings[rowobjects[i].Opening]++;


        //<----------Mapping openings according to the ELO ranking.---------->
        if (rowobjects[i].WhiteElo >= 750 && rowobjects[i].WhiteElo <= 1000){

            if (rangeMapping.find(750) == rangeMapping.end()) {
                if (rangeMapping[750].find(rowobjects[i].Opening) == rangeMapping[750].end()) {
                    rangeMapping[750][rowobjects[i].Opening] = 1;
                }
            }
            else {
                rangeMapping[750][rowobjects[i].Opening]++;
            }

        }

        if (rowobjects[i].WhiteElo > 1000 && rowobjects[i].WhiteElo <= 1250) {

            if (rangeMapping.find(1000) == rangeMapping.end()) {
                if (rangeMapping[1000].find(rowobjects[i].Opening) == rangeMapping[1000].end()) {
                    rangeMapping[1000][rowobjects[i].Opening] = 1;
                }
            }
            else {
                rangeMapping[1000][rowobjects[i].Opening]++;
            }
        }

        if (rowobjects[i].WhiteElo > 1250 && rowobjects[i].WhiteElo <= 1500) {

            if (rangeMapping.find(1250) == rangeMapping.end()) {
                if (rangeMapping[1250].find(rowobjects[i].Opening) == rangeMapping[1250].end()) {
                    rangeMapping[1250][rowobjects[i].Opening] = 1;
                }
            }
            else {
                rangeMapping[1250][rowobjects[i].Opening]++;
            }
        }

        if (rowobjects[i].WhiteElo > 1500 && rowobjects[i].WhiteElo <= 1750) {

            if (rangeMapping.find(1500) == rangeMapping.end()) {
                if (rangeMapping[1500].find(rowobjects[i].Opening) == rangeMapping[1500].end()) {
                    rangeMapping[1500][rowobjects[i].Opening] = 1;
                }
            }
            else {
                rangeMapping[1500][rowobjects[i].Opening]++;
            }
        }

        if (rowobjects[i].WhiteElo > 1750 && rowobjects[i].WhiteElo <= 2000) {

            if (rangeMapping.find(1750) == rangeMapping.end()) {
                if (rangeMapping[1750].find(rowobjects[i].Opening) == rangeMapping[1750].end()) {
                    rangeMapping[1750][rowobjects[i].Opening] = 1;
                }
            }
            else {
                rangeMapping[1750][rowobjects[i].Opening]++;
            }
        }

        if (rowobjects[i].WhiteElo > 2000 && rowobjects[i].WhiteElo <= 2250) {

            if (rangeMapping.find(2000) == rangeMapping.end()) {
                if (rangeMapping[2000].find(rowobjects[i].Opening) == rangeMapping[2000].end()) {
                    rangeMapping[2000][rowobjects[i].Opening] = 1;
                }
            }
            else {
                rangeMapping[2000][rowobjects[i].Opening]++;
            }
        }

        if (rowobjects[i].WhiteElo > 2250 && rowobjects[i].WhiteElo <= 2500) {

            if (rangeMapping.find(2250) == rangeMapping.end()) {
                if (rangeMapping[2250].find(rowobjects[i].Opening) == rangeMapping[2250].end()) {
                    rangeMapping[2250][rowobjects[i].Opening] = 1;
                }
            }
            else {
                rangeMapping[2250][rowobjects[i].Opening]++;
            }
        }

        if (rowobjects[i].WhiteElo > 2500 && rowobjects[i].WhiteElo <= 2750) {

            if (rangeMapping.find(2500) == rangeMapping.end()) {
                if (rangeMapping[2500].find(rowobjects[i].Opening) == rangeMapping[2500].end()) {
                    rangeMapping[2500][rowobjects[i].Opening] = 1;
                }
            }
            else {
                rangeMapping[2500][rowobjects[i].Opening]++;
            }
        }

        if (rowobjects[i].WhiteElo > 2750 && rowobjects[i].WhiteElo <= 3000) {

            if (rangeMapping.find(2750) == rangeMapping.end()) {
                if (rangeMapping[2750].find(rowobjects[i].Opening) == rangeMapping[2750].end()) {
                    rangeMapping[2750][rowobjects[i].Opening] = 1;
                }
            }
            else {
                rangeMapping[2750][rowobjects[i].Opening]++;
            }
        }

        if (rowobjects[i].WhiteElo > 3000 && rowobjects[i].WhiteElo <= 3250) {

            if (rangeMapping.find(3000) == rangeMapping.end()) {
                if (rangeMapping[3000].find(rowobjects[i].Opening) == rangeMapping[3000].end()) {
                    rangeMapping[3000][rowobjects[i].Opening] = 1;
                }
            }
            else {
                rangeMapping[3000][rowobjects[i].Opening]++;
            }
        }

        //<----------Mapping Probability of winmning while using the Opening.---------->
        ProbMapping[rowobjects[i].Opening].opening = rowobjects[i].Opening;

        if (rowobjects[i].Result == "Jan-00") {
            ProbMapping[rowobjects[i].Opening].wins++;
        }
        if (rowobjects[i].Result == "0-1") {
            ProbMapping[rowobjects[i].Opening].loss++;
        }
        if (rowobjects[i].Result == "1/2-1/2") {
            ProbMapping[rowobjects[i].Opening].draw++;
        }
        ProbMapping[rowobjects[i].Opening].winProbability = (double)ProbMapping[rowobjects[i].Opening].wins / ((double)ProbMapping[rowobjects[i].Opening].wins + (double)ProbMapping[rowobjects[i].Opening].loss + (double)ProbMapping[rowobjects[i].Opening].draw) * 100;
        ProbMapping[rowobjects[i].Opening].totalGamesPlayed = ProbMapping[rowobjects[i].Opening].wins + ProbMapping[rowobjects[i].Opening].loss + ProbMapping[rowobjects[i].Opening].draw;
   

        //<----------Mapping Opening Data---------->

        openingDataMapping[rowobjects[i].Opening].openingName = rowobjects[i].Opening;
        openingDataMapping[rowobjects[i].Opening].freqevents[rowobjects[i].Event]++;
       
        if (rowobjects[i].Result == "Jan-00") {
            openingDataMapping[rowobjects[i].Opening].wins++;
        }
        if (rowobjects[i].Result == "0-1") {
            openingDataMapping[rowobjects[i].Opening].loss++;
        }
        if (rowobjects[i].Result == "1/2-1/2") {
            openingDataMapping[rowobjects[i].Opening].draw++;
        }


        //<-----nigalaion-----

        if (rowobjects[i].Result != "1/2-1/2") {

            Xtemp.push_back(rowobjects[i].WhiteElo);
            Xtemp.push_back(rowobjects[i].BlackElo);
            X.push_back(Xtemp);
            Xtemp.clear();

            if (rowobjects[i].Result == "Jan-00")
                y.push_back(1);
            if (rowobjects[i].Result == "0-1")
                y.push_back(0);
        }


     }

     int option = 0;
     string player;
     string open;
     
     string s;
     

    while (1) {
        cout << "[1] Retrieve statistics for a player" << endl;
        cout << "[2] Retrieve statistics for an opening" << endl;
        cout << "[3] Display opening statistics according to Common ELO Ranking" << endl;
        cout << "[4] Display a probability of winning for each opening" << endl;
        cout << "[5] Logistic Regression" << endl;
        cout << "[6] Display Statistical Graphs" << endl;
        cout << "[7] View HighScore table" << endl;
        cout << "[8] Exit" << endl << endl << endl;

        cout << "Choose an option: ";
        cin >> option;
        if (option < 1 || option > 7) {
            cout << "Enter a valid option: ";
            continue;
        }
        if (option == 8)
            break;
          
        switch(option){

        case 1:  cout << "Search for a Player: ";
                 cin >> player;
                 getPlayersData(player, userMapping);
                 break;

        case 2: cout << "Search for an Opening: ";
                cin.ignore();
                cin.clear();
                fflush(stdin);
                getline(cin,open);
                getOpeningData(openingDataMapping, open);
                break;

        case 3: openingsCommonELO(rangeMapping, rowobjects);
                break;

        case 4:  probability(ProbMapping);
                 break;

        case 5: train(X, y, 10000, 0.1);
            /*  
            * # X --> Input.
            # y --> true/target value.
            # epochs --> Number of iterations.
            # lr --> Learning rate.
            */
                break;

        case 6: cout << "display LINKS i guess";
                break;

        case 7: mergeSort(rowobjects, 0, S-1);
                printsortedData(rowobjects);
                break;
                

        default: break;


     

        }
    }


    
    



    
    return 0;
     
}




void getPlayersData(string player,unordered_map<string, UserData>& userMapping) {

    int max = -9999;
    string ev;
    string opning;

    if (userMapping.find(player) == userMapping.end()) {
        cout << "Not found!" << endl << endl;;
        system("pause");
        system("CLS");
        return;
    }

    else {
        cout << "User-ID:   ";
        cout << userMapping[player].userId << endl;
        cout << "Wins:  ";
        cout << userMapping[player].wins << endl;
        cout << "Loss:  ";
        cout << userMapping[player].loss << endl;
        cout << "Draw:  ";
        cout << userMapping[player].draw << endl;
        cout << "No# of Times played as White:  ";
        cout << userMapping[player].playedAsWhite << endl;
        cout << "No# of Times played as Black:  ";
        cout << userMapping[player].playedAsBlack << endl;

        unordered_map<string, int>::iterator it = userMapping[player].eventsPlayed.begin();
        max = -9999;
 
        while (it != userMapping[player].eventsPlayed.end()) {
            if (it->second > max) {
                ev = it->first;
                max = it->second;
            }
            it++;
        }

        cout << "Most Frequent Event attended:  ";
        cout << ev << endl;

        unordered_map<string, int>::iterator it2 = userMapping[player].openings.begin();
        max = -9999;
        while (it2 != userMapping[player].openings.end()) {
            if (it2->second > max) {
                opning = it2->first;
                max = it2->second;
            }
            it2++;
        }

        cout << "Most Frequent Opening used:    ";
        cout << opning << endl;

        cout << "---------------------------------------------------------------------------------------------------------" << endl<<endl;

       
        system("pause");
        system("CLS");

        return;


    }




}

void openingsCommonELO(map<int, map<string, int>>& rangeMapping,vector<RowData>& rowobjects) {

    int LB = 0;
    int UB = 0;
    int i = 0;

    priority_queue<Openings> mostFrequent;
    Openings obj;
    map<int, pair<string,int>> mostfreqopn;

    

    for (auto ent1 : rangeMapping) {
        UB = ent1.first;
        cout<<"\t\t"<<"ELO RANGE:   " << LB << "-" << UB << "\t\t" << endl;
        for (auto ent2 : ent1.second) {
            cout<<"Opening: " << ent2.first << "    Frequency:  " << ent2.second << endl;

            obj.openings = ent2.first;
            obj.frequency = ent2.second;
            mostFrequent.push(obj);
            }

   cout << "---------------------------------------------------------------------------------------------------------" << endl;

        mostfreqopn[UB] = make_pair(mostFrequent.top().openings,mostFrequent.top().frequency);
        //cout << "\n\n\n\n" <<"MOST FREQUENT OPENING WITHIN RANGE: "<<LB<<"-"<<UB<< mostFrequent.top().openings << "\n\n\n\n";
        while (!mostFrequent.empty())
            mostFrequent.pop();

        LB = UB;
        cout << endl<<endl<<endl<<endl;
        }

    int lbb = 0;
    int ubb = 0;
    for (auto mfo : mostfreqopn) {
        ubb = mfo.first;
        cout << "Most Frequent Openings within ELO Range " << lbb << "-" << ubb << ":       " << mfo.second.first << "with freq =" << mfo.second.second << endl;;
        lbb = ubb;
    }

    cout << "---------------------------------------------------------------------------------------------------------" << endl<<endl;

    system("pause");
    system("CLS");

    return;

}

void probability(map<string, Probability>& ProbMapping) {
   
    cout << endl << endl;

    cout<<setw(50)<<left<< "Opening" <<left<<setw(15)<< "Wins" << left << setw(15) << "Loss" << left << setw(15) << "Draw" << left << setw(15) << "Probability(%)" << endl;
    for (auto x : ProbMapping) {
        if (!(x.second.winProbability== 100) && x.second.totalGamesPlayed > minGames) {
            cout << setw(50) << left<< x.first << left << setw(15) <<x.second.wins << left << setw(15) << x.second.loss << left << setw(15) << x.second.draw << left << setw(15) << x.second.winProbability <<endl;
        }
    }

    cout << "---------------------------------------------------------------------------------------------------------" << endl << endl;

    system("pause");
    system("CLS");

    return;

}

void getOpeningData(unordered_map<string, OpeningData>& DataMap, string open) {

    int max = -9999'9;
    string freqevent;

    if (DataMap.find(open) == DataMap.end()) {
        cout << "Not found!" << endl << endl;;
        system("pause");
        system("CLS");
        return;
    }

    else {
        unordered_map<string, int>::iterator it = DataMap[open].freqevents.begin();

        while (it != DataMap[open].freqevents.end()) {

            if (it->second > max) {
                freqevent = it->first;
                max = it->second;
            }
            it++;
        }

        cout << endl << endl;
        cout << "Opening Name:  ";
        cout << DataMap[open].openingName<<endl;

        cout << "Wins with this opening:    ";
        cout << DataMap[open].wins<<endl;

        cout << "Loss with this opening:    ";
        cout << DataMap[open].loss<<endl;

        cout << "Draw with this opening:    ";
        cout << DataMap[open].draw<<endl;

        
    }

    cout << "---------------------------------------------------------------------------------------------------------" << endl << endl;

    system("pause");
    system("CLS");

    return;


    return;
}


//DOT PRODUCT
double dotProduct(vector<int>& X, vector<double>& weights) {
    double result = 0;
    for (int i = 0; i < weights.size(); i++) {
        result = result + ((double)X[i] * weights[i]);
    }
    return result;
}


//z = h(theta' * x )
// x --> features
// theta --> weight vector
// sigmoid --> 1 / (1 + e^-z)
// result should be greater than zero and less than 1
// if z > 0 answer should be greater 0.5 
double sigmoid(double z) {
    return (1 / (1 + exp(-1 * z)));
}


//The goal is to minimize this cost function
double costFunction(vector<double>& y, vector<double>& y_hat) {
    double loss = 0;
    int m = y.size();
    for (int i = 0; i < y.size(); i++) {
        loss = loss + ((y[i] * log(y_hat[i])) + ((1 - y[i]) * log(1 - y_hat[i])));
    }
    loss = loss * -1;
    // cout << "LOSS  : ";
    //cout << loss << endl;
    loss = loss / m;
    return loss;
}

// Gradient Descent
// This is the most gayest approach we could think of atm 
// NEED IMMEDIATE REFACTOR
vector<double> weights = { 0.2, 0.5 };
double bias = 0.5;
vector<double> dw(2, 0);
double db;

void gradients(vector<vector<int>>& X, vector<double>& y, vector<double>& y_hat) {
    int m = X.size();
    // y_hat - y
    vector<int> diffYHat(y.size());
    for (int i = 0; i < y.size(); i++) {
        diffYHat[i] = y_hat[i] - y[i];
    }

    // dot product bitches
    // X.T * diff
    for (int j = 0; j < X[0].size(); j++) {
        for (int i = 0; i < diffYHat.size(); i++) {
            dw[j] = dw[j] + X[i][j] * diffYHat[i];
        }
        dw[j] = dw[j] / m;
    }

    for (int i = 0; i < m; i++) {
        db = db + diffYHat[i];
    }
    db = db / m;
}


//TODO
/*void normalize(vector<vector<int>>& X) {
    int m = X.size();
    int n = X[0].size();

    for (int i = 0; i < )
}
*/

void train(vector<vector<int>>& X, vector<double>& y, int epochs, double lr) {
    int m = X.size();
    int n = X[0].size();

    vector<double> dummyWeights = { 0.9, 0.5 };
    double dummyB = 0;
    vector<double> losses;

    for (int i = 0; i < epochs; i++) {
        vector<double> y_hat(y.size());
        for (int j = 0; j < y.size(); j++) {
            y_hat[j] = sigmoid(dotProduct(X[j], dummyWeights) + dummyB);
        }
        gradients(X, y, y_hat);
        for (int k = 0; k < dummyWeights.size(); k++) {
            dummyWeights[k] = dummyWeights[k] - lr * dw[k];
        }
        dummyB = dummyB - lr * db;
        vector<double> dummyHat(y.size());
        for (int k = 0; k < y.size(); k++) {
            dummyHat[k] = sigmoid(dotProduct(X[k], dummyWeights) + dummyB);
        }
        double l = costFunction(y, dummyHat);
        losses.push_back(l);
        //cout << "I :  " << i << endl;
    }
    cout << "LOSSES : " << endl;
    for (int i = 0; i < losses.size(); i++) {
        cout << losses[i] << endl;
    }
    cout << "DUMMY WEIGHTS : ";
    for (int i = 0; i < dummyWeights.size(); i++) {
        cout << dummyWeights[i] << endl;
    }
    cout << "BIAS : " << dummyB << endl;

}










void merge(vector<RowData>& array, int const left, int const mid, int const right)
{
    auto const subArrayOne = mid - left + 1;
    auto const subArrayTwo = right - mid;

    // Create temp arrays
    vector<RowData> leftArray(S); 
    vector<RowData> rightArray(S);

    // Copy data to temp arrays leftArray[] and rightArray[]
    for (auto i = 0; i < subArrayOne; i++)
        leftArray[i] = array[left + i];
    for (auto j = 0; j < subArrayTwo; j++)
        rightArray[j] = array[mid + 1 + j];

    auto indexOfSubArrayOne = 0, // Initial index of first sub-array
        indexOfSubArrayTwo = 0; // Initial index of second sub-array
    int indexOfMergedArray = left; // Initial index of merged array

    // Merge the temp arrays back into array[left..right]
    while (indexOfSubArrayOne < subArrayOne && indexOfSubArrayTwo < subArrayTwo) {
        if (leftArray[indexOfSubArrayOne].WhiteRatingDiff <= rightArray[indexOfSubArrayTwo].WhiteRatingDiff) {
            array[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
            indexOfSubArrayOne++;
        }
        else {
            array[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
            indexOfSubArrayTwo++;
        }
        indexOfMergedArray++;
    }
    // Copy the remaining elements of
    // left[], if there are any
    while (indexOfSubArrayOne < subArrayOne) {
        array[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
        indexOfSubArrayOne++;
        indexOfMergedArray++;
    }
    // Copy the remaining elements of
    // right[], if there are any
    while (indexOfSubArrayTwo < subArrayTwo) {
        array[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
        indexOfSubArrayTwo++;
        indexOfMergedArray++;
    }
}

// Change int to rowobject
void mergeSort(vector<RowData>& vec, int left, int right) {
    if (right > left) {
        int middle = left + (right - left) / 2;
        mergeSort(vec, left, middle);
        mergeSort(vec, middle + 1, right);
        merge(vec, left, middle, right);
    }
}


void printsortedData(vector<RowData>& rowobjects) {

    CSVReader reader("chessData.csv");

    
        cout << setw(20) << left << "White" << setw(15) << left << "WhiteElo" << setw(15) << left << "WhiteRatingDiff" << setw(17) << right << "Opening"<<setw(15) << left << endl;

    for (int i = 0; i < S; i++) {
        cout<< setw(20) << left
            << rowobjects[i].White << setw(15) << left
            << rowobjects[i].WhiteElo << setw(15) << left
            << rowobjects[i].WhiteRatingDiff << setw(15) << left
            << rowobjects[i].Opening << setw(15) << left
            << endl;
    }
}
