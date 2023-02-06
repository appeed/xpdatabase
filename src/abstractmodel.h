#ifndef ABSTRACTMODEL_H
#define ABSTRACTMODEL_H

#include <QAbstractListModel>
#include <QDataStream>
#include <QList>
#include <QMultiHash>

template <class T>
class AbstractModel : public QAbstractListModel
{
public:
	int count() const;
	int rowCount(const QModelIndex& = QModelIndex()) const Q_DECL_OVERRIDE;
	QVariant data(const QModelIndex&, int = Qt::DisplayRole) const Q_DECL_OVERRIDE;

public:
	void add(const QString&, T*);
	T *value(const QString&) const;
	T *at(const int) const;

	template <typename U>
	friend QDataStream& operator<<(QDataStream&, const AbstractModel<U>&);

private:
	QMultiHash<QString, int> hash;
	QList<T*> list;
};

template <class T>
int AbstractModel<T>::count() const
{
	return list.size();
}

template <class T>
int AbstractModel<T>::rowCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent);
	return count();
}

template <class T>
QVariant AbstractModel<T>::data(const QModelIndex& index, int role) const
{
	// Get index
	int row = index.row();

	// Fetch item
	if((row < 0) || (row >= rowCount())) return QVariant();
	T* t = list[row];
	if(!t) return QVariant();

	// Return role
	return role;
}

template <class T>
void AbstractModel<T>::add(const QString& key, T* item)
{
	hash.insert(key, list.count());
	list.append(item);
}

template <class T>
T *AbstractModel<T>::value(const QString& key) const
{
	int i = hash.value(key, -1);
	if(i==-1 || i>=list.count()) return NULL;
	return list.at(i);
}

template <class T>
T *AbstractModel<T>::at(const int i) const
{
	if(i>=list.count()) return NULL;
	return list.at(i);
}

template <class T>
QDataStream& operator<<(QDataStream& out, const AbstractModel<T>& v)
{
	out << v.list;
	return out;
}

#endif // ABSTRACTMODEL_H
