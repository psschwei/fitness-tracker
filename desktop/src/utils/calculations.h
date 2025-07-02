#ifndef CALCULATIONS_H
#define CALCULATIONS_H

#include <QString>

class Calculations
{
public:
    // BMI categories
    static QString getBMICategory(double bmi);
    static QString formatBMI(double bmi);
    static QString getBMICategoryColor(const QString &category);
    
    // Body fat categories
    static QString getBodyFatCategory(double bodyFat, bool isMale = true);
    static QString formatBodyFat(double bodyFat, bool isMale = true);
    static QString getBodyFatCategoryColor(const QString &category);
};

#endif // CALCULATIONS_H 