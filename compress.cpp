#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

using namespace std;

class LZWTrieCompressor {
public:
    LZWTrieCompressor() {
        resetDictionary();
    }

    vector<int> compress(const string& data) {
        resetDictionary();
        vector<int> compressed;
        string w;

        for (char c : data) {
            string wc = w + c;
            if (trieContains(wc)) {
                w = wc;
            } else {
                compressed.push_back(getCode(w));
                addToTrie(wc);
                w = string(1, c);
            }
        }

        if (!w.empty()) {
            compressed.push_back(getCode(w));
        }

        return compressed;
    }

    string decompress(const vector<int>& compressed) {
        unordered_map<int, string> dictionary;
        for (int i = 0; i < 256; i++) {
            dictionary[i] = string(1, static_cast<char>(i));
        }

        string decompressed;
        string w = dictionary[compressed[0]];
        decompressed += w;

        int next_code = 256;
        for (size_t i = 1; i < compressed.size(); i++) {
            int code = compressed[i];
            string entry;

            if (dictionary.count(code)) {
                entry = dictionary[code];
            } else if (code == next_code) {
                entry = w + w[0];
            } else {
                throw runtime_error("Invalid compressed code");
            }

            decompressed += entry;
            dictionary[next_code] = w + entry[0];
            next_code++;
            w = entry;
        }

        return decompressed;
    }

private:
    struct TrieNode {
        int code;
        unordered_map<char, unique_ptr<TrieNode>> children;
        TrieNode() : code(-1) {}
    };

    unique_ptr<TrieNode> root;
    int next_code;

    void resetDictionary() {
        root = make_unique<TrieNode>();
        next_code = 0;
        for (int i = 0; i < 256; i++) {
            addToTrie(string(1, static_cast<char>(i)));
        }
    }

    bool trieContains(const string& s) {
        TrieNode* current = root.get();
        for (char c : s) {
            if (current->children.find(c) == current->children.end()) {
                return false;
            }
            current = current->children[c].get();
        }
        return true;
    }

    int getCode(const string& s) {
        TrieNode* current = root.get();
        for (size_t i = 0; i < s.length(); i++) {
            char c = s[i];
            if (current->children.find(c) == current->children.end()) {
                throw runtime_error("String not in trie");
            }
            if (i == s.length() - 1) {
                return current->children[c]->code;
            }
            current = current->children[c].get();
        }
        throw runtime_error("Empty string");
    }

    void addToTrie(const string& s) {
        TrieNode* current = root.get();
        for (size_t i = 0; i < s.length(); i++) {
            char c = s[i];
            if (current->children.find(c) == current->children.end()) {
                current->children[c] = make_unique<TrieNode>();
            }
            if (i == s.length() - 1) {
                current->children[c]->code = next_code++;
            }
            current = current->children[c].get();
        }
    }
};

int main() {
    LZWTrieCompressor lzw;

    string original = "TOBEORNOTTOBEORTOBEORNOThgfdtr7iykigfdxzykuymugfd86oiuygfdzr7uikjygfdx8zxlilkbvk";
    cout << "Original: " << original << endl;

    vector<int> compressed = lzw.compress(original);
    cout << "Compressed: ";
    for (int code : compressed) {
        cout << code << " ";
    }
    cout << endl;

    string decompressed = lzw.decompress(compressed);
    cout << "Decompressed: " << decompressed << endl;

    cout << "Original == Decompressed: " 
         << (original == decompressed ? "true" : "false") 
         << endl;

    return 0;
}