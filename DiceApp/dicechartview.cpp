#include "dicechartview.h"

DiceChartView::DiceChartView(QWidget* parent) : QChartView(parent), dice_model_(){

}

void DiceChartView::load(const std::string& input, int rolls) {
    dice_model_.load(input);
    create_chart(rolls);
}

void DiceChartView::create_chart(int rolls) {
    this->chart()->deleteLater();

    auto set = new QBarSet("Rolls results");
    std::vector<int> results(dice_model_.max() - dice_model_.min() + 1, 0);

    for (int i = 0; i < rolls; ++i) results[dice_model_.roll() - dice_model_.min()]++;
    for (auto res: results) set->append(res);

    QBarSeries *series = new QBarSeries;
    series->append(set);
    series->setLabelsVisible(true);
    QStringList categories;

    for (int i = dice_model_.min(); i <= dice_model_.max(); ++i) categories << QString::number(i);

    auto chart = new QChart;
    chart->addSeries(series);
    chart->setTitle("Dice");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    auto axisX = new QBarCategoryAxis;
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    auto axisY = new QValueAxis;
    axisY->setLabelFormat("%d");
    axisY->setTickCount(std::clamp(*std::max_element(results.begin(), results.end()) + 1, 2, 10));
    axisY->setRange(0, *std::max_element(results.begin(), results.end()));
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    this->setChart(chart);
}
