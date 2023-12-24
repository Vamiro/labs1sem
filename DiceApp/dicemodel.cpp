#include "dicemodel.h"

DiceModel::DiceModel() : rd_(), gen_(rd_()) {

}

void DiceModel::load(const std::string &input) {
    dices_ = parse(input);
    max_ = 0;
    min_ = 0;
    for(auto dice : dices_) {
        max_ += dice.dice_type * dice.rolls_count;
        min_ += dice.rolls_count;
    }
}

int DiceModel::roll() {
    int sum = 0;

    for (auto &dice : dices_) {
        sum += calculate(dice);
    }
    return sum;
}

int DiceModel::max() const{
    return max_;
}

int DiceModel::min() const{
    return min_;
}

std::vector<Dice> DiceModel::parse(const std::string& input) {
    std::vector<Dice> dices;
    std::regex regex(R"(\d+d\d+|\+\d+)");

    for (auto it = std::sregex_token_iterator(input.begin(), input.end(), regex); it != std::sregex_token_iterator(); ++it) {
        dices.push_back(parse_dice(it->str()));
    }
    return dices;
}

Dice DiceModel::parse_dice(const std::string &str) {
    Dice dice;
    std::regex regex(R"((\d+)d(\d+)|\+(\d+))");
    std::smatch matches;
    std::regex_match(str, matches, regex);

    if (!matches[1].str().empty()) {
        dice.rolls_count = std::stoi(matches[1].str());
        dice.dice_type = std::stoi(matches[2].str());
    } else if (!matches[3].str().empty()) {
        dice.rolls_count = std::stoi(matches[3].str());
        dice.dice_type = 1;
    }
    return dice;
}

int DiceModel::calculate(Dice dice) {
    int result = 0;
    if(dice.dice_type == 1) {
        result = dice.rolls_count;
    } else {
        for(int i = 0; i < dice.rolls_count; ++i) {
            result += generate_random_number(1, dice.dice_type);
        }
    }

    return result;
}

int DiceModel::generate_random_number(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(gen_);
}
