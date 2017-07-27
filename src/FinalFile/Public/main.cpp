#include <iostream>
#include <string>
#include <memory>

auto JonSnowDies(const std::string& content) -> bool {
    return content == "Jon Snow dies";
}

auto TyrionDies(const std::string& content) -> bool {
    return content == "Tyrion dies";
}

auto CerseiDies(const std::string& content) -> bool {
    return content == "Cersei dies";
}

struct Deleter {
    void operator()(FILE* f) const {
        std::cout << "Deleter is releasing a FILE\n";
        fclose(f);
    };
};

auto ReadFile(const std::string& fileName) -> void {
    auto file = std::unique_ptr<FILE, Deleter>(fopen(fileName.c_str(), "r"));
    if (file) {
        char line[256] = { '\0' };
        fread(line, 1, sizeof(line), file.get());

        if (JonSnowDies(line))
            throw "But... But... King in the North...";

        else if (TyrionDies(line))
            throw "I'm not watching this show anymore";

        else if (CerseiDies(line))
            throw "A party";

        else std::cout << "OK, now show me those dragons" << std::endl;
    }
}

auto main() -> int {
    try {
        ReadFile("..\\..\\src\\ReadingFile\\Public\\GameOfThrones.txt");
        ReadFile("..\\..\\src\\ReadingFile\\Public\\GameOfThronesYAY.txt");
    } catch (...) {
        std::cout << "Don't worry, I GOT this" << std::endl;
    }

    return 0;
}