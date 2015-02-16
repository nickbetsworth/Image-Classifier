#pragma once
#include <vector>

template <class T> class TreeItem
{
public:
	TreeItem();
	~TreeItem();
	int get_num_items() { return m_items.size(); }
	// Need some map or something
	bool add_item(T)
private:
	vector<T> m_items;
};

