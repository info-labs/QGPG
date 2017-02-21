#include "gpgkeylistmodel.h"

#include <gpgme++/key.h>

GPGKeyListModel::GPGKeyListModel(GpgME::Context *ctx, QObject *parent) :
    QAbstractItemModel(parent),
    ctx(ctx)
{

}

bool GPGKeyListModel::hasChildren(const QModelIndex &parent) const
{
    return ! parent.isValid();
}

QVariant GPGKeyListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if ( role != Qt::DisplayRole ) {
        return QVariant();
    }

    if ( orientation == Qt::Vertical ) {
        return QVariant(section);
    }

    // Horizontal
    if ( section == 0 ) {
        return QVariant(tr("name"));
    } else if ( section == 1 ) {
        return QVariant(tr("email"));
    } else if ( section == 2 ) {
        return QVariant(tr("comment"));
    }

    // other case
    return QVariant(section);
}

QVariant GPGKeyListModel::data(const QModelIndex &index, int role) const
{
    if ( role != Qt::DisplayRole ) {
        return QVariant();
    }

    if ( this->ctx == nullptr ) {
        return QVariant();
    }

    GpgME::Key key;
    this->ctx->startKeyListing();
    for ( int i = 0; ; ++i ) {
        GpgME::Error error;
        key = this->ctx->nextKey(error);
        if ( key.isNull() ) {
            // escape
            return QVariant();
        }

        if ( i == index.row() ) {
            break;
        }
    }

    auto uids = key.userIDs();
    if ( uids.size() < 1 ) {
        return QVariant();
    }

    if ( index.column() == 0 ) {
        // name
        const char *name = uids[0].name();
        return name ? QVariant(name) : QVariant();
    } else if ( index.column() == 1 ) {
        // email
        const char *email = uids[0].email();
        return email ? QVariant(email) : QVariant();
    } else if ( index.column() == 2) {
        // comment
        const char *comment = uids[0].comment();
        return comment ? QVariant(comment) : QVariant();
    }

    return QVariant();
}

int GPGKeyListModel::columnCount(const QModelIndex &parent) const
{
    // key, email, comment
    return 3;
}

int GPGKeyListModel::rowCount(const QModelIndex &parent) const
{
    if ( this->ctx == nullptr ) {
        return 0;
    }

    this->ctx->startKeyListing();
    int i = 0;
    for ( i = 0; ; ++i ) {
        GpgME::Error error;
        GpgME::Key key = this->ctx->nextKey(error);
        if ( key.isNull() ) {
            break;
        }
    }
    return i;
}

QModelIndex GPGKeyListModel::index(int row, int column, const QModelIndex &parent) const
{
    if ( this->hasIndex(row, column, parent) ) {
        return this->createIndex(row, column);
    }
    return QModelIndex();
}

QModelIndex GPGKeyListModel::parent(const QModelIndex &child) const
{
    if ( ! child.isValid() ) {
        return QModelIndex();
    }

    return this->createIndex(0, 0);
}
