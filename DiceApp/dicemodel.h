#ifndef DICEMODEL_H
#define DICEMODEL_H

#include <string>
#include <vector>
#include <regex>
#include <random>

struct Dice {
    int rolls_count;
    int dice_type;
};

class DiceModel {
public:
    DiceModel();
    void load(const std::string& input);
    int roll();
    int max() const;
    int min() const;
private:
    int max_;
    int min_;
    std::vector<Dice> dices_;
    std::random_device rd_;
    std::mt19937 gen_;

    std::vector<Dice> parse(const std::string& input);
    Dice parse_dice(const std::string& str);
    int calculate(Dice dice);
    int generate_random_number(int min, int max);
};

#endif // DICEMODEL_H
