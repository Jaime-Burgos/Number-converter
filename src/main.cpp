#include <cstdint>
#include <iostream>
#include <qnamespace.h>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>
#include <utility>
#include <optional>
#include <QString>
#include <QStringList>

std::map<QString,int> wordMap{{"one", 1},
                              {"two", 2},
                              {"three", 3},
                              {"four", 4},
                              {"five", 5},
                              {"six", 6},
                              {"seven", 7},
                              {"eight", 8},
                              {"nine", 9},
                              {"zero", 0},
                              {"ten", 10},
                              {"eleven", 11},
                              {"twelve", 12},
                              {"dozen", 12},
                              {"thirteen", 13},
                              {"fourteen", 14},
                              {"fifteen", 15},
                              {"sixteen", 16},
                              {"seventeen", 17},
                              {"eighteen", 18},
                              {"nineteen", 19},
                              {"twenty", 20},
                              {"thirty", 30},
                              {"forty", 40},
                              {"fifty", 50},
                              {"sixty", 60},
                              {"seventy", 70},
                              {"eighty", 80},
                              {"ninety", 90},
                              {"hundred", 100},
                              {"thousand", 1E3},
                              {"million", 1E6},
                              {"bilion", 1E9}};

std::map<QString,int> multipliers{{"hundred", 100},
                                  {"thousand", 1E3},
                                  {"million", 1E6},
                                  {"bilion", 1E9}};

bool isNumber(const QString& word)
{
    bool retVal = true;

    auto splitWord = word.split("-");

    for (auto part : splitWord)
    {
        part.remove(",");
        if ((wordMap.find(part.toLower()) == wordMap.end()))
        {
            return false;
        }
    }

    return true;
}

std::list<std::pair<int,int>> findNumbers(const QStringList& splitPhrase)
{
    QString result;

    std::list<std::pair<int,int>> numbersLocation;

    int firstNumber = splitPhrase.length();

    for (int i = 0; i < splitPhrase.length(); i++)
    {
        auto word = splitPhrase.at(i);

        bool isANumber = isNumber(word);

        if (firstNumber == splitPhrase.length()) // Number not found yet
        {
            if (isANumber)
            {
                firstNumber = i;
            }
        }
        else { // Number was already found
            if (!isANumber) // But next word is not a number
            {
                if (word.compare("and", Qt::CaseInsensitive) != 0)
                {
                    numbersLocation.emplace_back(firstNumber, i - 1);
                    
                    // Reset firstNumber location to continue the loop
                    firstNumber = splitPhrase.length();
                }
            }
        }

        if ((i+1) >= splitPhrase.length() && (firstNumber != splitPhrase.length()))
        {
            numbersLocation.emplace_back(firstNumber, i);
        }
    }

    return numbersLocation;
}

QString convertToDigits(const QStringList& wordsNumber)
{
    std::int64_t finalNumber = 0;

    bool trailingComma = false;

    for (auto word : wordsNumber)
    {
        if (word.compare("and", Qt::CaseInsensitive) == 0)
        {
            continue;
        }

        if (word.contains(","))
        {
            word.remove(",");
            trailingComma = true;
        }

        auto multiplier = multipliers.find(word);
        if (multiplier != multipliers.end())
        {
            finalNumber *= multiplier->second;
            continue;
        }

        auto splitWord = word.split("-");

        for (auto singleWord : splitWord)
        {
            auto number = wordMap.find(singleWord.toLower());
            if (number != wordMap.end())
            {
                finalNumber += number->second;
            }
            else {
                std::cerr << "ERROR! Unrecognised word " << word.toStdString() << std::endl;
            }
        }
    }

    QString result = QString::fromStdString(std::to_string(finalNumber));

    if (trailingComma)
    {
        result += ",";
    }

    return result;
}

QString substituteNumbers(const QStringList& phrase, std::list<std::pair<int,int>> locations)
{
    QStringList retVal;

    auto it = locations.begin();

    QStringList fullTextNumber;

    for (int i = 0; i < phrase.length(); i++)
    {
        if (i < it->first || i > it->second)
        {
            retVal += phrase.at(i);
        }
        else if (i < it->second)
        {
            fullTextNumber += phrase.at(i);
        }
        else if (i == it->second)
        {
            fullTextNumber += phrase.at(i);
            retVal += convertToDigits(fullTextNumber);
            fullTextNumber.clear();
            it++;
        }
    }

    return retVal.join(" ");
}

QString convertPhrase(const QString& phrase)
{
    QStringList splitPhrase = phrase.split(" ");

    auto locations = findNumbers(splitPhrase);

    if (locations.empty())
    {
        std::cout << "No numbers found in phrase" << std::endl;
        return phrase;
    }
    else {
        auto converted = substituteNumbers(splitPhrase,locations);
        return converted;
    }

}

int main(int argc, char* argv[])
{
    QStringList inputPhrases = {
        "run One hundred and three thousand miles",
        "ten",
        "This phrase does not have a number on it",
        "I own thirty-three cars and two houses",
        "It costs twenty-five thousand and thirty dollars",
        "twelve pears, twenty-five apples",
        "pick one, it's on me",
        "One thousand and nine hundred", // Not supported, two multipiers
        "one hundred and twenty-three million",
        "Twenty-three hundred sixty-one",
        "Nineteen fifty-six was quite a year", // Years in this format are not supported
        "One-half is slightly less than five-eighths" // Fractions not supported
        };

    for (auto phrase : inputPhrases) {
        std::cout << "\nInput: \"" << phrase.toStdString() << "\"\n";
        auto converted = convertPhrase(phrase);
        std::cout << "Output: \"" << converted.toStdString() << "\"\n";
    }

    return 0;
}
