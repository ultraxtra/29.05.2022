
#include <boost/filesystem/operations.hpp>
#include <iostream>
#include <iomanip>
#include <string>
#include <Windows.h>
#include <conio.h>
#include <stdio.h>

using namespace std;
using namespace boost;


uint32_t SizeDir(filesystem::path dir)
{
    size_t size = 0;
    for (filesystem::recursive_directory_iterator it(dir);
        it != filesystem::recursive_directory_iterator();
        ++it)
    {
        if (!filesystem::is_directory(*it))
            size += filesystem::file_size(*it);
    }
    return size;
}

struct HumanReadable {
    std::uintmax_t size{};
private:
    friend std::ostream& operator<<(std::ostream& os, HumanReadable hr) {
        int i{};
        double mantissa = hr.size;
        for (; mantissa >= 1024.; mantissa /= 1024., ++i) {}
        mantissa = std::ceil(mantissa * 10.) / 10.;
        os << mantissa << " " << "BKMGTPE"[i];
        return i == 0 ? os : os << "B ";
    }
    friend std::ostream& operator<<(std::ostream& os, uint32_t size) {
        int i{};
        double mantissa = size;
        for (; mantissa >= 1024.; mantissa /= 1024., ++i) {}
        mantissa = std::ceil(mantissa * 10.) / 10.;
        os << mantissa << " " << "BKMGTPE"[i];
        return i == 0 ? os : os << "B ";
    }
};

void CreateDir(string path, string namedir)
{
    boost::system::error_code error;
    boost::filesystem::create_directories(path + '/' + namedir, error);
    assert(!error);
}

void RemoveDir(string path, string namedir)
{
    try
    {
        if (boost::filesystem::exists(path + '/' + namedir))
        {
            boost::filesystem::remove_all(path + '/' + namedir);
        }
    }
    catch (boost::filesystem::filesystem_error const& e)
    {
        cout << e.what() << endl;
        std::system("pause");
    }
}

void RenameDir(string path)
{
    string old_name, new_name;
    cout << "Enter old name dir: ";
    getline(cin, old_name);
    cout << "Enter new name dir: ";
    getline(cin, new_name);
    try
    {
        if (boost::filesystem::exists(path + '/' + old_name))
        {
            boost::filesystem::rename(path + '/' + old_name, path + '/' + new_name);
        }
    }
    catch (boost::filesystem::filesystem_error const& e)
    {
        cout << e.what() << endl;
        std::system("pause");
    }
}

void PrintDir(string path)
{
    filesystem::directory_iterator begin(path);
    filesystem::directory_iterator end;
    filesystem::path file;
    cout << "\t" << left << setfill(' ') << setw(45) << "Filename" << "Size" << right << setfill(' ') << setw(50) << "Type" << endl;
    for (; begin != end; ++begin)
    {
        filesystem::file_status fs = boost::filesystem::status(*begin);
        file = *begin;
        cout << left << setfill('-') << setw(50) << file.filename().string();
        switch (fs.type()) {
        case filesystem::regular_file:
            std::cout << HumanReadable{ boost::filesystem::file_size(*begin) } << right << setfill('-') << setw(45) << "FILE" << endl;
            break;
        case filesystem::symlink_file:
            std::cout << HumanReadable{ SizeDir(*begin) } << right << setfill('-') << setw(45) << "SYMLINK" << endl;
            break;
        case filesystem::directory_file:
            std::cout << HumanReadable{ SizeDir(*begin) } << right << setfill('-') << setw(45) << "DIRECTORY" << endl;
            break;
        default:
            std::cout << HumanReadable{ SizeDir(*begin) } << right << setfill('-') << setw(45) << "OTHER" << endl;
            break;
        }
    }
}

void CreateFile() {
    ofstream f;
    f.open(path + "/" + file, ios::out);
    if (!f.is_open()) 
    else {
        f << content;
        cout << file << "file created" << path << endl;
    }
    f.close();
}
void DeleteFile() {
    string pathFile = path + "/" + file;
    char* char_arr;
    char_arr = &pathFile[0];
    if (remove(char_arr) != 0) {
        cout << "file not found " << file << "\n";
    }
    else
        cout << file << "file delete\n";
}


int main()
{

    std::system("color F0");

    string path;
    string namedir;
    char switch_on;
    cout << "Enter path: ";
    getline(cin, path);
    do
    {
        std::system("cls");
        PrintDir(path);
        cout << "1 - Create dir" << endl;
        cout << "2 - Remove dir" << endl;
        cout << "3 - Rename dir" << endl;
        cout << "4 - Open dir" << endl;
        cout << "5 - Back" << endl;
        cout << "Enter: ";
        cin >> switch_on;
        switch (switch_on)
        {
        case '1':
        {
            cin.ignore();
            cout << "Enter name of new dir: ";
            getline(cin, namedir);
            CreateDir(path, namedir);
        }break;
        case '2':
        {
            cin.ignore();
            cout << "Enter name dir for remove: ";
            getline(cin, namedir);
            RemoveDir(path, namedir);
        }break;
        case '3':
        {
            cin.ignore();
            RenameDir(path);
        }break;
        case '4':
        {
            cin.ignore();
            cout << "Enter name of dir: ";
            getline(cin, namedir);
            filesystem::current_path(path + '/' + namedir);
            path = filesystem::current_path().string();
        }break;
        case '5':
        {
            filesystem::path p = filesystem::current_path();
            path = p.parent_path().string();
            filesystem::current_path(path);
        }break;
        case '6':
        {
            CreateFile();
        }break;
        case '7':
        {
            DeleteFile();
        }break
        }
    } while (true);

    return 0;

}