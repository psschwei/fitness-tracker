#include "bodycompositionview.h"
#include <QMessageBox>

BodyCompositionView::BodyCompositionView(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    
    // Connect button signals
    connect(m_editButton, &QPushButton::clicked, this, &BodyCompositionView::onEditClicked);
    connect(m_deleteButton, &QPushButton::clicked, this, &BodyCompositionView::onDeleteClicked);
}

void BodyCompositionView::setupUI()
{
    // Create data display labels
    m_weightLabel = new QLabel(this);
    m_waistLabel = new QLabel(this);
    m_heightLabel = new QLabel(this);
    m_neckLabel = new QLabel(this);
    m_notesLabel = new QLabel(this);
    m_timestampLabel = new QLabel(this);
    
    // Style the labels
    QString labelStyle = "QLabel { font-size: 14px; padding: 5px; }";
    m_weightLabel->setStyleSheet(labelStyle);
    m_waistLabel->setStyleSheet(labelStyle);
    m_heightLabel->setStyleSheet(labelStyle);
    m_neckLabel->setStyleSheet(labelStyle);
    m_notesLabel->setStyleSheet(labelStyle);
    m_timestampLabel->setStyleSheet("QLabel { font-size: 12px; color: #666; padding: 5px; }");
    
    m_notesLabel->setWordWrap(true);
    
    // Create buttons
    m_editButton = new QPushButton("Edit", this);
    m_deleteButton = new QPushButton("Delete", this);
    m_deleteButton->setStyleSheet("QPushButton { background-color: #dc3545; color: white; border: none; padding: 8px 16px; border-radius: 4px; }");
    
    // Create data group
    m_dataGroup = new QGroupBox("Body Composition Data", this);
    QGridLayout *dataLayout = new QGridLayout(m_dataGroup);
    
    dataLayout->addWidget(new QLabel("Weight:"), 0, 0);
    dataLayout->addWidget(m_weightLabel, 0, 1);
    
    dataLayout->addWidget(new QLabel("Waist:"), 1, 0);
    dataLayout->addWidget(m_waistLabel, 1, 1);
    
    dataLayout->addWidget(new QLabel("Height:"), 2, 0);
    dataLayout->addWidget(m_heightLabel, 2, 1);
    
    dataLayout->addWidget(new QLabel("Neck:"), 3, 0);
    dataLayout->addWidget(m_neckLabel, 3, 1);
    
    dataLayout->addWidget(new QLabel("Notes:"), 4, 0);
    dataLayout->addWidget(m_notesLabel, 4, 1);
    
    dataLayout->addWidget(m_timestampLabel, 5, 0, 1, 2, Qt::AlignRight);
    
    // Create button layout
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_deleteButton);
    buttonLayout->addWidget(m_editButton);
    
    // Create main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(40, 20, 40, 20);
    mainLayout->addWidget(m_dataGroup);
    mainLayout->addStretch();
    mainLayout->addLayout(buttonLayout);
    
    setLayout(mainLayout);
}

void BodyCompositionView::setData(const BodyComposition &data)
{
    m_data = data;
    updateDisplay();
}

void BodyCompositionView::clear()
{
    m_data = BodyComposition();
    updateDisplay();
}

void BodyCompositionView::updateDisplay()
{
    if (m_data.isEmpty()) {
        m_dataGroup->setVisible(false);
        m_editButton->setVisible(false);
        m_deleteButton->setVisible(false);
        return;
    }
    
    m_dataGroup->setVisible(true);
    m_editButton->setVisible(true);
    m_deleteButton->setVisible(true);
    
    m_weightLabel->setText(QString("%1 lbs").arg(m_data.weight(), 0, 'f', 1));
    m_waistLabel->setText(QString("%1 inches").arg(m_data.waistCircumference(), 0, 'f', 1));
    m_heightLabel->setText(QString("%1 inches").arg(m_data.height(), 0, 'f', 1));
    m_neckLabel->setText(QString("%1 inches").arg(m_data.neckCircumference(), 0, 'f', 1));
    
    if (m_data.notes().isEmpty()) {
        m_notesLabel->setText("No notes");
        m_notesLabel->setStyleSheet("QLabel { font-size: 14px; color: #666; font-style: italic; padding: 5px; }");
    } else {
        m_notesLabel->setText(m_data.notes());
        m_notesLabel->setStyleSheet("QLabel { font-size: 14px; padding: 5px; }");
    }
    
    m_timestampLabel->setText(QString("Last updated: %1").arg(m_data.timestamp().toString("MMM dd, yyyy h:mm AP")));
}

void BodyCompositionView::onEditClicked()
{
    emit editRequested();
}

void BodyCompositionView::onDeleteClicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, 
        "Confirm Delete", 
        "Are you sure you want to delete this body composition entry?",
        QMessageBox::Yes | QMessageBox::No
    );
    
    if (reply == QMessageBox::Yes) {
        emit deleteRequested();
    }
} 