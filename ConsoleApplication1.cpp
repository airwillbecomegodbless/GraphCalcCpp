#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <random>
#include <algorithm>
#include <emscripten.h>
#include <emscripten/html5.h>
#include <SDL.h>

const int w = 800;
const int h = 600;

double wRatio = 50;
double hRatio = 50;

double wCenter = w/2;
double hCenter = h/2;

double pStartX;
double pStartY;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

std::string yFunction = "";

// parts of function
std::vector<std::string> partsOfFunction;
std::vector<std::string> duplicate;
std::map<std::string, int> keys;


class Coordinates 
{      
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
            if (isalnum(yFunction[i + 1]) && isalnum(yFunction[i - 1]))
            {
                j = i + 1;
                while (isalnum(yFunction[j]))
                {
                    j++;
                }
                pos1 = j;
                j = i - 1;
                while (isalnum(yFunction[j]))
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
            else if (isalnum(yFunction[i + 1]) && yFunction[i - 1] == ')')
            {
                j = i + 1;
                while (isalnum(yFunction[j]))
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
            else if (isalnum(yFunction[i - 1]) && yFunction[i + 1] == '(')
            {
                j = i + 1;
                while (yFunction[j] != ')')
                {
                    j++;
                }
                pos1 = j;
                j = i - 1;
                while (isalnum(yFunction[j]))
                {
                    j--;
                }
                yFunction.insert(pos1, 1, ')');
                yFunction.insert(j + 1, 1, '(');
                i++;
            }
        }
    }
    //std::cout << "Function after parentheses for * and / >>> " << yFunction << std::endl;
    for (int i = yFunction.length() - 1; i >= 0; i--)
    {
        if (yFunction[i] != '*' && yFunction[i] != '/' && yFunction[i] != '+' && yFunction[i] != '-')
        {
            if (i < yFunction.length() - 1 && yFunction[i] != ')' && yFunction[i] != '(' && yFunction[i + 1] == '(')
            {
                pos1 = i + 1;
                while (isalnum(yFunction[i]) || yFunction[i] == '-')
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
                while (isalnum(yFunction[i]))
                {
                    i++;
                }
                yFunction.insert(i + 1, 1, ')');
                yFunction.insert(pos1, 1, '(');
            }
        }
    }
    //std::cout << "Function after parentheses for * >>> " << yFunction << std::endl;
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
        //std::cout << "Number of Parentheses >>> " << parenthesesNum << std::endl;

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
            //std::cout << "Part " << i << " >>> " << partsOfFunction[i] << std::endl;
        }
        for (auto const& [key, val] : keys)
        {
            //std::cout << "Key " << " >>> " << key << " Part >>> " << partsOfFunction[keys[key]] << std::endl;
        }
    }
}

double Solve(double x)
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
            //std::cout << "Part " << part << " >>> " << partsOfFunction[part] << std::endl;
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
                if (isalnum(partsOfFunction[part][i]))
                {
                    if (partsOfFunction[part][i] == 'x')
                    {
                        strNum += std::to_string(x);
                    }
                    else
                    {
                        strNum += partsOfFunction[part][i];
                    }
                    while (i < partsOfFunction[part].size() && (isdigit(partsOfFunction[part][i + 1]) || partsOfFunction[part][i + 1] == '.'))
                    {
                        i++;
                        if (partsOfFunction[part][i] == 'x')
                        {
                            strNum += std::to_string(x);
                        }
                        else
                        {
                            strNum += partsOfFunction[part][i];
                        }
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
            //std::cout << "Result " << part << " >>> " << partsOfFunction[part] << std::endl;
            result = 0;
        }
        return std::stod(partsOfFunction[partsOfFunction.size() - 1]);
}

void Calculate(std::string input)
{
    partsOfFunction.clear();
    duplicate.clear();
    keys.clear();
    input.erase(remove(input.begin(), input.end(), ' '), input.end());
    if (input.find('=') != std::string::npos)
    {
        if (input.substr(input.find('=') + 1, input.length() - input.find('=') - 1).find('y') != std::string::npos)
        {
            yFunction = input.substr(0, input.length() - (input.length() - input.find('=')));
        }
        else
        {
            yFunction = input.substr(input.find('=') + 1, input.length() - input.find('=') - 1);
        }
    }
    else 
    {
        yFunction = input;
    }
    yFunction.insert(yFunction.begin(), '(');
    yFunction.push_back(')');
    yFunction.erase(remove(yFunction.begin(), yFunction.end(), ' '), yFunction.end());    
    if (yFunction.find('x') != std::string::npos)
    {
        for (int i = yFunction.length() - 1; i >= 0; i--)
        {
            if (i < yFunction.length() - 1 && yFunction[i] == 'x' && yFunction[i + 1] == 'x')
            {
                yFunction.insert(i + 1, 1, '*');
            }
        }
    }
    //std::cout << "Function after removing * >>> " << yFunction << std::endl;

 	yFunction = AddParantheses(yFunction);

	Parentheses(yFunction);
    duplicate = partsOfFunction;
    
}

void draw() {
    // Clear screen
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // white
    SDL_RenderClear(renderer);

    // Draw axes in black
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    // X-axis
    SDL_RenderDrawLine(renderer, 0, h/2, w, h/2);
    // Y-axis
    SDL_RenderDrawLine(renderer, w/2, 0, w/2, h);

    // X-axis lines
    for (double x = wCenter; x >= 0; x -= wRatio) {
        SDL_RenderDrawLine(renderer, x, h/2 - 10, x, h/2 + 10);
    }
    for (double x = wCenter; x <= w; x += wRatio) {
        SDL_RenderDrawLine(renderer, x, h/2 - 10, x, h/2 + 10);
    }

    // Y-axis lines
    for (double y = hCenter; y >= 0; y -= hRatio) {
        SDL_RenderDrawLine(renderer, w/2 - 10, y, w/2 + 10, y);
    }

    for (double y = hCenter; y <= h; y += hRatio) {
        SDL_RenderDrawLine(renderer, w/2 - 10, y, w/2 + 10, y);
    }

    // Draw simple function y = x^2
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // red

        for (double x = -w/2/wRatio; x <= w/2/wRatio; x += 0.01) {
            if (x == -w/2/wRatio)
            {
                double y0 = Solve(x);
                partsOfFunction = duplicate;
                pStartX = int(x * wRatio + w/2);        // pixel X for first point
                pStartY = int(-y0 * hRatio + h/2);      // pixel Y for first point
            }
            double y = Solve(x); // change to x^4 + 4*x^3 if you want
            partsOfFunction = duplicate;
            int cx = int(x * wRatio + w/2);
            int cy = int(-y * hRatio + h/2);
            SDL_RenderDrawLine(renderer, (int)pStartX, (int)pStartY, cx, cy);
            pStartX = cx;
            pStartY = cy;
        }
        
    SDL_RenderPresent(renderer);
}

// JavaScript function to get input text
EM_JS(char*, get_input_text, (), {
  const value = document.getElementById('userInput').value;
  const lengthBytes = lengthBytesUTF8(value) + 1;
  const stringOnWasmHeap = _malloc(lengthBytes);
  stringToUTF8(value, stringOnWasmHeap, lengthBytes);
  return stringOnWasmHeap;
});

// Function C++ will call every time the text changes
extern "C" {
EMSCRIPTEN_KEEPALIVE
void onInputChange() {
  char* text = get_input_text();
  std::string inputText(text);
  free(text);

  //std::cout << "User typed: " << inputText << std::endl;

  // Recompute using the typed string
  Calculate(inputText);
  draw();

  EM_ASM({
    var s = UTF8ToString($0);
    var el = document.getElementById('liveText');
    if (el) el.innerText = 'Live text: ' + s;
  }, inputText.c_str());
}
}


void main_loop() {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        if(event.type == SDL_QUIT) emscripten_cancel_main_loop();
        if(event.type == SDL_MOUSEWHEEL) {
            wRatio -= event.wheel.y * 2; // zoom factor
            hRatio -= event.wheel.y * 2;
            if(wRatio < 5) wRatio = 5;
            if(hRatio < 5) hRatio = 5;
        }
    }
    draw();
}



int main()
{
    Calculate("");
    
	SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("SDL Graph", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_EventState(SDL_KEYDOWN, SDL_DISABLE);
    SDL_EventState(SDL_KEYUP, SDL_DISABLE);
    SDL_EventState(SDL_TEXTINPUT, SDL_DISABLE);

    emscripten_set_main_loop(main_loop, 0, 1);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}