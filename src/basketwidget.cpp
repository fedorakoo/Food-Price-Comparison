#include "basketwidget.h"
#include "ui_basketwidget.h"

BasketWidget::BasketWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::BasketWidget)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowStaysOnTopHint |
                         Qt::WindowSystemMenuHint |
                         Qt::FramelessWindowHint|
                         Qt::Window |
                         Qt::Popup);
    this->move(200,100);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->resizeRowsToContents();
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(120);
    ui->tableWidget->setColumnWidth(0, 120);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    updateInfo();

    numberProductOnBasket = 0;
}


void BasketWidget::addProductBasket(Product* product) {
    if(product->getName() != " " && product->getBestPrice() != 0) {
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        QTableWidgetItem *nameProductItem = new QTableWidgetItem;
        nameProductItem->setText(product->getName());
        ui->tableWidget->setItem(numberProductOnBasket + 1, 1, nameProductItem);
        for(int i = 0; i < 7; i++) {
            if(product->getArrPrice()[i] == -1) {
                arrNoProduct[i]++;
                ui->tableWidget->setItem(numberProductOnBasket + 1, 2 + i,
                                         new QTableWidgetItem("-"));
            }
            else {
                arrFullPrice[i] += product->getArrPrice()[i];
                ui->tableWidget->setItem(numberProductOnBasket + 1, 2 + i,
                                         new QTableWidgetItem(QString::number(product->getArrPrice()[i])));
            }
        }
        QPushButton* button = new QPushButton("Удалить");
        ui->tableWidget->setIndexWidget(ui->tableWidget->model()->index(numberProductOnBasket + 1, 9), button);
        QTableWidgetItem* item = new QTableWidgetItem();
        QPixmap pixmap = product->getPicture().scaled(120,120, Qt::KeepAspectRatio);
        QTableWidgetItem *itemPicture = new QTableWidgetItem;
        itemPicture->setData(Qt::DecorationRole, pixmap);

        ui->tableWidget->setItem(numberProductOnBasket + 1, 0, itemPicture);

        connect(button, &QPushButton::clicked, this, &BasketWidget::deleteProduct);
        updateInfo();
        numberProductOnBasket++;
    }
}

BasketWidget::~BasketWidget()
{
    delete ui;
}

void BasketWidget::deleteProduct() {
    if(numberProductOnBasket > 0) {
        QPushButton* button = qobject_cast<QPushButton *>(sender());
        for(int i = 0; i < ui->tableWidget->rowCount(); ++i)
        {
            if(ui->tableWidget->cellWidget(i, 9) == button)
            {
                for(int j = 0; j < 7; j++) {
                QTableWidgetItem *item = ui->tableWidget->item(i, j + 2);
                    if(item->text() == "-") {
                        arrNoProduct[j]--;
                    }
                    else {
                        arrFullPrice[j] -= (item->text()).toDouble();
                    }
                }
                ui->tableWidget->removeRow(i);
                numberProductOnBasket--;
                break;
            }
        }
    }
    updateInfo();
}

void BasketWidget::updateInfo() {
    QTableWidgetItem *nameProductItem = new QTableWidgetItem;
    nameProductItem->setText("Информация о всех продуктах");
    ui->tableWidget->setItem(0,1,nameProductItem);
    for(int i = 0; i < 7; i++) {
        if(arrNoProduct[i] != 0) {
            ui->tableWidget->setItem(0, 2 + i,
                                     new QTableWidgetItem("-"));
        }
        else {
            ui->tableWidget->setItem(0, 2 + i,
                                     new QTableWidgetItem(QString::number(arrFullPrice[i])));
        }
    }
    QPushButton* button = new QPushButton("Удалить всё");
    connect(button, &QPushButton::clicked, this, &BasketWidget::deleteBasket);
    ui->tableWidget->setIndexWidget(ui->tableWidget->model()->index(0, 9), button);
}

void BasketWidget::deleteBasket() {
    while(numberProductOnBasket) {
        for(int j = 0; j < 7; j++) {
            arrFullPrice[j] = 0;
            arrNoProduct[j] = 0;
        }
        ui->tableWidget->removeRow(1);
        numberProductOnBasket--;
    }
    updateInfo();
}
