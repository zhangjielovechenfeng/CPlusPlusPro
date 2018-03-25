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
	CommonFactory();
	virtual ~CommonFactory();

	ProductBase* CreateProduct(Type type);

	void ReleaseProduct();

protected:
	typedef ProductBase* (FactoryChild :: *CreateProductFunc)();
	typedef map<Type, CreateProductFunc> CreatorMap;
	CreatorMap m_creatorMap;
};

