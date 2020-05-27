#include "c_elements_display.h"
#include "ui_c_elements_display.h"

QMap<QString,QString> c_elements_display::frToEn_elem = c_elements_display::set_frTo_En_elem();
QMap<QString,int> c_elements_display::frToId_elem = c_elements_display::set_frToId_elem();

c_elements_display::c_elements_display(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::c_elements_display)
{
    ui->setupUi(this);
    this->setStyleSheet(QString("background-color : %1").arg(app_color::grey_blue));
    ui->widget->setStyleSheet(QString("background-color : %1; border-radius:3px;").arg(app_color::dark_blue));

    elems << "Feu" << "Eau" << "Terre" << "Air";

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(ui->widget);
    shadow->setColor(QColor(91, 108, 142, 180));
    shadow->setOffset(2,2);
    ui->widget->setGraphicsEffect(static_cast<QGraphicsEffect*>(shadow));
    elems_chosen << true << true << true << false;
    update_view();
}

c_elements_display::~c_elements_display()
{
    delete ui;
}

QMap<QString,QString> c_elements_display::set_frTo_En_elem() {
    QMap<QString,QString> res;
    res["Feu"] = "fire";
    res["Terre"] = "earth";
    res["Air"] = "wind";
    res["Eau"] = "water";
    return res;
}

QMap<QString,int> c_elements_display::set_frToId_elem() {
    QMap<QString,int> res;
    res["Feu"] = 0;
    res["Terre"] = 1;
    res["Air"] = 2;
    res["Eau"] = 3;
    return res;
}

void c_elements_display::update_view() {
    ui->elem1->setPixmap(QPixmap(QString("images/elements/%1_%2.png").arg(frToEn_elem[elems.at(0)]).arg(elems_chosen.at(0)?"big":"inked")));
    ui->elem1->setBackgroundRole(QPalette::Base);
    ui->elem1->setScaledContents(true);

    ui->elem2->setPixmap(QPixmap(QString("images/elements/%1_%2.png").arg(frToEn_elem[elems.at(1)]).arg(elems_chosen.at(1)?"big":"inked")));
    ui->elem2->setBackgroundRole(QPalette::Base);
    ui->elem2->setScaledContents(true);

    ui->elem3->setPixmap(QPixmap(QString("images/elements/%1_%2.png").arg(frToEn_elem[elems.at(2)]).arg(elems_chosen.at(2)?"big":"inked")));
    ui->elem3->setBackgroundRole(QPalette::Base);
    ui->elem3->setScaledContents(true);

    ui->elem4->setPixmap(QPixmap(QString("images/elements/%1_%2.png").arg(frToEn_elem[elems.at(3)]).arg(elems_chosen.at(3)?"big":"inked")));
    ui->elem4->setBackgroundRole(QPalette::Base);
    ui->elem4->setScaledContents(true);
}

void c_elements_display::setElements(QList<QString> new_elems) {
    for (int i = 0; i < elems_chosen.size(); ++i) {
        elems_chosen.replace(i,false);
    }
    foreach (QString elem, new_elems) {
        elems_chosen.replace(frToId_elem[elem],true);
    }
    update_view();
}

void c_elements_display::mouseDoubleClickEvent(QMouseEvent* /*event*/) {
    c_element_dialog diag(3);
    diag.setElems(elems_chosen);
    if (diag.exec()==QDialog::Accepted) {
        elems_chosen = diag.getElems();
        update_view();
    }
}