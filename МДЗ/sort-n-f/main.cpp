#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <clocale>

namespace {

    bool ignore_case_compare(std::string s1, std::string s2)
    {
        std::locale s("en_US.UTF-8");
        for (auto & i : s1)
            i = std::tolower(i);
        for (auto & i : s2)
            i = std::tolower(i);
        return s(s1, s2);
    }

    bool is_digit(std::string s)
    {
        if (s.empty())
            return true;
        for (size_t i = 0; i < s.size(); i++)
            if (!std::isdigit(s[i]) && (i != 0 || s[i] != '-'))
                return false;
        return true;
    }

    std::string numeric_value(std::string s)
    {
        if (s.empty())
            return s;
        size_t j = 0, n = s.size() - 1;
        for (size_t i = 0; i < s.size(); i++)
            if (s[i] != ' ')
            {
                j = i;
                break;
            }
        for (size_t i = s.size(); i > 0; i++)
            if (s[i - 1] != ' ')
            {
                n = i - 1;
                break;
            }
        std::string num_val = "";
        if (j == n)
            return num_val;
        n++;
        for (size_t i = j; i < n; i++)
            num_val += s[i];
        return num_val;
    }

    bool numeric_sort_compare(std::string s1, std::string s2)
    {
        std::locale s("en_US.UTF-8");
        s1 = numeric_value(s1);
        s2 = numeric_value(s2);
        double n1, n2;
        if (is_digit(s1) && is_digit(s2))
        {
            n1 = s1.empty() ? -0.5 : atoi(s1.c_str());
            n2 = s2.empty() ? -0.5 : atoi(s2.c_str());
            return n1 <= n2;
        }
        if (is_digit(s1))
        {
            n1 = s1.empty() ? -0.5 : atoi(s1.c_str());
            return n1 <= 0.5;
        }
        if (is_digit(s2))
        {
            n2 = s2.empty() ? -0.5 : atoi(s2.c_str());
            return 0.5 <= n2;
        }
        return s(s1, s2);
    }


    void sort_stream(std::istream & input, const bool ignore_case, const bool numeric_sort)
    {
        std::vector<std::string> lines;
        // read lines
        std::string line;
        while (std::getline(input, line))
            lines.push_back(line);
        if (ignore_case)
        {
            std::sort(lines.begin(), lines.end(), ignore_case_compare);
        }
        if (numeric_sort)
        {
            std::sort(lines.begin(), lines.end(), numeric_sort_compare);
        }
        if (!ignore_case && !numeric_sort)
        {
            std::sort(lines.begin(), lines.end(), std::locale("en_US.UTF-8"));
        }
        for (const auto & x : lines)
            std::cout << x << '\n';
    }

} // anonymous namespace

int main(int argc, char ** argv)
{
    std::setlocale(LC_ALL, "en_US.UTF-8");
    bool ignore_case = false; //f
    bool numeric_sort = false; //n
    const char * input_name = nullptr;
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            if (argv[i][1] != '-')
            {
                const size_t len = std::strlen(argv[i]);
                for (size_t j = 1; j < len; j++)
                {
                    switch (argv[i][j])
                    {
                        case 'f':
                            ignore_case = true;
                            break;
                        case 'n':
                            numeric_sort = true;
                            break;
                    }
                }
            }
            else {
                if (std::strcmp(argv[i], "--ignore_case") == 0)
                {
                    ignore_case = true;
                }
                else if (std::strcmp(argv[i], "--numeric_sort") == 0)
                {
                    numeric_sort = true;
                }
            }
        }
        else {
            input_name = argv[i];
        }
    }

    if (input_name != nullptr)
    {
        std::ifstream f(input_name);
        sort_stream(f, ignore_case, numeric_sort);
    }
    else {
        sort_stream(std::cin, ignore_case, numeric_sort);
    }
}
