#pragma once

#include <map>
#include <stddef.h>
#include "Util.h"

using namespace std;
/*
	通用工厂父类（模板类）
*/


#define REGISTER_PRODUCT(Type, Product, FactoryChild) \
	m_productMap.insert(ProductMap::value_type(Type, FactoryChild::Create##Product));

#define INSTANCE_PRODUCT(ProductBase, Product)\
ProductBase* Create##Product()\
{\
	return new Product();\
}

template <typename Type, typename ProductBase, typename FactoryChild>
class CommonFactory
{
public:
	CommonFactory()
	{
		m_creatorMap.clear();
	}
	virtual ~CommonFactory()
	{
		m_creatorMap.clear();
	}

	virtual ProductBase* CreateProduct(Type type)
	{
		CreatorMapIter it = m_creatorMap.find(type);
		if (it == m_creatorMap.end())
		{
			return NULL;
		}
		return (dynamic_cast<FactoryChild*>(this)->*(it->second))();
	}

	void ReleaseProduct()
	{
		m_creatorMap.clear();
	}

protected:
	typedef ProductBase* (FactoryChild :: *CreateProductFunc)();
	typedef map<Type, CreateProductFunc> CreatorMap;
	typedef typename CreatorMap::const_iterator CreatorMapIter;
	CreatorMap m_creatorMap;
};

