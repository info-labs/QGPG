#include "gpgkeylistmodel.h"

#include <gpgme++/key.h>
#include <gpgme++/keylistresult.h>

class KeyItem : public std::enable_shared_from_this<KeyItem>
{
public:
    std::weak_ptr<KeyItem> parent;
    std::vector<std::shared_ptr<KeyItem>> childs;

    GpgME::Key key;

    KeyItem(const std::shared_ptr<KeyItem> &parent, const GpgME::Key &key=GpgME::Key()) :
        parent(parent),
        childs(),
        key(key)
    {}

    int row() const
    {
        if ( std::shared_ptr<KeyItem> parent = this->parent.lock() ) {
            return std::distance(std::begin(parent->childs), std::find(std::begin(parent->childs), std::end(parent->childs), this->shared_from_this()));
        } else {
            return 0;
        }
    }

    std::shared_ptr<KeyItem> create(const GpgME::Key &key)
    {
        auto result = std::make_shared<KeyItem>(this->shared_from_this(), key);
        this->childs.push_back(result);
        return result;
    }

    static std::shared_ptr<KeyItem> create(const std::shared_ptr<GpgME::Context> &ctx)
    {
        std::shared_ptr<KeyItem> root = std::make_shared<KeyItem>(nullptr);

        ctx->startKeyListing();
        for (int i = 0; ; ++i) {
            GpgME::Error error;
            GpgME::Key key = ctx->nextKey(error);
            if ( key.isNull() ) {
                break;
            }
            root->create(key);
        }
        ctx->endKeyListing();

        return root;
    }

};


GPGKeyListModel::GPGKeyListModel(const std::shared_ptr<GpgME::Context> &ctx, QObject *parent) :
    QAbstractItemModel(parent),
    ctx(ctx)
{
    this->root = KeyItem::create(ctx);
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
        return QVariant(tr("ID"));
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

    KeyItem *indexItem = static_cast<KeyItem *>(index.internalPointer());
    if ( ! indexItem ) {
        return QVariant();
    }

    GpgME::Key key = indexItem->key;
    auto uids = key.userIDs();
    if ( uids.size() < 1 ) {
        return QVariant();
    }

    if ( index.column() == 0 ) {
        // ID
        const char *id = uids[0].id();
        return id ? QVariant(id) : QVariant();
    }

    return QVariant();
}

int GPGKeyListModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

int GPGKeyListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    if ( ! this->ctx ) {
        return 0;
    }

    return this->root->childs.size();
}

QModelIndex GPGKeyListModel::index(int row, int column, const QModelIndex &parent) const
{
    if ( ! this->hasIndex(row, column, parent) ) {
        return QModelIndex();
    }

    KeyItem *parentItem = nullptr;
    if ( ! parent.isValid() ) {
        parentItem = this->root.get();
    } else {
        parentItem = static_cast<KeyItem *>(parent.internalPointer());
    }

    std::shared_ptr<KeyItem> child = parentItem->childs.at(row);
    if ( child ) {
        return this->createIndex(row, column, child.get());
    }
    return QModelIndex();
}

QModelIndex GPGKeyListModel::parent(const QModelIndex &child) const
{
    if ( ! child.isValid() ) {
        return QModelIndex();
    }

    KeyItem *childItem = static_cast<KeyItem *>(child.internalPointer());
    if ( std::shared_ptr<KeyItem> parent = childItem->parent.lock() ) {
        this->createIndex(parent->row(), 0, parent.get());
    }
    return QModelIndex();
}
