#ifndef GPGKEYLISTMODEL_H
#define GPGKEYLISTMODEL_H

#include <QAbstractItemModel>

#include <gpgme++/context.h>

class GPGKeyListModel : public QAbstractItemModel
{
public:
    GPGKeyListModel(GpgME::Context *ctx, QObject *parent=nullptr);

    bool hasChildren(const QModelIndex &parent) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    int columnCount(const QModelIndex &parent) const override;
    int rowCount(const QModelIndex &parent) const override;

    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;

private:
    GpgME::Context *ctx;

};

#endif // GPGKEYLISTMODEL_H
