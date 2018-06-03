#include "objects3dlist.h"

void Objects3DList::addButton()
{
    QString pathToFile = QFileDialog::getOpenFileName(0, "Choose file");

    if (!pathToFile.isEmpty())
        addObject(pathToFile);
}

void Objects3DList::addItemToScene(QListWidgetItem *item)
{
    QVariant data = item->data(Qt::UserRole);

    ObjectManager::Instance().addObject(data.toString());
}

Objects3DList::Objects3DList()
{
    int fontSize = 13;

    QVBoxLayout *mainLayout_v = new QVBoxLayout;

    // Horizontal layout 1
    QHBoxLayout *layout1_h = new QHBoxLayout;
    layout1_h->setAlignment(Qt::AlignCenter | Qt::AlignTop);

    QLabel *landscapeWidgetName = new QLabel("Object manager");
    landscapeWidgetName->setStyleSheet(QString("font-size: %1px").arg(fontSize + 2));
    layout1_h->addWidget(landscapeWidgetName);


    // Horizontal layout 2
    QHBoxLayout *layout2_h = new QHBoxLayout;
    layout1_h->setAlignment(Qt::AlignCenter | Qt::AlignTop);

    QLabel *emptyLabel = new QLabel("");
    emptyLabel->setStyleSheet(QString("font-size: %1px").arg(fontSize));
    layout2_h->addWidget(emptyLabel);

    // Vertical layout 3
    QVBoxLayout *layout3_v = new QVBoxLayout;
    layout3_v->setAlignment(Qt::AlignTop);


    QPushButton *button = new QPushButton;
    button->setText("Add object");
    button->setStyleSheet(QString("font-size: %1px").arg(fontSize));
    connect(button, QPushButton::clicked, this, Objects3DList::addButton);
    layout3_v->addWidget(button);

    m_listWidget = new QListWidget();
    layout3_v->addWidget(m_listWidget);


    // Set up main layout
    mainLayout_v->addLayout(layout1_h);
    mainLayout_v->addLayout(layout2_h);
    mainLayout_v->addLayout(layout3_v);

    QWidget *w = new QWidget;
    w->setLayout(mainLayout_v);
    setWidget(w);
    setWindowTitle("Object creator");

    connect(m_listWidget, QListWidget::itemDoubleClicked, this, addItemToScene);
}

Objects3DList::~Objects3DList()
{

}

void Objects3DList::addObject(QString pathToFile)
{
    bool bOk;
    QString modelName = QInputDialog::getText( 0,
                                         "Set model name",
                                         "Name:",
                                         QLineEdit::Normal,
                                         "Model name",
                                         &bOk
                                        );
    if (!bOk)
        return;

    QWidget* wgt = new QWidget;
    QLayout* l = new QHBoxLayout;
    l->addWidget(new QLabel(modelName));
    wgt->setLayout(l);

    QListWidgetItem* item = new QListWidgetItem(m_listWidget);
    QVariant fullFilePath(pathToFile);
    item->setData(Qt::UserRole, fullFilePath);
    item->setSizeHint(wgt->sizeHint());
    m_listWidget->setItemWidget(item, wgt);
    m_listWidget->update();
}
