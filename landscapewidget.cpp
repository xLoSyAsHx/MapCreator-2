#include "landscapewidget.h"

#include <QMessageBox>

#include <QPushButton>
#include <QFileDialog>
#include "landscapemanager.h"


void LandscapeWidget::onCreateLandscape()
{
    bool convertStatus = false;
    int width = m_landscapeWidth->text().toInt(&convertStatus, 10);
    int height =  m_landscapeHeight->text().toInt(&convertStatus, 10);

    LandscapeManager::Instance().createLandscape(width, height, 1);

    m_landscapeWidth->setEnabled(false);
    m_landscapeHeight->setEnabled(false);

    m_createButton->setText("Delete");
    disconnect(m_createButton, QPushButton::clicked, this, onCreateLandscape);
    connect(m_createButton, QPushButton::clicked, this, onDeleteLandscape);
}

void LandscapeWidget::onDeleteLandscape()
{
    QMessageBox::StandardButton answer =
            QMessageBox::question(
                this,
                "Warning! Deleting landscape",
                "Do you really want to delete landscape? You can't restore it.");

    if (answer == QMessageBox::StandardButton::No)
        return;

    LandscapeManager::Instance().destroy();

    m_landscapeWidth->setEnabled(true);
    m_landscapeHeight->setEnabled(true);

    m_createButton->setText("Create");
    disconnect(m_createButton, QPushButton::clicked, this, onDeleteLandscape);
    connect(m_createButton, QPushButton::clicked, this, onCreateLandscape);
}

void LandscapeWidget::onAddTexture()
{
    QString pathToFile = QFileDialog::getOpenFileName(0, "Choose file");

    if (!pathToFile.isEmpty())
    {
        QPixmap *image = new QPixmap(pathToFile);
        LandscapeManager::Instance().addTexture(
                    aiTextureType_DIFFUSE, new QOpenGLTexture(image->toImage()));
    }
}

void LandscapeWidget::onSculptToolStatusChanged()
{
    m_system->onSculptToolStatusChanged();
}

LandscapeWidget::LandscapeWidget(System* system) :
    m_system(system)
{
    int fontSize = 13;

    m_widget = new QWidget(this);
    QVBoxLayout *mainLayout_v = new QVBoxLayout;

    // Horizontal layout 1
    QHBoxLayout *layout1_h = new QHBoxLayout;
    layout1_h->setAlignment(Qt::AlignCenter | Qt::AlignTop);

    QLabel *landscapeWidgetName = new QLabel("Landscape manager");
    landscapeWidgetName->setStyleSheet(QString("font-size: %1px").arg(fontSize + 2));
    layout1_h->addWidget(landscapeWidgetName);


    // Horizontal layout 2
    QHBoxLayout *layout2_h = new QHBoxLayout;
    layout1_h->setAlignment(Qt::AlignCenter | Qt::AlignTop);

    QLabel *emptyLabel = new QLabel("");
    emptyLabel->setStyleSheet(QString("font-size: %1px").arg(fontSize + 2));
    layout2_h->addWidget(emptyLabel);


    // Grid layout
    QGridLayout *gridLayout = new QGridLayout;
    gridLayout ->setAlignment(Qt::AlignTop);

    // Grid row 0
    QLabel *widthLabel = new QLabel("Width");
    widthLabel->setStyleSheet(QString("font-size: %1px").arg(fontSize));
    gridLayout->addWidget(widthLabel, 0, 0);

    QLabel *heightLabel = new QLabel("Height");
    heightLabel->setStyleSheet(QString("font-size: %1px").arg(fontSize));
    gridLayout->addWidget(heightLabel, 0, 2);


    // Grid row 1
    m_landscapeWidth = new QLineEdit;
    m_landscapeWidth->setInputMask("9999");
    m_landscapeWidth->setText("0");
    m_landscapeWidth->setAlignment(Qt::AlignCenter);
    m_landscapeWidth->setStyleSheet(QString("font-size: %1px").arg(fontSize));
    gridLayout->addWidget(m_landscapeWidth, 1, 0);

    QLabel *xLabel = new QLabel(" x ");
    xLabel->setStyleSheet(QString("font-size: %1px").arg(fontSize));
    gridLayout->addWidget(xLabel, 1, 1);

    m_landscapeHeight = new QLineEdit;
    m_landscapeHeight->setInputMask("9999");
    m_landscapeHeight->setText("0");
    m_landscapeHeight->setAlignment(Qt::AlignCenter);
    m_landscapeHeight->setStyleSheet(QString("font-size: %1px").arg(fontSize));
    gridLayout->addWidget(m_landscapeHeight, 1, 2);

    m_createButton = new QPushButton("Create");
    m_createButton->setStyleSheet(QString("font-size: %1px").arg(fontSize));
    connect(m_createButton, QPushButton::clicked, this, onCreateLandscape);
    gridLayout->addWidget(m_createButton, 1, 3);

    // Horizontal layout 3
    QHBoxLayout *layout3_h = new QHBoxLayout;
    layout3_h->setAlignment(Qt::AlignCenter | Qt::AlignTop);

    QPushButton *addTexture = new QPushButton("Add texture");
    addTexture->setStyleSheet(QString("font-size: %1px").arg(fontSize + 2));
    layout3_h->addWidget(addTexture);
    connect(addTexture, QPushButton::clicked, this, onAddTexture);

    // Horizontal layout 4
    QHBoxLayout *layout4_h = new QHBoxLayout;
    layout4_h->setAlignment(Qt::AlignCenter | Qt::AlignTop);

    QPushButton *enableSculptTool = new QPushButton("Enable landscape sculpt tool");
    enableSculptTool->setStyleSheet(QString("font-size: %1px").arg(fontSize + 2));
    layout4_h->addWidget(enableSculptTool);
    connect(enableSculptTool, QPushButton::clicked, this, onSculptToolStatusChanged);


    // Set up main layout
    mainLayout_v->addLayout(layout1_h);
    mainLayout_v->addLayout(layout2_h);
    mainLayout_v->addLayout(gridLayout);
    mainLayout_v->addLayout(layout3_h);
    mainLayout_v->addLayout(layout4_h);
    mainLayout_v->setAlignment(Qt::AlignTop);

    m_widget->setLayout(mainLayout_v);
    setWidget(m_widget);
    setWindowTitle("Landscape creator");
}

LandscapeWidget::~LandscapeWidget()
{
    delete m_widget;
}
