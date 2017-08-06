#include <iostream>
#include <fstream>
#include <string>

auto JonSnowDies(const std::string& content) -> bool {
    return content == "Jon Snow dies";
}

auto TyrionDies(const std::string& content) -> bool {
    return content == "Tyrion dies";
}

auto CerseiDies(const std::string& content) -> bool {
    return content == "Cersei dies";
}

class File {
public:
    File() {  }

    auto IsOpen() -> bool { return false; }

    auto ReadLine() -> std::string {
		return "";
    }

    ~File() { }

private:
};

auto ReadFile(const std::string& fileName) -> void {
    char line[256] = { '\0' };
	auto file = File();
    if (file.IsOpen()) {
        auto line = file.ReadLine();

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