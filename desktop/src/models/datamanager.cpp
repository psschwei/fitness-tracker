#include "datamanager.h"
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QStandardPaths>
#include <QDebug>

DataManager::DataManager(QObject *parent)
    : QObject(parent)
{
    m_dataFilePath = getDataFilePath();
    ensureDataDirectory();
    loadData();
}

bool DataManager::saveBodyComposition(const BodyComposition &data)
{
    m_bodyCompositionData[data.date()] = data;
    bool success = saveData();
    if (success) {
        emit dataChanged();
    }
    return success;
}

BodyComposition DataManager::loadBodyComposition(const QDate &date)
{
    return m_bodyCompositionData.value(date, BodyComposition());
}

bool DataManager::hasBodyComposition(const QDate &date) const
{
    return m_bodyCompositionData.contains(date);
}

void DataManager::deleteBodyComposition(const QDate &date)
{
    if (m_bodyCompositionData.remove(date) > 0) {
        saveData();
        emit dataChanged();
    }
}

QList<QDate> DataManager::getBodyCompositionDates() const
{
    return m_bodyCompositionData.keys();
}

bool DataManager::loadData()
{
    QFile file(m_dataFilePath);
    if (!file.exists()) {
        return true; // No data file yet, that's okay
    }
    
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open data file for reading:" << m_dataFilePath;
        return false;
    }
    
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &error);
    if (error.error != QJsonParseError::NoError) {
        qWarning() << "Failed to parse data file:" << error.errorString();
        return false;
    }
    
    QJsonObject root = doc.object();
    if (root.contains("bodyComposition")) {
        QJsonArray array = root["bodyComposition"].toArray();
        m_bodyCompositionData.clear();
        
        for (const QJsonValue &value : array) {
            QJsonObject obj = value.toObject();
            BodyComposition composition = BodyComposition::fromJson(obj);
            if (composition.date().isValid()) {
                m_bodyCompositionData[composition.date()] = composition;
            }
        }
    }
    
    return true;
}

bool DataManager::saveData()
{
    QFile file(m_dataFilePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Failed to open data file for writing:" << m_dataFilePath;
        return false;
    }
    
    QJsonObject root;
    QJsonArray bodyCompositionArray;
    
    for (const BodyComposition &composition : m_bodyCompositionData.values()) {
        bodyCompositionArray.append(composition.toJson());
    }
    
    root["bodyComposition"] = bodyCompositionArray;
    
    QJsonDocument doc(root);
    qint64 bytesWritten = file.write(doc.toJson());
    
    if (bytesWritten == -1) {
        qWarning() << "Failed to write data file";
        return false;
    }
    
    return true;
}

QString DataManager::getDataFilePath() const
{
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    return appDataPath + "/fitness-tracker/data.json";
}

void DataManager::ensureDataDirectory() const
{
    QDir dir = QFileInfo(m_dataFilePath).dir();
    if (!dir.exists()) {
        dir.mkpath(".");
    }
} 