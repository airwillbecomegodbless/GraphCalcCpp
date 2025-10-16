// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <random>
#include <algorithm>


// parts of function
std::vector<std::string> partsOfFunction;
std::map<std::string, int> keys;


class Coordinates {      
public:            
    std::vector<int> open;
    std::vector<int> closed;
};

std::string generateRandomString(int length)
{
    // Define the list of possible characters
    const std::string CHARACTERS
        = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuv"
        "wxyz0123456789";

    // Create a random number generator
    std::random_device rd;
    std::mt19937 generator(rd());

    // Create a distribution to uniformly select from all
    // characters
    std::uniform_int_distribution<> distribution(
        0, CHARACTERS.size() - 1);

    // Generate the random string
    std::string random_string;
    for (int i = 0; i < length; ++i) {
        random_string
            += CHARACTERS[distribution(generator)];
    }

    return random_string;
}

std::string AddParantheses(std::string yFunction)
{
    int j;
    int pos1;
    for (int i = 0; i < yFunction.length(); i++)
    {
        if (i > 0 && i < yFunction.length() - 1 && yFunction[i] == '*' || yFunction[i] == '/')  // ((((2+3)*((4+1)))/5))
        {
            if (isdigit(yFunction[i + 1]) && isdigit(yFunction[i - 1]))
            {
                j = i + 1;
                while (isdigit(yFunction[j]))
                {
                    j++;
                }
                pos1 = j;
                j = i - 1;
                while (isdigit(yFunction[j]))
                {
                    j--;
                }
                yFunction.insert(pos1, 1, ')');
                yFunction.insert(j + 1, 1, '(');
                i++;
            }
            else if (yFunction[i + 1] == '(' && yFunction[i - 1] == ')')
            {
                j = i + 1;
                while (yFunction[j] != ')')
                {
                    j++;
                }
                pos1 = j;
                
                j = i - 1;
                while (yFunction[j] != '(')
                {
                    j--;
                }
                yFunction.insert(pos1, 1, ')');
                yFunction.insert(j + 1, 1, '(');
                i++;
            }
            else if (isdigit(yFunction[i + 1]) && yFunction[i - 1] == ')')
            {
                std::cout << "work" << std::endl;
                j = i + 1;
                while (isdigit(yFunction[j]))
                {
                    j++;
                }
                pos1 = j;
                j = i - 1;
                while (yFunction[j] != '(')
                {
                    j--;
                }
                yFunction.insert(pos1, 1, ')');
                yFunction.insert(j + 1, 1, '(');
                i++;
            }
            else if (isdigit(yFunction[i - 1]) && yFunction[i + 1] == '(')
            {
                j = i + 1;
                while (yFunction[j] != ')')
                {
                    j++;
                }
                pos1 = j;
                j = i - 1;
                while (isdigit(yFunction[j]))
                {
                    j--;
                }
                yFunction.insert(pos1, 1, ')');
                yFunction.insert(j + 1, 1, '(');
                i++;
            }
        }
    }
    std::cout << "Function after parentheses for * and / >>> " << yFunction << std::endl;
    for (int i = yFunction.length() - 1; i >= 0; i--)
    {
        if (yFunction[i] != '*' && yFunction[i] != '/' && yFunction[i] != '+' && yFunction[i] != '-')
        {
            if (i < yFunction.length() - 1 && yFunction[i] != ')' && yFunction[i] != '(' && yFunction[i + 1] == '(')
            {
                pos1 = i + 1;
                while (isalpha(yFunction[i]) || yFunction[i] == '-' || yFunction[i] == 'x')
                {
                    i--;
                }
                yFunction.insert(i + 1, 1, ')');
                yFunction.insert(i, 1, '(');
            }
            if (i > 0 && yFunction[i - 1] == ')' && yFunction[i] != '(' && yFunction[i] != ')')
            {
                pos1 = i;
                if (yFunction[i] == '-')
                {
                    i++;
                }
                while (isalpha(yFunction[i]) || yFunction[i] == 'x')
                {
                    i++;
                }
                yFunction.insert(i + 1, 1, ')');
                yFunction.insert(pos1, 1, '(');
            }
        }
    }
    std::cout << "Function after parentheses for * >>> " << yFunction << std::endl;
	return yFunction;
}

void Parentheses(std::string yFunction)
{
    // coordinates of parentheses
    Coordinates insideOfParentheses;
    // to check for copies
    std::vector<int> openParenthesesIndexes;
    std::vector<int> closedParenthesesIndexes;
    // to choose last parentheses
    int lastOpen;
    // amount of parentheses
    int parenthesesNum = 0;

    // part of function
    std::string prt;
    // used parts
    std::vector<int> alreadyPart = { 99999999 };
    // key var
    std::string k;
    // flag
    bool flag = false;
    // length of key
    int keyLength = 10;
    if (yFunction.find("(") != std::string::npos && yFunction.find(")") != std::string::npos) // Parentheses
    {
        for (int i = 0; i < yFunction.length(); i++)
        {
            if (yFunction[i] == '(')
            {
                parenthesesNum++;
            }
        }
        std::cout << "Number of Parentheses >>> " << parenthesesNum << std::endl;

        while (parenthesesNum != openParenthesesIndexes.size()) // open and closed parantheses index
        {
            for (int i = 0; i < yFunction.length(); i++)
            {
                if (yFunction[i] == '(' && std::find(openParenthesesIndexes.begin(), openParenthesesIndexes.end(), i) == openParenthesesIndexes.end())
                {
                    lastOpen = i;
                }
            }
            openParenthesesIndexes.push_back(lastOpen);
            insideOfParentheses.open.push_back(lastOpen);
            for (int i = lastOpen; i < yFunction.length(); i++)
            {
                if (yFunction[i] == ')' && std::find(closedParenthesesIndexes.begin(), closedParenthesesIndexes.end(), i) == closedParenthesesIndexes.end())
                {
                    insideOfParentheses.closed.push_back(i);
                    closedParenthesesIndexes.push_back(i);
                    break;
                }
            }
        }

        for (int i = 0; i < insideOfParentheses.open.size(); i++)
        {
            prt = "";
            for (int j = i - 1; j >= 0; j--) // iterate thru already included parts
            {
                if (insideOfParentheses.open[j] > insideOfParentheses.open[i] && insideOfParentheses.closed[j] < insideOfParentheses.closed[i] && std::find(alreadyPart.begin(), alreadyPart.end(), j) == alreadyPart.end() && j != alreadyPart.back()) // if parenthesis inside and wasnt already inclued 
                {
                    /*if (yFunction.substr(insideOfParentheses.open[j] - insideOfParentheses.open[i], 1) == "/" || yFunction.substr(insideOfParentheses.open[j] - insideOfParentheses.open[i], 1) == "-" || yFunction.substr(insideOfParentheses.open[j] - insideOfParentheses.open[i], 1) == "+")
                    {
                        prt = prt + yFunction.substr(insideOfParentheses.open[j] - insideOfParentheses.open[i], 1);
                    }*/
                    if (yFunction.substr(insideOfParentheses.open[i] + 1, insideOfParentheses.open[j] - insideOfParentheses.open[i] - 1) != "" && yFunction.substr(insideOfParentheses.open[i] + 1, 1) != "(")
                    {
                        flag = false;
                        for (auto const& [key, val] : keys)
                        {
                            if (val == j) {
                                prt = prt + yFunction.substr(insideOfParentheses.open[i], insideOfParentheses.open[j] - insideOfParentheses.open[i]) + "(" + key + "))";
                                flag = true;
                                break;
                            }
                        }
                        if (!flag)
                        {
                            k = generateRandomString(keyLength);
                            keys[k] = j;
                            prt = prt + yFunction.substr(insideOfParentheses.open[i], insideOfParentheses.open[j] - insideOfParentheses.open[i]) + "(" + k + "))";
                        }
                    }
                    else
                    {
                        for (auto const& [key, val] : keys)
                        {
                            if (val == j) {
                                prt = prt + "(" + key + ")";
                                flag = true;
                                break;
                            }
                        }
                        if (!flag)
                        {
                            k = generateRandomString(keyLength);
                            keys[k] = j;
                            prt = prt + "(" + k + ")";
                        }
                    }
                    alreadyPart.push_back(j);
                    if (yFunction.substr(insideOfParentheses.closed[j] + 1, insideOfParentheses.closed[i] - insideOfParentheses.closed[j] - 1).find('(') == std::string::npos) // idk
                    {
                        prt = prt + yFunction.substr(insideOfParentheses.closed[j] + 1, insideOfParentheses.closed[i] - insideOfParentheses.closed[j] - 1);
                    }
                    
                }
            }
            if (prt != "")
            {
                partsOfFunction.push_back(prt);
            }
            else
            {
                partsOfFunction.push_back(yFunction.substr(insideOfParentheses.open[i], insideOfParentheses.closed[i] - insideOfParentheses.open[i] + 1));
            }
        }
        for (int i = 0; i < partsOfFunction.size(); i++)
        {
            std::cout << "Part " << i << " >>> " << partsOfFunction[i] << std::endl;
        }
        for (auto const& [key, val] : keys)
        {
            std::cout << "Key " << " >>> " << key << " Part >>> " << partsOfFunction[keys[key]] << std::endl;
        }
    }
}

float Solve()
{
    double result = 0;
    char lastSign = '\0';
    std::string strNum;
    double num = 0;
    bool neg = false;
    bool flag;
    std::string k;

    for (int part = 0; part < partsOfFunction.size(); part++) // get rid of parentheses and solve
    {
        flag = false;
        k = "";
        for (int i = partsOfFunction[part].size(); i >= 0; i--)
        {
            if (partsOfFunction[part][i] == '(' && i != 0 && partsOfFunction[part][i - 1] == ')')
            {
                partsOfFunction[part][i] = '*';
            }
            else if (partsOfFunction[part][i] == '(' || partsOfFunction[part][i] == ')')
            {
                partsOfFunction[part].erase(i, 1);
            }
        }
        for (auto const& [key, val] : keys)
        {
            if (partsOfFunction[part].find(key) != std::string::npos)
            {
                partsOfFunction[part].replace(partsOfFunction[part].find(key), key.length(), partsOfFunction[keys[key]]);
            }
        }
        std::cout << "Part " << part << " >>> " << partsOfFunction[part] << std::endl;
        for (int i = 0; i < partsOfFunction[part].size(); i++)
        {
            strNum = "";
            num = 0;
            if (partsOfFunction[part][i] == '*' || partsOfFunction[part][i] == '/' || partsOfFunction[part][i] == '-' || partsOfFunction[part][i] == '+')
            {
                if (partsOfFunction[part][i] == '-' && i > 0 && (partsOfFunction[part][i - 1] == '*' || partsOfFunction[part][i - 1] == '/' || partsOfFunction[part][i - 1] == '-' || partsOfFunction[part][i - 1] == '+'))
                {
                    neg = true;
                }
                else
                {
                    lastSign = partsOfFunction[part][i];
                }
            }
            if (isdigit(partsOfFunction[part][i]))
            {
                strNum += partsOfFunction[part][i];
                while (i < partsOfFunction[part].size() && (isdigit(partsOfFunction[part][i + 1]) || partsOfFunction[part][i + 1] == '.'))
                {
                    i++;
                    strNum += partsOfFunction[part][i];
                }
                num = std::stod(strNum);
                if (lastSign != '\0' && strNum != "")
                {
                    if (neg)
                    {
                        num = -num;
                        neg = false;
                    }
                    switch (lastSign)
                    {
                    case '*':
                        result *= num;
                        lastSign = '\0';
                        break;
                    case '/':
                        result /= num;
                        lastSign = '\0';
                        break;
                    case '-':
                        result -= num;
                        lastSign = '\0';
                        break;
                    case '+':
                        result += num;
                        lastSign = '\0';
                        break;
                    default:
                        break;
                    }
                }
                else if (strNum != "")
                {
                    result = num;
                }
            }
        }
        partsOfFunction[part] = std::to_string(result);
        std::cout << "Result " << part << " >>> " << partsOfFunction[part] << std::endl;
        result = 0;
    }
	result = std::stod(partsOfFunction[partsOfFunction.size() - 1]);
    return result;
}

int main()
{
    // function
    std::string yFunction = "(6/(2*(1+2)))";
    yFunction.insert(yFunction.begin(), '(');
    yFunction.push_back(')');
    yFunction.erase(remove(yFunction.begin(), yFunction.end(), ' '), yFunction.end());
    std::cout << "Function >>> " << yFunction << std::endl;

    /*for (int i = yFunction.length() - 2; i >= 1; i--)
    {
        if (yFunction[i] == '*' && yFunction[i - 1] == ')' && yFunction[i + 1] == '(')
        {
            yFunction.erase(i, 1);
        }
    }
    std::cout << "Function after removing * >>> " << yFunction << std::endl;*/

 	yFunction = AddParantheses(yFunction);

    

	Parentheses(yFunction);

	float result = Solve();
	std::cout << std::endl << "RESULT >>> " << result << std::endl;


    return 0;
}