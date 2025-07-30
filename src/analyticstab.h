#ifndef ANALYTICSTAB_H
#define ANALYTICSTAB_H

#include <QWidget>
#include <QTabWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QChart>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include "models/datamanager.h"

class AnalyticsTab : public QWidget
{
    Q_OBJECT

public:
    explicit AnalyticsTab(DataManager *dataManager, QWidget *parent = nullptr);

private slots:
    void onDataChanged();
    void onTimeRangeChanged();
    void refreshCharts();

private:
    void setupUI();
    void setupWeightChart();
    void setupBMIChart();
    void setupBodyFatChart();
    void setupWaistChart();
    
    void updateWeightChart();
    void updateBMIChart();
    void updateBodyFatChart();
    void updateWaistChart();
    
    void setupChartAppearance(QChart *chart, const QString &title);
    QList<BodyComposition> getFilteredData();
    void setDateAxisRange(QDateTimeAxis* dateAxis, const QList<BodyComposition>& data);
    
    DataManager *m_dataManager;
    
    // UI Components
    QTabWidget *m_chartTabs;
    QComboBox *m_timeRangeCombo;
    QPushButton *m_refreshButton;
    
    // Charts
    QChartView *m_weightChartView;
    QChartView *m_bmiChartView;
    QChartView *m_bodyFatChartView;
    QChartView *m_waistChartView;
    
    QChart *m_weightChart;
    QChart *m_bmiChart;  
    QChart *m_bodyFatChart;
    QChart *m_waistChart;
    
    // Chart series
    QLineSeries *m_weightSeries;
    QLineSeries *m_bmiSeries;
    QLineSeries *m_bodyFatSeries;
    QLineSeries *m_waistSeries;
    
    // BMI category areas
    QAreaSeries *m_underweightArea;
    QAreaSeries *m_normalArea;
    QAreaSeries *m_overweightArea;
    QAreaSeries *m_obeseArea;
    
    // Body fat category areas (for males)
    QAreaSeries *m_bodyFatEssentialArea;
    QAreaSeries *m_bodyFatAthleticArea;
    QAreaSeries *m_bodyFatFitnessArea;
    QAreaSeries *m_bodyFatAverageArea;
    QAreaSeries *m_bodyFatObeseArea;
};

#endif // ANALYTICSTAB_H