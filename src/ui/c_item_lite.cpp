#include "c_item_lite.h"
#include "ui_c_item_lite.h"

c_item_lite::c_item_lite(c_dbmanager * database_manager, c_item item, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::c_item_lite) {
    ui->setupUi(this);
    this->setAttribute(Qt::WA_Hover);
    _parent = parent;
    _item = item;
    setStyleSheet(QString(".c_item_lite {background-color: %1; border: 1px solid %2; border-radius: 4px;}").arg(app_color::dark_blue).arg(app_color::color_rarity.at(_item.getRarity())));
    ui->widget_background->setStyleSheet(QString(".QWidget#widget_background{background-color: %1; border : 1px solid %1; border-radius: 4px;}").arg(app_color::dark_blue));
    ui->lvl->setStyleSheet(QString("background-color: %1; border: 1px solid white; border-radius: 5px; color:white; padding: 2px ").arg(app_color::dark));
    ui->image_widget->setStyleSheet(QString(".QWidget#image_widget{background-color:%1; border-top-left-radius: 3px; border-bottom-left-radius: 3px;}").arg(app_color::green_blue));
    ui->icon->setStyleSheet(QString("background-color: %1;").arg(app_color::dark_blue));
    it_display = nullptr;
    _database_manager = database_manager;

#ifdef Q_OS_MACX
    imageDir = QCoreApplication::applicationDirPath() + "/../Resources";
#else
    imageDir = QCoreApplication::applicationDirPath();
#endif

    updateView();
}

c_item_lite::~c_item_lite() {
    delete ui;
    if (it_display != nullptr) {
        it_display->deleteLater();
        it_display = nullptr;
    }
}

void c_item_lite::set_icon() {
    QPixmap image;
    QString type = "NONE";
    if (!_item.isEmpty()) {
        type = mapItemToType[_item.getType().getTitle()];
    }
        if(!type.compare("LEGS")) {
            image = QPixmap(":/images/divers/smalllegs.png");
        } else if (!type.compare("RING")) {
            image = QPixmap(":/images/divers/smallring.png");
        } else if (!type.compare("NECK")) {
            image = QPixmap(":/images/divers/smallneck.png");
        } else if (!type.compare("BACK")) {
            image = QPixmap(":/images/divers/smallback.png");
        } else if (!type.compare("BELT")) {
            image = QPixmap(":/images/divers/smallbelt.png");
        } else if (!type.compare("HEAD")) {
            image = QPixmap(":/images/divers/smallhead.png");
        } else if (!type.compare("CHEST")) {
            image = QPixmap(":/images/divers/smallchest.png");
        } else if (!type.compare("SHOULDERS")) {
            image = QPixmap(":/images/divers/smallshoulders.png");
        } else if (!type.compare("SECOND_HAND")) {
            image = QPixmap(":/images/divers/smallsecondhand.png");
        } else if (!type.compare("PET")) {
            image = QPixmap(":/images/divers/smallpet.png");
        } else if (!type.compare("ACCESSORY")) {
            image = QPixmap(":/images/divers/smallaccessory.png");
        } else if (!type.compare("ONE_HAND_WEAPON")) {
            image = QPixmap(":/images/divers/smallfirsthand.png");
        } else if (!type.compare("TWO_HAND_WEAPON")) {
            image = QPixmap(":/images/divers/smalldoublehand.png");
        } else if (!type.compare("MOUNT")) {
            image = QPixmap(":/images/divers/smallmount.png");
        }
    ui->icon->setPixmap(image);
    ui->icon->setBackgroundRole(QPalette::Base);
    ui->icon->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->icon->setScaledContents(true);
}

bool c_item_lite::event(QEvent *event) {
    if ( event->type() == QEvent::HoverEnter ) {
        QPoint p = this->pos();
        p.setY(p.y()+60);
        p.setX(p.x()+9);
        if (it_display == nullptr) {
            it_display = new c_item_display(_item);
            it_display->setWindowFlags(Qt::ToolTip | Qt::Popup);
            it_display->setFocusPolicy(Qt::NoFocus);
        }
        if ((p.y() + it_display->size().height()) > (_parent->size().height() + 100)) {
            p.setY(p.y()-it_display->size().height()-70);
        }
        QPoint res = _parent->mapToGlobal(p);
        it_display->completeData(_database_manager);
        it_display->move(res);
        it_display->show();
        timer = new QTimer(this);
        connect(timer,&QTimer::timeout,this,&c_item_lite::check_mouse_over);
        timer->start(100);
        emit item_hovered(mapItemToType[_item.getType().getTitle()], res);

    } else if ( event->type() == QEvent::HoverLeave ) {
        it_display->hide();
        emit item_hide(mapItemToType[_item.getType().getTitle()]);

    }
    return QWidget::event(event);
}

void c_item_lite::setDatabase_manager(c_dbmanager *manager) {
    _database_manager = manager;
}

c_item c_item_lite::item() const {
    return _item;
}

void c_item_lite::mouseDoubleClickEvent(QMouseEvent* /*event*/) {
    it_display->completeData(_database_manager);
    _item = it_display->getItem();
    emit item_doubleCliked(_item);
}

void c_item_lite::mouseReleaseEvent(QMouseEvent *event) {
    mouseDoubleClickEvent(event);
}

void c_item_lite::check_mouse_over() {
    if(underMouse()) {
        timer->start(100);
    } else {
        disconnect(timer,&QTimer::timeout,this,&c_item_lite::check_mouse_over);
        if (!it_display->isHidden()) {
            it_display->hide();
            it_display->update();
            emit item_hide(mapItemToType[_item.getType().getTitle()]);
        }
    }
}

void c_item_lite::setItem(const c_item &item) {
    _item = item;
    if (it_display != nullptr) {
        it_display->deleteLater();
        it_display = nullptr;
    }
    updateView();
}

void c_item_lite::updateView() {
    QFontMetrics metrics(ui->name->font());
    QString elidedText = metrics.elidedText(_item.getName(), Qt::ElideRight, ui->name->width());
    ui->name->setStyleSheet(QString("color : %1").arg(app_color::color_rarity.at(_item.getRarity())));
    ui->name->setText(elidedText);
    ui->lvl->setText(QString("LvL.%1").arg(_item.getLvl()));
    QPixmap image = QPixmap(QString(imageDir + "/images/items/%1.png").arg(_item.getGfxId()));
    ui->image->setPixmap(image);
    ui->image->setBackgroundRole(QPalette::Base);
    ui->image->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->image->setScaledContents(true);
    set_icon();
}
