#ifndef TASKLIST_H
#define TASKLIST_H

#include <task.h>
#include <QAbstractListModel>
#include <QMainWindow>
#include <QMimeData>
#include <QObject>
#include <QWidget>

class TaskList : public QAbstractListModel {
  Q_OBJECT
  private:
  QVector<MyTask> list;

  public:
  TaskList(QObject *par = nullptr) : QAbstractListModel(par) {
    list << MyTask("прjjjjоект", "28.05.2021")
         << MyTask("проект", "28.05.2021");
  };

  int rowCount(const QModelIndex &parent) const override {
    if (!parent.isValid())
      return list.size();
    return 0;
  }

  QVariant data(const QModelIndex &ind,
                int role = Qt::DisplayRole) const override {
    if (!checkIndex(ind, CheckIndexOption::IndexIsValid))
      return QVariant();
    const MyTask &data = list[ind.row()];
    if (role == Qt::DisplayRole)
      return QVariant::fromValue(data);
    if (role == Qt::EditRole)
      return QVariant::fromValue(data);
    return QVariant();
  }

  bool setData(const QModelIndex &index,
               const QVariant &value,
               int role = Qt::EditRole) override {
    if (role != Qt::EditRole)
      return false;
    list[index.row()] = value.value<MyTask>();
    emit dataChanged(index, index);
    return true;
  }

  Qt::ItemFlags flags(const QModelIndex &index) const override {
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable |
           Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
  }

  bool insertRows(int position, int rows, const QModelIndex &parent) override {
    beginInsertRows(QModelIndex(), position, position + rows - 1);

    for (int row = 0; row < rows; row++) {
      list.insert(position, MyTask());
    }

    endInsertRows();
    return true;
  }

  bool removeRows(int position, int rows, const QModelIndex &parent) override {
    beginRemoveRows(QModelIndex(), position, position + rows - 1);

    for (int row = 0; row < rows; row++) {
      list.removeAt(position);
    }

    endRemoveRows();
    return true;
  }

  Qt::DropActions supportedDropActions() const override {
    return Qt::CopyAction | Qt::MoveAction;
  }

  //осторожно! код из интернетов, стоит разобраться
  QStringList mimeTypes() const override {
    QStringList types;
    types << "application/vnd.text.list";
    return types;
  }

  QMimeData *mimeData(const QModelIndexList &indexes) const override {
    QMimeData *mimeData = new QMimeData();
    QByteArray encodedData;

    QDataStream stream(&encodedData, QIODevice::WriteOnly);

    foreach (const QModelIndex &index, indexes) {
      if (index.isValid()) {
        QString text = data(index, Qt::DisplayRole).toString();
        stream << text;
      }
    }

    mimeData->setData("application/vnd.text.list", encodedData);
    return mimeData;
  }

  bool dropMimeData(const QMimeData *data,
                    Qt::DropAction action,
                    int row,
                    int column,
                    const QModelIndex &parent) {
    // qDebug() << action;
    if (action == Qt::IgnoreAction)
      return true;

    if (!data->hasFormat("application/vnd.text.list"))
      return false;

    if (column > 0)
      return false;
    int beginRow;

    if (row != -1)
      beginRow = row;
    else if (parent.isValid())
      beginRow = parent.row();
    else
      beginRow = rowCount(QModelIndex());
    QByteArray encodedData = data->data("application/vnd.text.list");
    QDataStream stream(&encodedData, QIODevice::ReadOnly);
    QStringList newItems;
    int rows = 0;

    while (!stream.atEnd()) {
      QString text;
      stream >> text;
      newItems << text;
      ++rows;
    }
    insertRows(beginRow, rows, QModelIndex());
    foreach (const QString &text, newItems) {
      QModelIndex idx = index(beginRow, 0, QModelIndex());
      setData(idx, text);
      beginRow++;
    }

    return true;
  }
  //конец кода из интернетов
};

#endif  // TASKLIST_H
