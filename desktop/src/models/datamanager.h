#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QObject>
#include <QDate>
#include <QMap>
#include "bodycomposition.h"

class DataManager : public QObject
{
    Q_OBJECT

public:
    explicit DataManager(QObject *parent = nullptr);
    
    // Body composition data management
    bool saveBodyComposition(const BodyComposition &data);
    BodyComposition loadBodyComposition(const QDate &date);
    bool hasBodyComposition(const QDate &date) const;
    void deleteBodyComposition(const QDate &date);
    QList<QDate> getBodyCompositionDates() const;
    
    // Data persistence
    bool loadData();
    bool saveData();

signals:
    void dataChanged();

private:
    QString getDataFilePath() const;
    void ensureDataDirectory() const;
    
    QMap<QDate, BodyComposition> m_bodyCompositionData;
    QString m_dataFilePath;
};

#endif // DATAMANAGER_H 