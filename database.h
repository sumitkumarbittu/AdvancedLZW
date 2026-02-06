#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>
#include <iomanip>
#include <ctime>
#include <iostream>

using namespace std;

class CompressionDatabase {
private:
    bool dbOpen;
    vector<string> history;

public:
    CompressionDatabase(const string& dbName) : dbOpen(true) {
        cout << "Database stub initialized (SQLite3 not available).\n";
    }

    ~CompressionDatabase() {
    }

    bool isOpen() const {
        return dbOpen;
    }

    void saveCompressionStats(const string& originalFile, 
                            const string& compressedFile,
                            size_t originalSize,
                            size_t compressedSize,
                            double ratio,
                            double spaceSaving) {
        if (!dbOpen) return;
        
        string entry = originalFile + " -> " + compressedFile + 
                      " (Ratio: " + to_string(ratio) + ", Savings: " + 
                      to_string(spaceSaving * 100) + "%)";
        history.push_back(entry);
    }

    void displayHistory() {
        if (!dbOpen) return;

        cout << "\nCompression History:\n";
        cout << "========================================================================================================\n";
        
        if (history.empty()) {
            cout << "No compression history available.\n";
        } else {
            for (size_t i = 0; i < history.size(); ++i) {
                cout << (i + 1) << ". " << history[i] << "\n";
            }
        }
        
        cout << "========================================================================================================\n";
    }

    void clearHistory() {
        if (!dbOpen) return;
        history.clear();
        cout << "Compression history cleared successfully.\n";
    }
};

#endif // DATABASE_H