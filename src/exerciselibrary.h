#ifndef EXERCISELIBRARY_H
#define EXERCISELIBRARY_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QMessageBox>
#include <QList>
#include "models/datamanager.h"
#include "models/exercise.h"

class ExerciseLibrary : public QWidget
{
    Q_OBJECT

public:
    explicit ExerciseLibrary(DataManager *dataManager, QWidget *parent = nullptr);

signals:
    void exerciseAdded();
    void exerciseUpdated();
    void exerciseDeleted();

private slots:
    void addExercise();
    void editExercise();
    void deleteExercise();
    void clearForm();
    void refreshTable();

private:
    void setupUI();
    void setupTable();
    void populateTable();
    bool validateForm();
    void showError(const QString &message);
    void showSuccess(const QString &message);
    
    DataManager *m_dataManager;
    
    // UI elements
    QLineEdit *m_nameEdit;
    QComboBox *m_categoryCombo;
    QPushButton *m_addButton;
    QPushButton *m_editButton;
    QPushButton *m_deleteButton;
    QPushButton *m_clearButton;
    QTableWidget *m_exercisesTable;
    
    // Current exercise being edited
    int m_currentExerciseId;
    bool m_isEditing;
};

#endif // EXERCISELIBRARY_H 