#ifndef BODYCOMPOSITIONVIEW_H
#define BODYCOMPOSITIONVIEW_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include "models/bodycomposition.h"

class BodyCompositionView : public QWidget
{
    Q_OBJECT

public:
    explicit BodyCompositionView(QWidget *parent = nullptr);
    
    void setData(const BodyComposition &data);
    void clear();

signals:
    void editRequested();
    void deleteRequested();

private slots:
    void onEditClicked();
    void onDeleteClicked();

private:
    void setupUI();
    void updateDisplay();
    
    BodyComposition m_data;
    
    // UI Elements
    QLabel *m_weightLabel;
    QLabel *m_waistLabel;
    QLabel *m_heightLabel;
    QLabel *m_neckLabel;
    QLabel *m_notesLabel;
    QLabel *m_timestampLabel;
    QPushButton *m_editButton;
    QPushButton *m_deleteButton;
    QGroupBox *m_dataGroup;
};

#endif // BODYCOMPOSITIONVIEW_H 