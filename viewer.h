#pragma once
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

// Builds a self-contained viewer page from a results CSV.
// It reads the CSV and a template page, puts the CSV text into the template
// at the __CSV__ marker, and writes the result as a new HTML file.
class ViewerWriter {
    string csvPath;
    string templatePath;
    string outPath;

    static bool readAll(const string& path, string& text) {
        ifstream in(path);
        if (!in.is_open()) return false;
        stringstream ss;
        ss << in.rdbuf();
        text = ss.str();
        return true;
    }

public:
    ViewerWriter(string csv = "sim_results.csv",
                 string tmpl = "viewer_template.html",
                 string out = "viewer.html")
        : csvPath(csv), templatePath(tmpl), outPath(out) {}

    bool write() {
        string csv, page;
        if (!readAll(csvPath, csv)) {
            cout << "Could not open " << csvPath << " (run the simulation first)" << endl;
            return false;
        }
        if (!readAll(templatePath, page)) {
            cout << "Could not open " << templatePath << " (keep it next to this program)" << endl;
            return false;
        }
        const string marker = "__CSV__";
        size_t pos = page.find(marker);
        if (pos == string::npos) {
            cout << "Template has no " << marker << " marker" << endl;
            return false;
        }
        page.replace(pos, marker.size(), csv);

        ofstream out(outPath);
        if (!out.is_open()) {
            cout << "Could not write " << outPath << endl;
            return false;
        }
        out << page;
        cout << "Wrote " << outPath << " - open it in your browser" << endl;
        return true;
    }
};
