#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <bitset>
#if defined(__linux__)
#define CLEAR "clear"
#else
#define CLEAR "cls"
#endif

enum option
{
    FILE_ = 2,
    CONSOLE = 3,
    EXIT = 9
};

enum result
{
    SUCCESS = 0,
    ERROR = 1
};


short string_analysis(std::string &main_string)
{
    if(main_string=="exit") return EXIT;
    if(main_string=="-") return CONSOLE;
    int i = 0;
    if((i = main_string.find(' '))!=std::string::npos){
        if((main_string[i+1]!='\0' && main_string[i+1]!='\n')&&(i!=0)) return FILE_;
    }
    return ERROR;
}

void get_names(std::string &main_string,std::string &input_string,std::string &output_string)
{
    int delim = 0;
    delim = main_string.find(' ');
    for(int i = 0; i<delim;i++){
        input_string.push_back(main_string[i]);
    }
    while(main_string[delim]==' ') delim++;
    for(int i = delim; i<main_string.size();i++){
        output_string.push_back(main_string[i]);
    }
}

short input_func(std::string &filename, std::vector<char> &string)
{
    std::ifstream fin;
    fin.open(filename);
    if(!fin) return ERROR;
    else {
        int symbol = 0;
        while ((symbol = fin.get()) != EOF) string.push_back((char) symbol);
        fin.close();
        return SUCCESS;
    }
}

void input_func(std::istream &stream, std::vector<char> &string)
{
    std::cout << '>';
    int symbol = 0;
    while((symbol = stream.get())!='\n') string.push_back((char)symbol);
}

short output_func(std::string &filename, std::vector<unsigned char> &string)
{
    std::ofstream fout;
    fout.open(filename);
    if(!fout) return ERROR;
    else {
        for (char i: string) fout << i;
        fout.close();
        return SUCCESS;
    }
}

void output_func(std::ostream &stream, std::vector<unsigned char> &string)
{
    stream << "bin::"; 
    for(unsigned char i : string) stream << std::bitset<8>(i);
    std::cout <<"\nPress enter to continue...\n";
    std::cin.get();
}

void howTo()
{
    std::cout
        << R"(To encode a file, use "infile_name.format" "outfile_name.format")" << "\n"
        << R"(To encode a std input stream content, use "-")" << "\n"
        << R"(To exit, use "exit")" << "\n";
}

void encoding(std::vector<char> &input,std::vector<unsigned char> &output)
{
    char header = 0x04;
    output.push_back(header);
    if(input.size()<128){
        output.push_back((char)input.size());
        for(char & i : input)
            output.push_back(i);
    }
    else{
        unsigned int size_len = input.size();
        unsigned char second_otcet = 128;
        output.push_back(second_otcet);
        while(size_len!=0){
            output[1]++;
            output.push_back(size_len%256);
            size_len/=256;
        }
        unsigned char buf;
        int N = (output[1] - 128);
        for(int i = 0; i<N/2;i++){
            buf = output[i+2];
            output[i + 2] = output[N-i-1 + 2];
            output[N-i-1 + 2] = buf;
        }
        for(char & i : input)
            output.push_back(i);
    }
}

int main() {
    std::string main_srting, input_name, output_name;
    std::vector<char> input;std::vector<unsigned char> output;
    howTo();
    while(true) {
        std::cout << '>';
        std::getline(std::cin,main_srting,'\n');
        if (string_analysis(main_srting) == FILE_) {
            get_names(main_srting,input_name,output_name);
            if(input_func(input_name,input)==ERROR){
                system(CLEAR);
                std::cout << "ERROR!\n\n";
                howTo();
                main_srting.clear();
                continue;
            }
            encoding(input,output);
            if(output_func(output_name,output)==ERROR){
                system(CLEAR);
                std::cout << "ERROR!\n\n";
                howTo();
                main_srting.clear();
                continue;
            }
            system(CLEAR);
            main_srting.clear();
            continue;
        } else if (string_analysis(main_srting) == CONSOLE) {
            input_func(std::cin,input);
            encoding(input,output);
            output_func(std::cout,output);
            system(CLEAR);
            main_srting.clear();
            continue;
        } else if (string_analysis(main_srting) == EXIT) {
            main_srting.clear();
            return EXIT_SUCCESS;
        } else if (string_analysis(main_srting) == ERROR) {
            system(CLEAR);
            std::cout << "ERROR!\n\n";
            howTo();
            main_srting.clear();
            continue;
        }
    }
}
