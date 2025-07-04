#include "calculations.h"

QString Calculations::getBMICategory(double bmi)
{
    if (bmi < 18.5) {
        return "Underweight";
    } else if (bmi < 25) {
        return "Normal weight";
    } else if (bmi < 30) {
        return "Overweight";
    } else {
        return "Obese";
    }
}

QString Calculations::formatBMI(double bmi)
{
    if (bmi <= 0.0) {
        return "Not available";
    }
    QString category = getBMICategory(bmi);
    return QString("%1 (%2)").arg(bmi, 0, 'f', 1).arg(category);
}

QString Calculations::getBodyFatCategory(double bodyFat, bool isMale)
{
    if (isMale) {
        if (bodyFat < 6) return "Essential fat";
        if (bodyFat < 14) return "Athlete";
        if (bodyFat < 18) return "Fitness";
        if (bodyFat < 25) return "Average";
        return "Obese";
    } else {
        if (bodyFat < 14) return "Essential fat";
        if (bodyFat < 21) return "Athlete";
        if (bodyFat < 25) return "Fitness";
        if (bodyFat < 32) return "Average";
        return "Obese";
    }
}

QString Calculations::formatBodyFat(double bodyFat, bool isMale)
{
    if (bodyFat <= 0.0) {
        return "Not available";
    }
    QString category = getBodyFatCategory(bodyFat, isMale);
    return QString("%1% (%2)").arg(bodyFat, 0, 'f', 1).arg(category);
}

QString Calculations::getBodyFatCategoryColor(const QString &category)
{
    if (category == "Obese" || category == "Essential fat") {
        return "#dc3545"; // Red
    } else if (category == "Average") {
        return "#28a745"; // Green
    } else if (category == "Fitness" || category == "Athlete") {
        return "#007bff"; // Blue
    } else {
        return "#333"; // Default dark gray
    }
}

QString Calculations::getBMICategoryColor(const QString &category)
{
    if (category == "Obese" || category == "Underweight") {
        return "#dc3545"; // Red
    } else if (category == "Overweight") {
        return "#fd7e14"; // Orange
    } else if (category == "Normal weight") {
        return "#28a745"; // Green
    } else {
        return "#333"; // Default dark gray
    }
} 