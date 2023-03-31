#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "HashTable.h"


using namespace std;

void ADD(HashTable* nameTable, HashTable* shortTable){
    string nameS, shortS, wknS;

    cout<<"What is the name of the stock: ";
    cin>> nameS;
    cout<<endl;
    cout<<"Was ist der Kuerzel: ";
    cin>> shortS;
    cout<<endl;
    cout<<"Was ist die WKN: ";
    cin>> wknS;
    cout<<endl;

    Stock *stock1;
    stock1 = new Stock(nameS, shortS, wknS, " ");
    nameTable->add(stock1, nameS);
    shortTable->add(stock1, shortS);
}

void SEARCH(HashTable* nameTable, HashTable* shortTable){
    int inputSearch;
    string key;
    std::tuple<string, string, string, string> result;
    int correct = 1;

    while(correct){
        cout<<"Nach welcher Aktie suchen Sie (geben Sie 1 ein um nach Namen zu suchen; 2 um nach Kuerzel zu suchen:";
        cin>>inputSearch;
        cout<<endl;
        switch (inputSearch) {
            case 1:
                cout<<"Was ist der Name der Aktie: ";
                cin>>key;
                cout<<endl;
                result = nameTable->search(key, 0);
                correct = 0;
                break;
            case 2:
                cout<<"Was ist der Kuerzel der Aktie: ";
                cin>>key;
                cout<<endl;
                result = shortTable->search(key, 1);
                correct = 0;
                break;
            default:
                cout<<"Eingabe ist nicht korrekt, probieren Sie nochmal.";
                cout<<endl;
                break;
        }
    }

std::vector<std::string> lastRowValues;
    std::ifstream csvFile(get<3>(result));

    std::string line;
    while (std::getline(csvFile, line)) {
        std::vector<std::string> values;
        std::string value;
        std::istringstream ss(line);
        while (std::getline(ss, value, ',')) {
            values.push_back(value);
        }
        lastRowValues = values;
    }

    csvFile.close();



    if (result == std::make_tuple("", "", "", "")) {
        std::cout << "Stock not found" << std::endl;
    } else {
        // Access the returned values using std::get
        string name = std::get<0>(result);
        string short_name = std::get<1>(result);
        string wkn = std::get<2>(result);

        // Print the returned values
        std::cout << "Name: " << name << std::endl;
        std::cout << "Short name: " << short_name << std::endl;
        std::cout << "WKN: " << wkn << std::endl;
        for (const auto& value : lastRowValues) {
        std::cout << value << " ";
    }
    std::cout << std::endl;
    }
}

void IMPORT(HashTable* nameTable, HashTable* shortTable){
    string csv, nameS, short_name;
    int check;
    tuple<string, string, string, string> stock;


    cout<<"What is the name of the file, wich you want to import: ";
    cin>>csv;
    cout<<endl;
    cout<<"Input the name of the stock wich you want to import the file to: ";
    cin>>nameS;
    cout<<endl;

    check = nameTable->import(csv, nameS, 0);
    switch (check) {
        case 0:
            stock = nameTable->search(nameS, 0);
            short_name = get<1>(stock);
            shortTable->import(csv, short_name, 1);
            cout<<"The csv File was succesfully uploaded"<<endl;
            break;
        case 1:
            cout<<"The stock does not exist. Please first add a stock and then import the file."<<endl;
            break;
        case 2:
            cout<<"The csv File does not exist. Please first upload the file and then try then import again."<<endl;
            break;
    }
}

void PLOT(HashTable* nameTable){
    string stockName;

    cout<<"Input the name of the stock you want to view:";
    cin>>stockName;
    tuple<string, string, string, string> stock = nameTable->search(stockName, 0);
    string fileCSV = get<3>(stock);

    // Check if the file was opened successfully
    std::ifstream file(fileCSV);
    if (!file.is_open()) {
        std::cout << "Failed to open file." << std::endl;
    }

    vector<string> closeValues = nameTable->closeList(fileCSV);

    // Print the data in the array
    /*for (auto value : closeValues) {
        std::cout << value << " ";
    }
    std::cout << std::endl;*/
}

void DELETE(HashTable* nameTable, HashTable* shortTable){
    string stockName;

    cout<<"Type in the name of the stock you want to delete: ";
    cin>>stockName;
    tuple<string, string, string, string> stock = nameTable->search(stockName, 0);
    string short_name = get<1>(stock);
    int result = nameTable->delStock(stockName, 0);
    int tmp = shortTable->delStock(short_name, 1);
    switch (result) {
        case 0:
            cout<<"Stock has been successfully deleted.";
            break;
        case 1:
            cout<<"Stock does not exist.";
            break;
    }
}

void LOAD(HashTable* nameTable, HashTable* shortTable){
    string filename;

    cout<<"Laden Sie die hashtabelle mit namen als erstes hoch:";
    cin>>filename;
    nameTable->loadFile(filename, 0);
    cout<<endl;

    cout<<"Laden Sie die hashtabelle mit namen als erstes hoch:";
    cin>>filename;
    shortTable->loadFile(filename, 1);
    cout<<endl;

}

int main() {
    HashTable nameTable;
    HashTable shortTable;
    int input, status = 0;

    while(!status){
        input = 1;
        cout<<endl<<"1:Add, 2: Delete; 3: Import, 4: Search, 5: Plot, 6: Save File, 7: Load File, 8: Quit Programm.";
        cout<<endl<<"Tippen Sie die Zahl von einer der Auswahlmoeglichkeiten von oben ein: ";
        cin>>input;
        cout<<endl;


        switch (input) {
            case 1:
                ADD(&nameTable, &shortTable);
                break;
            case 2:
                DELETE(&nameTable, &shortTable);
                break;
            case 3:
                IMPORT(&nameTable, &shortTable);
                break;
            case 4:
                SEARCH(&nameTable, &shortTable);
                break;
            case 5:
                PLOT(&nameTable);
                break;
            case 6:
                nameTable.saveFile(0);
                shortTable.saveFile(1);
                cout<<"Hashtable successfully saved.";
                break;
            case 7:
                LOAD(&nameTable, &shortTable);
                break;
            case 8:
                cout<<"The program will be closed.";
                status = 1;
                break;
            default:
                cout<<"Invalid input. Try again.";
                break;
        }
    }


    return 0;
}
