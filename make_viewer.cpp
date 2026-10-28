#include "viewer.h"

int main(int argc, char* argv[]) {
    string csv = argc > 1 ? argv[1] : "sim_results.csv";
    string out = argc > 2 ? argv[2] : "viewer.html";
    ViewerWriter writer(csv, "viewer_template.html", out);
    return writer.write() ? 0 : 1;
}
