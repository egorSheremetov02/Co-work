#ifndef ACTIONLIST_H
#define ACTIONLIST_H

#include <taskaction.h>
#include <QAbstractListModel>
#include <QMainWindow>
#include <QMimeData>
#include <QObject>
#include <QWidget>

class ActionList : public QAbstractListModel {
  private:
  QVector<TaskAction> list;

  public:
  ActionList(QObject *par = nullptr)
      : QAbstractListModel(par){
            // list <<
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
    const TaskAction &data = list[ind.row()];
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
    list[index.row()] = value.value<TaskAction>();
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
      list.insert(position, TaskAction());
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
};

Q_DECLARE_METATYPE(TaskAction);
#endif  // ACTIONLIST_H
