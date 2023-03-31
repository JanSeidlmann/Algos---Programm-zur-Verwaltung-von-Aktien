#include <iostream>
#include <string>
#include <array>
#include <fstream>
#include <sstream>
#include <cmath>
#include <tuple>

using namespace std;

class Stock {
public:
    string name;
    string short_name;
    string wkn;
    string file;
    //vector<vector<string>> stockData;

    Stock(string n, string s, string k, string f) : name(n), short_name(s), wkn(k), file(f) {};
};

class HashTable {
private:
    array<tuple<string, string, string, string>, 2000> table;
    int hash(string key) {
        int hashVal = 0;
        int n = key.length();
        for (int i = 0; i < n; i++) {
            hashVal += key[i] * pow(7, (n - 1 - i));
        }
        int index = hashVal % 2003;

        return index;
    }
public:
    int sorting(string key){
        int index = hash(key);
        int i = 0;
        while (get<2>(table[index]) != "") {
            if(i == 2000){
                return 2420;
            }
            index = (index + i * i) % 2000;
            i++;
        }
        return index;
    }

    void add(Stock* stock, string key) {
        int index = sorting(key);
        if(index == 2420){
            cout<<"Table is full. No stock can be added anymore.";
        }
        else
            table[index] = make_tuple(stock->name, stock->short_name, stock->wkn, stock->file);
    }

    tuple<string, string, string, string> search(string key, int method) {
        int index = hash(key);

        for (int i = 0; i < 2000; i++) {
            if (method == 0 && key == get<0>(table[index])) {
                return table[index];
            }
            if (method == 1 && key == get<1>(table[index])) {
                return table[index];
            }
            index = (index + i * i) % 2000;
        }
        return make_tuple("", "", "", ""); // Return empty tuple if Stock object not found
    }

    int import(string csv, string name, int method) {
        //check if Stock exists in table
        std::tuple<string, string, string, string>nameS = search(name, method);
        if(nameS == std::make_tuple("", "", "", "")){
            return 1;
        }
        //check if file exists
        fstream file (csv, ios::in);
        if(!file.is_open())
            return 2;
        //add file, if both stock and file exist
        int index = hash(name);
        auto myTuple = table[index];
        get<3>(myTuple) = csv;
        table[index] = myTuple;
        cout<<get<3>(table[index]);
        return 0;
    }

    vector<string> closeList(string fileCSV){
        std::ifstream file(fileCSV);

        // Select the column to read
        int column = 4;  // Index of the column to read (0-based)

        // Read the CSV file line by line
        std::string line;
        std::vector<string> column_data;
        while (std::getline(file, line)) {
            // Split the line into fields
            std::istringstream iss(line);
            std::string field;
            int field_count = 0;
            while (std::getline(iss, field, ',')) {
                if (field_count == column) {
                    // Convert the field to a double and add it to the array

                    column_data.push_back(field);
                }
                ++field_count;
            }
        }
        return column_data;
    }
    void saveFile(int method) {
        string fileName;
        if(method == 0)
            fileName = "hash_table_stockName.txt";
        else if(method == 1)
            fileName = "hash_table_stockShort.txt";

        ofstream file(fileName);

        for (int i = 0; i < 2000; i++) {
            if(get<0>(table[i]) != "")
                file << get<0>(table[i]) << "," << get<1>(table[i]) << "," << get<2>(table[i])<< "," << get<3>(table[i]) << endl;
        }

        file.close();
    }
    int delStock(string name, int method) {
        int index = hash(name);

        switch (method) {
                case 0:
                    key = name;
                    break;
                case 1:
                    key = short_name;
                    break;
            }
        table[index] = make_tuple("delete", "delete", "delete", "delete");
        return 0;
    }

    void loadFile(const string filename, int method) {
        string key;

        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Error opening file." << endl;
            return;
        }

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string name, short_name, wkn, fileCSV;
            getline(ss, name, ',');
            getline(ss, short_name, ',');
            getline(ss, wkn, ',');
            getline(ss, fileCSV, ',');
            auto* stock = new Stock(name, short_name, wkn, fileCSV);
            switch (method) {
                case 0:
                    key == name;
                case 1:
                    key == short_name;
            }
            add(stock, key);
        }

        file.close();
    }

};
