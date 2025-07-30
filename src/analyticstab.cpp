#include "analyticstab.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDate>
#include <QDebug>
#include <cstdio>
#include <QtCharts/QLineSeries>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>

AnalyticsTab::AnalyticsTab(DataManager *dataManager, QWidget *parent)
    : QWidget(parent)
    , m_dataManager(dataManager)
    , m_chartTabs(nullptr)
    , m_timeRangeCombo(nullptr)
    , m_refreshButton(nullptr)
    , m_weightChartView(nullptr)
    , m_bmiChartView(nullptr)
    , m_bodyFatChartView(nullptr)
    , m_waistChartView(nullptr)
    , m_weightChart(nullptr)
    , m_bmiChart(nullptr)
    , m_bodyFatChart(nullptr)
    , m_waistChart(nullptr)
    , m_weightSeries(nullptr)
    , m_bmiSeries(nullptr)
    , m_bodyFatSeries(nullptr)
    , m_waistSeries(nullptr)
    , m_underweightArea(nullptr)
    , m_normalArea(nullptr)
    , m_overweightArea(nullptr)
    , m_obeseArea(nullptr)
{
    setupUI();
    
    // Connect to data manager signals
    connect(m_dataManager, &DataManager::dataChanged, this, &AnalyticsTab::onDataChanged);
    
    // Debug: Test data loading using same method as body composition tab
    printf("AnalyticsTab constructor: Testing individual date loading like body composition tab...\n");
    QList<QDate> allDates = m_dataManager->getBodyCompositionDates();
    printf("Found %d dates with body composition data\n", allDates.size());
    
    for (int i = 0; i < qMin(3, allDates.size()); ++i) {
        QDate date = allDates[i];
        if (m_dataManager->hasBodyComposition(date)) {
            BodyComposition comp = m_dataManager->loadBodyComposition(date);
            printf("  Date %s: Valid:%d Weight:%.1f BMI:%.1f\n", 
                   date.toString().toLocal8Bit().data(),
                   comp.date().isValid() ? 1 : 0, comp.weight(), comp.bmi());
        }
    }
    
    // Also test our aggregation methods
    QList<BodyComposition> testData = m_dataManager->getAllBodyCompositionSorted();
    printf("Our getAllBodyCompositionSorted method returns %d entries\n", testData.size());
    
    // Initial chart update
    refreshCharts();
}

void AnalyticsTab::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Controls layout
    QHBoxLayout *controlsLayout = new QHBoxLayout();
    
    QLabel *timeRangeLabel = new QLabel("Time Range:");
    m_timeRangeCombo = new QComboBox();
    m_timeRangeCombo->addItem("Last 30 Days", 30);
    m_timeRangeCombo->addItem("Last 90 Days", 90);
    m_timeRangeCombo->addItem("Last 6 Months", 180);
    m_timeRangeCombo->addItem("Last Year", 365);
    m_timeRangeCombo->addItem("All Time", -1);
    m_timeRangeCombo->setCurrentIndex(2); // Default to 6 months
    
    m_refreshButton = new QPushButton("Refresh");
    
    controlsLayout->addWidget(timeRangeLabel);
    controlsLayout->addWidget(m_timeRangeCombo);
    controlsLayout->addStretch();
    controlsLayout->addWidget(m_refreshButton);
    
    // Charts tab widget
    m_chartTabs = new QTabWidget();
    
    // Setup individual charts
    setupWeightChart();
    setupBMIChart();
    setupBodyFatChart();
    setupWaistChart();
    
    mainLayout->addLayout(controlsLayout);
    mainLayout->addWidget(m_chartTabs);
    
    // Connect signals
    connect(m_timeRangeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &AnalyticsTab::onTimeRangeChanged);
    connect(m_refreshButton, &QPushButton::clicked, this, &AnalyticsTab::refreshCharts);
}

void AnalyticsTab::setupWeightChart()
{
    m_weightChart = new QChart();
    m_weightSeries = new QLineSeries();
    
    setupChartAppearance(m_weightChart, "Weight Trend");
    m_weightChart->addSeries(m_weightSeries);
    
    // Manually set up axes following Qt documentation exactly
    QDateTimeAxis *axisX = new QDateTimeAxis();
    axisX->setTickCount(10);  // This is required according to Qt docs
    axisX->setFormat("MMM dd");
    axisX->setTitleText("Date");
    m_weightChart->addAxis(axisX, Qt::AlignBottom);
    m_weightSeries->attachAxis(axisX);
    
    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Weight (lbs)");
    m_weightChart->addAxis(axisY, Qt::AlignLeft);
    m_weightSeries->attachAxis(axisY);
    
    printf("setupWeightChart: Axes created and attached\n");
    
    m_weightChartView = new QChartView(m_weightChart);
    m_weightChartView->setRenderHint(QPainter::Antialiasing);
    
    m_chartTabs->addTab(m_weightChartView, "Weight");
}

void AnalyticsTab::setupBMIChart()
{
    m_bmiChart = new QChart();
    m_bmiSeries = new QLineSeries();
    
    setupChartAppearance(m_bmiChart, "BMI Progress");
    
    // Create BMI category background areas
    QLineSeries *underweightUpper = new QLineSeries();
    QLineSeries *underweightLower = new QLineSeries();
    QLineSeries *normalUpper = new QLineSeries();
    QLineSeries *normalLower = new QLineSeries();
    QLineSeries *overweightUpper = new QLineSeries();
    QLineSeries *overweightLower = new QLineSeries();
    QLineSeries *obeseUpper = new QLineSeries();
    QLineSeries *obeseLower = new QLineSeries();
    
    m_underweightArea = new QAreaSeries(underweightUpper, underweightLower);
    m_underweightArea->setName("Underweight (<18.5)");
    m_underweightArea->setBrush(QBrush(QColor(173, 216, 230, 100))); // Light blue
    
    m_normalArea = new QAreaSeries(normalUpper, normalLower);
    m_normalArea->setName("Normal (18.5-24.9)");
    m_normalArea->setBrush(QBrush(QColor(144, 238, 144, 100))); // Light green
    
    m_overweightArea = new QAreaSeries(overweightUpper, overweightLower);
    m_overweightArea->setName("Overweight (25-29.9)");
    m_overweightArea->setBrush(QBrush(QColor(255, 255, 0, 100))); // Light yellow
    
    m_obeseArea = new QAreaSeries(obeseUpper, obeseLower);
    m_obeseArea->setName("Obese (≥30)");
    m_obeseArea->setBrush(QBrush(QColor(255, 182, 193, 100))); // Light pink
    
    m_bmiChart->addSeries(m_underweightArea);
    m_bmiChart->addSeries(m_normalArea);
    m_bmiChart->addSeries(m_overweightArea);
    m_bmiChart->addSeries(m_obeseArea);
    m_bmiChart->addSeries(m_bmiSeries);
    
    // Setup axes with manual configuration like weight chart
    QDateTimeAxis *axisX = new QDateTimeAxis();
    axisX->setTickCount(10);
    axisX->setFormat("MMM dd");
    axisX->setTitleText("Date");
    m_bmiChart->addAxis(axisX, Qt::AlignBottom);
    
    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("BMI");
    axisY->setRange(15, 40);
    m_bmiChart->addAxis(axisY, Qt::AlignLeft);
    
    // Attach axes to all series
    m_underweightArea->attachAxis(axisX);
    m_underweightArea->attachAxis(axisY);
    m_normalArea->attachAxis(axisX);
    m_normalArea->attachAxis(axisY);
    m_overweightArea->attachAxis(axisX);
    m_overweightArea->attachAxis(axisY);
    m_obeseArea->attachAxis(axisX);
    m_obeseArea->attachAxis(axisY);
    m_bmiSeries->attachAxis(axisX);
    m_bmiSeries->attachAxis(axisY);
    
    m_bmiChartView = new QChartView(m_bmiChart);
    m_bmiChartView->setRenderHint(QPainter::Antialiasing);
    
    m_chartTabs->addTab(m_bmiChartView, "BMI");
}

void AnalyticsTab::setupBodyFatChart()
{
    m_bodyFatChart = new QChart();
    m_bodyFatSeries = new QLineSeries();
    
    setupChartAppearance(m_bodyFatChart, "Body Fat Percentage");
    
    // Create body fat category background areas (using male ranges)
    QLineSeries *essentialUpper = new QLineSeries();
    QLineSeries *essentialLower = new QLineSeries();
    QLineSeries *athleticUpper = new QLineSeries();
    QLineSeries *athleticLower = new QLineSeries();
    QLineSeries *fitnessUpper = new QLineSeries();
    QLineSeries *fitnessLower = new QLineSeries();
    QLineSeries *averageUpper = new QLineSeries();
    QLineSeries *averageLower = new QLineSeries();
    QLineSeries *obeseUpper = new QLineSeries();
    QLineSeries *obeseLower = new QLineSeries();
    
    m_bodyFatEssentialArea = new QAreaSeries(essentialUpper, essentialLower);
    m_bodyFatEssentialArea->setName("Essential (2-5%)");
    m_bodyFatEssentialArea->setBrush(QBrush(QColor(0, 255, 255, 100))); // Light cyan
    
    m_bodyFatAthleticArea = new QAreaSeries(athleticUpper, athleticLower);
    m_bodyFatAthleticArea->setName("Athletic (6-13%)");
    m_bodyFatAthleticArea->setBrush(QBrush(QColor(0, 255, 0, 100))); // Light green
    
    m_bodyFatFitnessArea = new QAreaSeries(fitnessUpper, fitnessLower);
    m_bodyFatFitnessArea->setName("Fitness (14-17%)");
    m_bodyFatFitnessArea->setBrush(QBrush(QColor(144, 238, 144, 100))); // Light green
    
    m_bodyFatAverageArea = new QAreaSeries(averageUpper, averageLower);
    m_bodyFatAverageArea->setName("Average (18-24%)");
    m_bodyFatAverageArea->setBrush(QBrush(QColor(255, 255, 0, 100))); // Light yellow
    
    m_bodyFatObeseArea = new QAreaSeries(obeseUpper, obeseLower);
    m_bodyFatObeseArea->setName("Obese (25%+)");
    m_bodyFatObeseArea->setBrush(QBrush(QColor(255, 182, 193, 100))); // Light pink
    
    m_bodyFatChart->addSeries(m_bodyFatEssentialArea);
    m_bodyFatChart->addSeries(m_bodyFatAthleticArea);
    m_bodyFatChart->addSeries(m_bodyFatFitnessArea);
    m_bodyFatChart->addSeries(m_bodyFatAverageArea);
    m_bodyFatChart->addSeries(m_bodyFatObeseArea);
    m_bodyFatChart->addSeries(m_bodyFatSeries);
    
    // Setup axes with manual configuration like weight chart
    QDateTimeAxis *axisX = new QDateTimeAxis();
    axisX->setTickCount(10);
    axisX->setFormat("MMM dd");
    axisX->setTitleText("Date");
    m_bodyFatChart->addAxis(axisX, Qt::AlignBottom);
    
    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Body Fat (%)");
    axisY->setRange(0, 40); // Range to show all categories with more headroom
    m_bodyFatChart->addAxis(axisY, Qt::AlignLeft);
    
    // Attach axes to all series
    m_bodyFatEssentialArea->attachAxis(axisX);
    m_bodyFatEssentialArea->attachAxis(axisY);
    m_bodyFatAthleticArea->attachAxis(axisX);
    m_bodyFatAthleticArea->attachAxis(axisY);
    m_bodyFatFitnessArea->attachAxis(axisX);
    m_bodyFatFitnessArea->attachAxis(axisY);
    m_bodyFatAverageArea->attachAxis(axisX);
    m_bodyFatAverageArea->attachAxis(axisY);
    m_bodyFatObeseArea->attachAxis(axisX);
    m_bodyFatObeseArea->attachAxis(axisY);
    m_bodyFatSeries->attachAxis(axisX);
    m_bodyFatSeries->attachAxis(axisY);
    
    m_bodyFatChartView = new QChartView(m_bodyFatChart);
    m_bodyFatChartView->setRenderHint(QPainter::Antialiasing);
    
    m_chartTabs->addTab(m_bodyFatChartView, "Body Fat");
}

void AnalyticsTab::setupWaistChart()
{
    m_waistChart = new QChart();
    m_waistSeries = new QLineSeries();
    
    setupChartAppearance(m_waistChart, "Waist Circumference");
    m_waistChart->addSeries(m_waistSeries);
    
    // Setup axes with manual configuration like weight chart
    QDateTimeAxis *axisX = new QDateTimeAxis();
    axisX->setTickCount(10);
    axisX->setFormat("MMM dd");
    axisX->setTitleText("Date");
    m_waistChart->addAxis(axisX, Qt::AlignBottom);
    m_waistSeries->attachAxis(axisX);
    
    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Waist Circumference (in)");
    m_waistChart->addAxis(axisY, Qt::AlignLeft);
    m_waistSeries->attachAxis(axisY);
    
    m_waistChartView = new QChartView(m_waistChart);
    m_waistChartView->setRenderHint(QPainter::Antialiasing);
    
    m_chartTabs->addTab(m_waistChartView, "Waist");
}

void AnalyticsTab::setupChartAppearance(QChart *chart, const QString &title)
{
    chart->setTitle(title);
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
}

QList<BodyComposition> AnalyticsTab::getFilteredData()
{
    int days = m_timeRangeCombo->currentData().toInt();
    
    QList<BodyComposition> data;
    if (days == -1) {
        // All time
        data = m_dataManager->getAllBodyCompositionSorted();
    } else {
        // Specific time range
        QDate endDate = QDate::currentDate();
        QDate startDate = endDate.addDays(-days);
        data = m_dataManager->getBodyCompositionRange(startDate, endDate);
    }
    
    qDebug() << "getFilteredData: Got" << data.size() << "body composition entries";
    for (const BodyComposition &composition : data) {
        qDebug() << "  Date:" << composition.date().toString() << "Weight:" << composition.weight() 
                 << "BMI:" << composition.bmi() << "Valid:" << composition.date().isValid();
    }
    
    return data;
}

void AnalyticsTab::setDateAxisRange(QDateTimeAxis* dateAxis, const QList<BodyComposition>& data)
{
    if (!dateAxis) return;
    
    // Calculate the requested time window from dropdown selection
    int days = m_timeRangeCombo->currentData().toInt();
    QDate endDate = QDate::currentDate();
    QDate startDate;
    
    if (days == -1) {
        // "All Time" - use data range if available, otherwise show last year
        if (!data.isEmpty()) {
            startDate = data.first().date();
            endDate = data.last().date();
        } else {
            startDate = endDate.addDays(-365);
        }
    } else {
        // Show the requested time window
        startDate = endDate.addDays(-days);
    }
    
    qint64 minTimestamp = startDate.startOfDay().toMSecsSinceEpoch();
    qint64 maxTimestamp = endDate.startOfDay().toMSecsSinceEpoch();
    QDateTime minDateTime = QDateTime::fromMSecsSinceEpoch(minTimestamp);
    QDateTime maxDateTime = QDateTime::fromMSecsSinceEpoch(maxTimestamp);
    dateAxis->setRange(minDateTime, maxDateTime);
}

void AnalyticsTab::onDataChanged()
{
    refreshCharts();
}

void AnalyticsTab::onTimeRangeChanged()
{
    refreshCharts();
}

void AnalyticsTab::refreshCharts()
{
    printf("refreshCharts called\n");
    updateWeightChart();
    updateBMIChart();
    updateBodyFatChart();
    updateWaistChart();
}

void AnalyticsTab::updateWeightChart()
{
    if (!m_weightSeries) {
        printf("updateWeightChart: m_weightSeries is null\n");
        return;
    }
    
    m_weightSeries->clear();
    
    QList<BodyComposition> data = getFilteredData();
    printf("updateWeightChart: got %d data points\n", data.size());
    if (data.isEmpty()) {
        printf("updateWeightChart: no data, returning\n");
        return;
    }
    
    int pointsAdded = 0;
    for (const BodyComposition &composition : data) {
        if (composition.weight() > 0) {
            QDate date = composition.date();
            if (!date.isValid()) {
                printf("updateWeightChart: Invalid date found, skipping\n");
                continue;
            }
            qint64 timestamp = date.startOfDay().toMSecsSinceEpoch();
            
            // Debug: verify our timestamp calculation
            QDateTime backToDateTime = QDateTime::fromMSecsSinceEpoch(timestamp);
            printf("updateWeightChart: Original date: %s\n", date.toString().toLocal8Bit().data());
            printf("updateWeightChart: Timestamp: %lld\n", timestamp);
            printf("updateWeightChart: Back to QDateTime: %s\n", backToDateTime.toString().toLocal8Bit().data());
            
            m_weightSeries->append(timestamp, composition.weight());
            pointsAdded++;
            printf("updateWeightChart: Added point - Weight: %.1f\n", composition.weight());
        }
    }
    printf("updateWeightChart: Total points added to series: %d\n", pointsAdded);
    printf("updateWeightChart: Series now has %d points\n", m_weightSeries->count());
    
    // Since we have manually set up axes, just ensure they have proper ranges
    if (m_weightChart && pointsAdded > 0) {
        QList<QAbstractAxis*> axesX = m_weightChart->axes(Qt::Horizontal);
        QList<QAbstractAxis*> axesY = m_weightChart->axes(Qt::Vertical);
        
        printf("updateWeightChart: Chart has %d horizontal axes and %d vertical axes\n", 
               axesX.size(), axesY.size());
               
        // Set axis ranges based on requested time window
        if (!axesX.isEmpty() && !axesY.isEmpty()) {
            printf("updateWeightChart: Setting ranges based on requested time window\n");
            
            // Set the datetime axis range using helper function
            if (QDateTimeAxis* dateAxis = qobject_cast<QDateTimeAxis*>(axesX.first())) {
                setDateAxisRange(dateAxis, data);
            }
            
            // Calculate weight range from available data (with reasonable defaults)
            if (QValueAxis* valueAxis = qobject_cast<QValueAxis*>(axesY.first())) {
                if (!data.isEmpty()) {
                    double minWeight = 999, maxWeight = 0;
                    for (const BodyComposition &comp : data) {
                        if (comp.weight() > 0) {
                            minWeight = qMin(minWeight, comp.weight());
                            maxWeight = qMax(maxWeight, comp.weight());
                        }
                    }
                    if (minWeight < 999) {
                        // Add padding to the range
                        double padding = qMax(5.0, (maxWeight - minWeight) * 0.1);
                        valueAxis->setRange(minWeight - padding, maxWeight + padding);
                        printf("updateWeightChart: Set weight axis range %.1f-%.1f lbs\n", 
                               minWeight - padding, maxWeight + padding);
                    } else {
                        valueAxis->setRange(150, 250); // Default range
                    }
                } else {
                    valueAxis->setRange(150, 250); // Default range when no data
                }
            }
        }
    }
}

void AnalyticsTab::updateBMIChart()
{
    if (!m_bmiSeries) return;
    
    m_bmiSeries->clear();
    
    QList<BodyComposition> data = getFilteredData();
    if (data.isEmpty()) {
        return;
    }
    
    printf("updateBMIChart: got %d data points\n", data.size());
    
    // Update BMI category background areas
    if (!data.isEmpty()) {
        qint64 startTime = data.first().date().startOfDay().toMSecsSinceEpoch();
        qint64 endTime = data.last().date().startOfDay().toMSecsSinceEpoch();
        
        // Clear and recreate area series
        auto underweightUpper = qobject_cast<QLineSeries*>(m_underweightArea->upperSeries());
        auto underweightLower = qobject_cast<QLineSeries*>(m_underweightArea->lowerSeries());
        auto normalUpper = qobject_cast<QLineSeries*>(m_normalArea->upperSeries());
        auto normalLower = qobject_cast<QLineSeries*>(m_normalArea->lowerSeries());
        auto overweightUpper = qobject_cast<QLineSeries*>(m_overweightArea->upperSeries());
        auto overweightLower = qobject_cast<QLineSeries*>(m_overweightArea->lowerSeries());
        auto obeseUpper = qobject_cast<QLineSeries*>(m_obeseArea->upperSeries());
        auto obeseLower = qobject_cast<QLineSeries*>(m_obeseArea->lowerSeries());
        
        if (underweightUpper && underweightLower) {
            underweightUpper->clear();
            underweightLower->clear();
            underweightUpper->append(startTime, 18.5);
            underweightUpper->append(endTime, 18.5);
            underweightLower->append(startTime, 15);
            underweightLower->append(endTime, 15);
        }
        
        if (normalUpper && normalLower) {
            normalUpper->clear();
            normalLower->clear();
            normalUpper->append(startTime, 24.9);
            normalUpper->append(endTime, 24.9);
            normalLower->append(startTime, 18.5);
            normalLower->append(endTime, 18.5);
        }
        
        if (overweightUpper && overweightLower) {
            overweightUpper->clear();
            overweightLower->clear();
            overweightUpper->append(startTime, 29.9);
            overweightUpper->append(endTime, 29.9);
            overweightLower->append(startTime, 25.0);
            overweightLower->append(endTime, 25.0);
        }
        
        if (obeseUpper && obeseLower) {
            obeseUpper->clear();
            obeseLower->clear();
            obeseUpper->append(startTime, 40);
            obeseUpper->append(endTime, 40);
            obeseLower->append(startTime, 30.0);
            obeseLower->append(endTime, 30.0);
        }
    }
    
    // Add BMI data points
    int pointsAdded = 0;
    for (const BodyComposition &composition : data) {
        if (composition.bmi() > 0) {
            QDate date = composition.date();
            if (!date.isValid()) {
                continue;
            }
            qint64 timestamp = date.startOfDay().toMSecsSinceEpoch();
            m_bmiSeries->append(timestamp, composition.bmi());
            pointsAdded++;
        }
    }
    
    printf("updateBMIChart: Added %d points to series\n", pointsAdded);
    
    // Dynamically set axis ranges based on filtered data
    if (m_bmiChart && pointsAdded > 0 && !data.isEmpty()) {
        QList<QAbstractAxis*> axesX = m_bmiChart->axes(Qt::Horizontal);
        QList<QAbstractAxis*> axesY = m_bmiChart->axes(Qt::Vertical);
        
        if (!axesX.isEmpty()) {
            if (QDateTimeAxis* dateAxis = qobject_cast<QDateTimeAxis*>(axesX.first())) {
                setDateAxisRange(dateAxis, data);
            }
        }
        
        if (!axesY.isEmpty()) {
            if (QValueAxis* valueAxis = qobject_cast<QValueAxis*>(axesY.first())) {
                // Keep BMI range at 15-40 for context with the colored zones
                valueAxis->setRange(15, 40);
            }
        }
    }
}

void AnalyticsTab::updateBodyFatChart()
{
    if (!m_bodyFatSeries) return;
    
    m_bodyFatSeries->clear();
    
    QList<BodyComposition> data = getFilteredData();
    if (data.isEmpty()) {
        return;
    }
    
    int pointsAdded = 0;
    for (const BodyComposition &composition : data) {
        if (composition.bodyFatPercentage() > 0) {
            QDate date = composition.date();
            if (!date.isValid()) {
                continue;
            }
            qint64 timestamp = date.startOfDay().toMSecsSinceEpoch();
            m_bodyFatSeries->append(timestamp, composition.bodyFatPercentage());
            pointsAdded++;
        }
    }
    
    printf("updateBodyFatChart: Added %d points to series\n", pointsAdded);
    
    // Update body fat category background areas
    if (!data.isEmpty()) {
        // Get the time range for this chart update
        int days = m_timeRangeCombo->currentData().toInt();
        QDate endDate = QDate::currentDate();
        QDate startDate;
        qint64 startTime, endTime;
        
        if (days == -1) {
            // Use data range for "All Time"  
            startTime = data.first().date().startOfDay().toMSecsSinceEpoch();
            endTime = data.last().date().startOfDay().toMSecsSinceEpoch();
        } else {
            // Use requested time window
            startDate = endDate.addDays(-days);
            startTime = startDate.startOfDay().toMSecsSinceEpoch();
            endTime = endDate.startOfDay().toMSecsSinceEpoch();
        }
        
        // Clear and recreate body fat area series
        auto essentialUpper = qobject_cast<QLineSeries*>(m_bodyFatEssentialArea->upperSeries());
        auto essentialLower = qobject_cast<QLineSeries*>(m_bodyFatEssentialArea->lowerSeries());
        auto athleticUpper = qobject_cast<QLineSeries*>(m_bodyFatAthleticArea->upperSeries());
        auto athleticLower = qobject_cast<QLineSeries*>(m_bodyFatAthleticArea->lowerSeries());
        auto fitnessUpper = qobject_cast<QLineSeries*>(m_bodyFatFitnessArea->upperSeries());
        auto fitnessLower = qobject_cast<QLineSeries*>(m_bodyFatFitnessArea->lowerSeries());
        auto averageUpper = qobject_cast<QLineSeries*>(m_bodyFatAverageArea->upperSeries());
        auto averageLower = qobject_cast<QLineSeries*>(m_bodyFatAverageArea->lowerSeries());
        auto obeseUpper = qobject_cast<QLineSeries*>(m_bodyFatObeseArea->upperSeries());
        auto obeseLower = qobject_cast<QLineSeries*>(m_bodyFatObeseArea->lowerSeries());
        
        // Essential fat (2-5%)
        if (essentialUpper && essentialLower) {
            essentialUpper->clear();
            essentialLower->clear();
            essentialUpper->append(startTime, 5.0);
            essentialUpper->append(endTime, 5.0);
            essentialLower->append(startTime, 2.0);
            essentialLower->append(endTime, 2.0);
        }
        
        // Athletic (6-13%)
        if (athleticUpper && athleticLower) {
            athleticUpper->clear();
            athleticLower->clear();
            athleticUpper->append(startTime, 13.0);
            athleticUpper->append(endTime, 13.0);
            athleticLower->append(startTime, 6.0);
            athleticLower->append(endTime, 6.0);
        }
        
        // Fitness (14-17%)
        if (fitnessUpper && fitnessLower) {
            fitnessUpper->clear();
            fitnessLower->clear();
            fitnessUpper->append(startTime, 17.0);
            fitnessUpper->append(endTime, 17.0);
            fitnessLower->append(startTime, 14.0);
            fitnessLower->append(endTime, 14.0);
        }
        
        // Average (18-24%)
        if (averageUpper && averageLower) {
            averageUpper->clear();
            averageLower->clear();
            averageUpper->append(startTime, 24.0);
            averageUpper->append(endTime, 24.0);
            averageLower->append(startTime, 18.0);
            averageLower->append(endTime, 18.0);
        }
        
        // Obese (25%+)
        if (obeseUpper && obeseLower) {
            obeseUpper->clear();
            obeseLower->clear();
            obeseUpper->append(startTime, 40.0);
            obeseUpper->append(endTime, 40.0);
            obeseLower->append(startTime, 25.0);
            obeseLower->append(endTime, 25.0);
        }
    }
    
    // Dynamically set axis ranges based on filtered data
    if (m_bodyFatChart && !data.isEmpty()) {
        QList<QAbstractAxis*> axesX = m_bodyFatChart->axes(Qt::Horizontal);
        QList<QAbstractAxis*> axesY = m_bodyFatChart->axes(Qt::Vertical);
        
        if (!axesX.isEmpty()) {
            if (QDateTimeAxis* dateAxis = qobject_cast<QDateTimeAxis*>(axesX.first())) {
                setDateAxisRange(dateAxis, data);
            }
        }
        
        if (!axesY.isEmpty()) {
            if (QValueAxis* valueAxis = qobject_cast<QValueAxis*>(axesY.first())) {
                // Keep the 0-40% range to show all body fat categories with headroom
                valueAxis->setRange(0, 40);
            }
        }
    }
}

void AnalyticsTab::updateWaistChart()
{
    if (!m_waistSeries) return;
    
    m_waistSeries->clear();
    
    QList<BodyComposition> data = getFilteredData();
    if (data.isEmpty()) {
        return;
    }
    
    int pointsAdded = 0;
    for (const BodyComposition &composition : data) {
        if (composition.waistCircumference() > 0) {
            QDate date = composition.date();
            if (!date.isValid()) {
                continue;
            }
            qint64 timestamp = date.startOfDay().toMSecsSinceEpoch();
            m_waistSeries->append(timestamp, composition.waistCircumference());
            pointsAdded++;
        }
    }
    
    printf("updateWaistChart: Added %d points to series\n", pointsAdded);
    
    // Dynamically set axis ranges based on filtered data
    if (m_waistChart && pointsAdded > 0 && !data.isEmpty()) {
        QList<QAbstractAxis*> axesX = m_waistChart->axes(Qt::Horizontal);
        QList<QAbstractAxis*> axesY = m_waistChart->axes(Qt::Vertical);
        
        if (!axesX.isEmpty()) {
            if (QDateTimeAxis* dateAxis = qobject_cast<QDateTimeAxis*>(axesX.first())) {
                QDate firstDate = data.first().date();
                QDate lastDate = data.last().date();
                qint64 minTimestamp = firstDate.startOfDay().toMSecsSinceEpoch();
                qint64 maxTimestamp = lastDate.startOfDay().toMSecsSinceEpoch();
                QDateTime minDateTime = QDateTime::fromMSecsSinceEpoch(minTimestamp);
                QDateTime maxDateTime = QDateTime::fromMSecsSinceEpoch(maxTimestamp);
                dateAxis->setRange(minDateTime, maxDateTime);
            }
        }
        
        if (!axesY.isEmpty()) {
            if (QValueAxis* valueAxis = qobject_cast<QValueAxis*>(axesY.first())) {
                // Calculate waist range from data
                double minWaist = 999, maxWaist = 0;
                for (const BodyComposition &comp : data) {
                    if (comp.waistCircumference() > 0) {
                        minWaist = qMin(minWaist, comp.waistCircumference());
                        maxWaist = qMax(maxWaist, comp.waistCircumference());
                    }
                }
                if (minWaist < 999) {
                    double padding = (maxWaist - minWaist) * 0.1;
                    valueAxis->setRange(minWaist - padding, maxWaist + padding);
                } else {
                    valueAxis->setRange(30, 50); // Default if no data
                }
            }
        }
    }
}