#ifndef C_IO_MANAGER_H
#define C_IO_MANAGER_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include "c_item.h"
#include "c_build.h"
#include "c_dbmanager.h"
#include "ui/c_enchantement_item.h"
#include "ui/c_load_builder_dialog.h"

class c_builder_view;

class c_io_manager : public QObject
{
    Q_OBJECT
public:
    enum jsonformat {file,database};

    explicit c_io_manager(c_dbmanager *_db_manager, QObject *parent = nullptr);

signals:

public slots:
    QJsonObject itemToJson(const c_item &item_view);
    QJsonObject builderToJson(const c_builder_view *builder);
    QJsonObject aptitudeToJson(const QMap<QString,int> bonuses);
    QJsonObject enchantToJson(const QMap<QString, c_enchantement_item *> item_sets);
    QByteArray save(c_builder_view *builder, const c_io_manager::jsonformat format = c_io_manager::jsonformat::database, const QString path = QString());
    void jsonToBuilder(c_builder_view *builder, const QJsonObject &json);
    QMap<QString, int> jsonToAptitudeMap(const QJsonObject &json);
    QMap<QString, QList<c_enchantement_chasse> > jsonToEnchantMap(const QJsonObject &json);
    bool load(c_builder_view *builder, const c_io_manager::jsonformat format, QString path = QString(), int id = 0);
    bool loadFrom(c_builder_view *builder, const c_io_manager::jsonformat format, QString path_json, int id);
    void update(c_builder_view *builder, const c_io_manager::jsonformat format = c_io_manager::jsonformat::database, const QString path = QString());


private:
    c_dbmanager *db_manager;
    c_load_builder_dialog *load_builder_dialog;
};

#endif // C_IO_MANAGER_H
