#include "CommonFactory.h"


template <typename Type, typename ProductBase, typename FactoryChild>
CommonFactory<Type, ProductBase, FactoryChild>::CommonFactory()
{
	m_creatorMap.clear();
}

template <typename Type, typename ProductBase, typename FactoryChild>
CommonFactory<Type, ProductBase, FactoryChild>::~CommonFactory()
{
	m_creatorMap.clear();
}

template <typename Type, typename ProductBase, typename FactoryChild>
ProductBase * CommonFactory<Type, ProductBase, FactoryChild>::CreateProduct(Type type)
{
	CreatorMap::iterator it = m_creatorMap.find(type);
	if (it == m_creatorMap.end())
	{
		return NULL;
	}
	return (it->second)();
}

template<typename Type, typename ProductBase, typename FactoryChild>
void CommonFactory<Type, ProductBase, FactoryChild>::ReleaseProduct()
{
	m_creatorMap.clear();
}
