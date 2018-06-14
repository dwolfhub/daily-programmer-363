#include <fstream>
#include <vector>
#include <cassert>

using namespace std;

enum direction
{
    clockwise = 0,
    counter_clockwise = 1
};

const vector<string> explode(const string &s, const char &c)
{
    string buff{""};
    vector<string> v;

    for (char n : s)
    {
        if (n != c)
        {
            buff += n;
        }
        else if (n == c && buff != "")
        {
            v.push_back(buff);
            buff = "";
        }
    }

    if (buff != "")
    {
        v.push_back(buff);
    }

    return v;
}

string convert_to_only_uppercase_letters(string text)
{
    int c_asci,
        text_len = text.length();

    for (int i = text_len - 1; i >= 0; i--)
    {
        c_asci = (int)text[i];
        if (c_asci < 65 || c_asci > 122 || (c_asci > 90 && c_asci < 97)) // not a letter
            text.erase(i, 1);
        else if (c_asci >= 97 && c_asci <= 122) // uppercase all
            text[i] = (char)(c_asci - 32);
    }

    return text;
}

vector<string> text_to_grid(string text, int cols, int rows)
{
    vector<string> grid;
    int i = 0;
    int text_len = text.length();

    while (text_len >= cols)
    {
        grid.push_back(text.substr(i * cols, cols));

        i++;
        text_len -= cols;
    }

    if (i * cols < text.length())
    {
        grid.push_back(text.substr(i * cols));
        grid.back().append(cols - grid.back().length(), 'X');
    }

    return grid;
}

void turn(int &xdelta, int &ydelta, direction dir)
{
    if (dir == direction::clockwise)
    {
        if (ydelta == 0)
        {
            if (xdelta < 0)
            {
                ydelta = -1;
            }
            else
            {
                ydelta = 1;
            }
            xdelta = 0;
        }
        else
        {
            if (ydelta < 0)
            {
                xdelta = 1;
            }
            else
            {
                xdelta = -1;
            }
            ydelta = 0;
        }
    }
    else
    {
        if (ydelta == 0)
        {
            if (xdelta < 0)
            {
                ydelta = 1;
            }
            else
            {
                ydelta = -1;
            }
            xdelta = 0;
        }
        else
        {
            if (ydelta < 0)
            {
                xdelta = -1;
            }
            else
            {
                xdelta = 1;
            }
            ydelta = 0;
        }
    }
}

string use_spiral_cipher(vector<string> grid, direction dir)
{
    string buff{""};
    int rows = grid.size(),
        cols = grid.front().length(),
        x = cols - 1,
        y = 0,
        xdelta = dir == direction::clockwise ? 0 : -1,
        ydelta = dir == direction::clockwise ? 1 : 0,
        nextx,
        nexty,
        total_len = rows * cols;

    while (total_len--)
    {
        buff.append(1, grid[y][x]);
        grid[y][x] = '-';

        nextx = x + xdelta;
        nexty = y + ydelta;

        if (nextx >= cols ||
            nexty >= rows ||
            nextx < 0 ||
            nexty < 0 ||
            grid[nexty][nextx] == '-')
        {
            turn(xdelta, ydelta, dir);
        }

        x += xdelta;
        y += ydelta;
    }

    return buff;
}

string encrypt(string text, int cols, int rows, direction dir)
{
    text = convert_to_only_uppercase_letters(text);
    vector<string> grid = text_to_grid(text, cols, rows);
    text = use_spiral_cipher(grid, dir);

    return text;
}

int main()
{
    ifstream ifile;

    ifile.open("input.txt");

    if (ifile.is_open())
    {
        string line;
        string dims_str;
        vector<string> line_parts;
        vector<string> dims;
        direction dir;
        string encrypted;

        while (getline(ifile, line))
        {
            line_parts = explode(line, '\t');
            dims_str = line_parts[1];

            dims = explode(dims_str.substr(1, dims_str.length() - 2), ',');

            dir = (line_parts[2] == "clockwise") ? direction::clockwise : direction::counter_clockwise;

            encrypted = encrypt(line_parts[0], stoi(dims[0]), stoi(dims[1]), dir);

            assert(encrypted == line_parts[3]);
        }
    }

    return 0;
}
