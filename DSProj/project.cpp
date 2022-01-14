#include <iostream>
#include "csv.hpp"
#include <map>
#include <unordered_map>

using namespace std;
using namespace csv;

const int S = 5000;



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



void getPlayersData(vector<RowData>& rowobjects, string player,unordered_map<string, UserData>& userMapping);
void openingsCommonELO(map<int, map<string, int>>& rangeMapping, vector<RowData>& rowobjects);

int main()
{
    CSVReader reader("chessData.csv");
    vector<RowData> rowobjects(S);
    unordered_map<string, UserData> userMapping;
    map<int, map<string, int>> rangeMapping;
    int i = 0;
    int max = -9999;

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
        catch(exception e){
            //cout << i << rowobjects[i].Event << rowobjects[i].White << endl;
            i--;
            //std::cerr << e.what();
        }

    
        i++;
    } //Saving Row data in class ROWDATA

    for (auto str : reader.get_col_names())
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
    }


    for (int i = 0; i < rowobjects.size(); i++) {

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
    }

   


    cout << "Search for a Player: ";
    string player;
    cin >> player;
    getPlayersData(rowobjects,player,userMapping);

    cout << endl << endl << endl << endl;

    openingsCommonELO(rangeMapping, rowobjects);
   
    

   
}


void getPlayersData(vector<RowData>& rowobjects,string player,unordered_map<string, UserData>& userMapping) {

    int max = -9999;
    string ev;
    string opning;

    if (userMapping.find(player) == userMapping.end()) {
        cout << "Not found!" << endl;
    }

    else {
        cout << "User-ID: ";
        cout << userMapping[player].userId << endl;
        cout << "Wins: ";
        cout << userMapping[player].wins << endl;
        cout << "Loss: ";
        cout << userMapping[player].loss << endl;
        cout << "Draw: ";
        cout << userMapping[player].draw << endl;
        cout << "No# of Times played as White: ";
        cout << userMapping[player].playedAsWhite << endl;
        cout << "No# of Times played as Black: ";
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

        cout << "Most Frequent Event attended: ";
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

        cout << "Most Frequent Opening used: ";
        cout << opning << endl;





    }


}

void openingsCommonELO(map<int, map<string, int>>& rangeMapping,vector<RowData>& rowobjects) {

    int LB = 0;
    int UB = 0;

    for (auto ent1 : rangeMapping) {
        UB = ent1.first;
        cout << "Most used opening in ELO Range: " << LB << "-" << UB << endl;
        for (auto ent2 : ent1.second) {
            cout << ent2.first << ":" << ent2.second << endl;
            }
        LB = UB;
        cout << endl<<endl;
        }

}