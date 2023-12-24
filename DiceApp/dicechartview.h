#ifndef DICECHARTVIEW_H
#define DICECHARTVIEW_H

#include <QChartView>
#include <QtCharts>
#include <QBarSet>
#include <QBarSeries>
#include <algorithm>
#include "dicemodel.h"

class DiceChartView : public QChartView
{
public:
    DiceChartView(QWidget* parent);

    void load (const std::string& input, int rolls);
private:
    DiceModel dice_model_;
    QChart chart_;

    void create_chart(int rolls);
};

#endif // DICECHARTVIEW_H
