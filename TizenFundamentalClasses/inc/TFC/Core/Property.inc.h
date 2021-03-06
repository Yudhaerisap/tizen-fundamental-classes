/*
 * Tizen Fundamental Classes - TFC
 * Copyright (c) 2016-2017 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *    Core/Property.inc.h
 *
 * Include-header for property object core infrastructures, which is an experimental
 * object which binds to accessor and mutator functions.
 *
 * Created on: 	Oct 24, 2016
 * Author: 		Gilang Mentari Hamidy (g.hamidy@samsung.com)
 */

#pragma once

#ifndef TFC_CORE_PROPERTY_INC_H_
#define TFC_CORE_PROPERTY_INC_H_

#ifndef TFC_CORE_H_
#include "TFC/Core.h"
#endif




#include "TFC/Core/Introspect.h"
#include <utility>

template<typename TClass>
class TFC::PropertyClass
{
protected:
	template<typename TValue>
	using Property = TFC::Core::PropertyObject<TClass, TValue>;
};

class TFC::Core::PropertyObjectBase
{
protected:
	void* const instance;
	PropertyObjectBase(void* instance);
	PropertyObjectBase(const PropertyObjectBase&) = delete;
	PropertyObjectBase& operator=(const PropertyObjectBase&) = delete;
};

template<typename TDefining, typename TValue>
struct TFC::Core::PropertyGetterFunction
{
	typedef TValue (TDefining::*Type)() const;
	typedef TValue ReturnType;
};

template<typename TDefining, typename TValue>
struct TFC::Core::PropertyGetterFunction<TDefining, TValue&>
{
	typedef TValue& (TDefining::*Type)() const;
	typedef TValue& ReturnType;
};

template<typename TDefining, typename TValue>
struct TFC::Core::PropertyGetterFunction<TDefining, TValue const&>
{
	typedef TValue const& (TDefining::*Type)() const;
	typedef TValue const& ReturnType;
};

template<typename TDefining, typename TValue>
struct TFC::Core::PropertyGetterFunction<TDefining, TValue*>
{
	typedef TValue* (TDefining::*Type)();
	typedef TValue* ReturnType;
	typedef TValue const* ConstReturnType;
};

template<typename TDefining, typename TValue>
struct TFC::Core::PropertyGetterFunction<TDefining, TValue const*>
{
	typedef TValue const* (TDefining::*Type)() const;
	typedef TValue const* ReturnType;
	typedef TValue const* ConstReturnType;
};

template<typename TDefining, typename TValue>
struct TFC::Core::PropertySetterFunction
{
	typedef void (TDefining::*Type)(TValue const&);
	typedef TValue const& OperatorParam;
};

template<typename TDefining, typename TValue>
struct TFC::Core::PropertySetterFunction<TDefining, TValue&>
{
	typedef void (TDefining::*Type)(TValue*);
	typedef TValue* OperatorParam;
};


template<typename TDefining, typename TValue>
struct TFC::Core::PropertySetterFunction<TDefining, TValue const&>
{
	typedef void (TDefining::*Type)(TValue const&);
	typedef TValue const& OperatorParam;
};

template<typename TDefining, typename TValue>
struct TFC::Core::PropertySetterFunction<TDefining, TValue*>
{
	typedef void (TDefining::*Type)(TValue*);
	typedef TValue* OperatorParam;
};

template<typename TDefining, typename TValue>
struct TFC::Core::PropertySetterFunction<TDefining, TValue const*>
{
	typedef void (TDefining::*Type)(TValue const*);
	typedef TValue const* OperatorParam;
};

template<typename TDefining, typename TValue>
struct TFC::Core::PropertyObject
{
	typedef typename PropertyGetterFunction<TDefining, TValue>::Type 			GetFunc;
	typedef typename PropertyGetterFunction<TDefining, TValue>::ReturnType 		GetFuncReturn;
	typedef typename std::remove_reference<GetFuncReturn>::type					GetFuncReturnBare;
	typedef typename PropertySetterFunction<TDefining, TValue>::Type 			SetFunc;
	typedef typename PropertySetterFunction<TDefining, TValue>::OperatorParam 	SetFuncParam;

	template<GetFunc func>
	class Get : protected TFC::Core::PropertyObjectBase
	{
	private:
		GetFuncReturn GetValue() const { return (reinterpret_cast<TDefining*>(instance)->*func)(); }
	public:
		typedef std::false_type Mutable;
		Get(TDefining* instance) : PropertyObjectBase(instance) { }

		operator GetFuncReturn() const
		{
			return GetValue();
		}

		GetFuncReturnBare* operator->()
		{
			return &((reinterpret_cast<TDefining*>(instance)->*func)());
		}

		GetFuncReturn T() const;

		template<typename TParam>
		auto operator==(TParam param)
			->decltype(GetValue() == param)
		{
			return GetValue() == param;
		}

		template<SetFunc setFunc>
		class Set : public Get<func>
		{
		public:
			typedef std::true_type Mutable;
			typedef TValue Type;
			Set(TDefining* instance) : Get<func>(instance) { }
			void operator=(SetFuncParam val)
			{
				(reinterpret_cast<TDefining*>(this->instance)->*setFunc)(val);
			}
		};


	};

	class Auto
	{
	private:
		TValue data;
	public:
		class ReadOnly;
		class ReadWrite;
	};
};

template<typename TDefining, typename TValue>
struct TFC::Core::PropertyObject<TDefining, TValue*>
{
	typedef typename PropertyGetterFunction<TDefining, TValue*>::Type 				GetFunc;
	typedef typename PropertyGetterFunction<TDefining, TValue*>::ReturnType 		GetFuncReturn;
	typedef typename PropertyGetterFunction<TDefining, TValue*>::ConstReturnType 	GetFuncConstReturn;
	typedef typename PropertySetterFunction<TDefining, TValue*>::Type 				SetFunc;
	typedef typename PropertySetterFunction<TDefining, TValue*>::OperatorParam 		SetFuncParam;

	template<GetFunc func>
	class Get : protected TFC::Core::PropertyObjectBase
	{
	public:
		typedef std::false_type Mutable;
		Get(TDefining* instance) : PropertyObjectBase(instance) { }

		operator GetFuncReturn()
		{
			return (reinterpret_cast<TDefining*>(instance)->*func)();
		}

		operator GetFuncConstReturn() const
		{
			return (reinterpret_cast<TDefining*>(instance)->*func)();
		}

		GetFuncReturn operator->()
		{
			return (reinterpret_cast<TDefining*>(instance)->*func)();
		}

		GetFuncConstReturn operator->() const
		{
			return (reinterpret_cast<TDefining*>(instance)->*func)();
		}

		GetFuncReturn T() const;

		template<SetFunc setFunc>
		class Set : public Get<func>
		{
		public:
			typedef std::true_type Mutable;
			typedef TValue Type;
			Set(TDefining* instance) : Get<func>(instance) { }
			void operator=(SetFuncParam val)
			{
				(reinterpret_cast<TDefining*>(this->instance)->*setFunc)(val);
			}
		};
	};
};

/*
class PropertyClass
{
private:



public:


	template<typename DefiningClass, typename ValueType>
	struct Property<DefiningClass, ValueType&>
	{
		typedef ValueType& (DefiningClass::*GetFunc)() const;
		typedef void (DefiningClass::*SetFunc)(const ValueType&);

		template<GetFunc func>
		class Get : protected PropertyBase
		{
		public:
			typedef std::false_type Mutable;
			typedef ValueType Type;
			Get(DefiningClass* instance) : PropertyBase(instance) { }
			operator ValueType&() const
			{
				return (reinterpret_cast<DefiningClass*>(this->instance)->*func)();
			}
			ValueType* operator->()
			{
				return &((reinterpret_cast<DefiningClass*>(this->instance)->*func)());
			}
		};

		template<GetFunc getFunc, SetFunc func>
		class GetSet : public Get<getFunc>
		{
		public:
			typedef std::true_type Mutable;
			typedef ValueType Type;
			GetSet(DefiningClass* instance) : Get<getFunc>(instance) { }
			void operator=(const ValueType& val)
			{
				(reinterpret_cast<DefiningClass*>(this->instance)->*func)(val);
			}
		};
	};

	template<typename DefiningClass, typename ValueType>
	struct Property<DefiningClass, ValueType*>
	{
		typedef ValueType* (DefiningClass::*GetFunc)() const;
		typedef void (DefiningClass::*SetFunc)(ValueType*);

		template<GetFunc func>
		class Get : protected PropertyBase
		{
		public:
			typedef std::false_type Mutable;
			typedef ValueType Type;
			Get(DefiningClass* instance) : PropertyBase(instance) { }
			operator ValueType*()
			{
				return (reinterpret_cast<DefiningClass*>(this->instance)->*func)();
			}
			ValueType* operator->()
			{
				return (reinterpret_cast<DefiningClass*>(this->instance)->*func)();
			}
		};

		template<GetFunc getFunc, SetFunc func>
		class GetSet : public Get<getFunc>
		{
		public:
			typedef std::true_type Mutable;
			typedef ValueType Type;
			GetSet(DefiningClass* instance) : Get<getFunc>(instance) { }
			void operator=(ValueType* val)
			{
				(reinterpret_cast<DefiningClass*>(this->instance)->*func)(val);
			}
		};

		class Auto
		{
		private:
			ValueType* data;
		public:
			class ReadOnly;
			class ReadWrite;
		};
	};
};

template<typename DefiningClass, typename ValueType>
class PropertyClass::Property<DefiningClass, ValueType>::Auto::ReadOnly : Auto
{
protected:
	void operator=(const ValueType& val)
	{
		this->data = val;
	}

	friend DefiningClass;
public:
	typedef std::false_type Mutable;
	typedef ValueType Type;
	operator ValueType()
	{
		return this->data;
	}
	ValueType* operator->()
	{
		return &this->data;
	}
};

template<typename DefiningClass, typename ValueType>
class PropertyClass::Property<DefiningClass, ValueType>::Auto::ReadWrite : public ReadOnly
{
public:
	typedef std::true_type Mutable;
	typedef ValueType Type;
	void operator=(const ValueType& val)
	{
		ReadOnly::operator =(val);
	}
};

template<typename DefiningClass, typename ValueType>
class PropertyClass::Property<DefiningClass, ValueType*>::Auto::ReadOnly : Auto
{
protected:
	void operator=(ValueType* val)
	{
		this->data = val;
	}

	friend DefiningClass;
public:
	typedef std::false_type Mutable;
	typedef ValueType Type;
	operator ValueType*()
	{
		return this->data;
	}
	ValueType* operator->()
	{
		return this->data;
	}
};

template<typename DefiningClass, typename ValueType>
class PropertyClass::Property<DefiningClass, ValueType*>::Auto::ReadWrite : public ReadOnly
{
public:
	typedef std::true_type Mutable;
	typedef ValueType Type;
	void operator=(ValueType* val)
	{
		ReadOnly::operator =(val);
	}

};

*/

#endif /* PROPERTY_INC_H_ */
