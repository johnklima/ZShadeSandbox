#ifndef __ZSHADE_MAP_H_
#define __ZSHADE_MAP_H_

#include <vector>
using namespace std;

//namespace zmap
//{

//Forward declaration
//template <class _Key, class _Value> class zmap_iterator;


//This is a handly homeaid map class
// that is an alternative to the full blown
// stl map class
template <class _Key, class _Value>
class ZShadeMap
{
	mutable vector<_Key> key_;
	mutable vector<_Value> value_;
	
	_Value initial_value;
	
	//Iterator values to the first and second in map
	mutable _Key   *first_;
	mutable _Value *second_;
	
	mutable ZShadeMap *next_;
	mutable ZShadeMap *prev_;
	
public:
	//Default constructor
	ZShadeMap() : first_(NULL), second_(NULL), next_(NULL), prev_(NULL) {}
	
	void copy( const ZShadeMap & c )
	{
		key_ = c.key_;
		value_ = c.value_;
		initial_value = c.initial_value;
	}
	
	//Copy constructor
	ZShadeMap(const ZShadeMap & c) : first_(NULL), second_(NULL), next_(NULL), prev_(NULL)
	{
		copy( c );
	}
	
	//Paired constructor
	ZShadeMap(const _Key & pKey, const _Value & pValue) : first_(NULL), second_(NULL), next_(NULL), prev_(NULL)
	{
		_key.push_back(pKey);
		_value.push_back(pValue);
	}
	
	//Destructor
	virtual ~ZShadeMap()
	{
		
	}
	
	void SetInitialValue(_Value pValue) { initial_value = pValue; }
	
	//Assignment operator
	ZShadeMap & operator = ( const ZShadeMap & c )
	{
		//this->clear();
		
		copy( c );
		
		return(*this);
	}
	
	int size()
	{
		//This is a good representation of the size
		return(key_.size());
	}
	
	_Value & find(int p_pIndex)
	{
		return(value_[p_pIndex]);
	}
	
	_Value * get_ptr(_Key p_pIndex)
	{
		int found_index = -1;
		
		for( int i = 0; i < key_.size(); i++ )
		{
			if( key_[i] == p_pIndex )
			{
				//key value is found
				found_index = i;
				break;
			}
		}
		
		if( found_index == -1 )
			return(NULL);
		else
			return(&value_[found_index]);
	}
	
	_Value get(_Key p_pIndex)
	{
		int found_index = -1;
		
		for( int i = 0; i < key_.size(); i++ )
		{
			if( key_[i] == p_pIndex )
			{
				//key value is found
				found_index = i;
				break;
			}
		}
		
		if( found_index == -1 )
			return(initial_value);
		return(value_[found_index]);
	}
	
	_Value & operator [] (_Key p_pIndex)
	{
		int found_index = -1;
		
		for( int i = 0; i < key_.size(); i++ )
		{
			if( key_[i] == p_pIndex )
			{
				//key value is found
				found_index = i;
				break;
			}
		}
		
		if( found_index == -1 )
			return(initial_value);
		else
			return(value_[found_index]);
	}
	
	void insert(_Key pKey, _Value pValue)
	{
		key_.push_back(pKey);
		value_.push_back(pValue);
	}
	
	void clear()
	{
		key_.clear();
		value_.clear();
	}
	
	//---------------------------------------------------------- iterator --------------------------------------------------------
	
	/*typedef zmap_iterator<_Key, _Value> ziterator;
	typedef const zmap_iterator<_Key, _Value> const_iterator;
	
protected:
	
	//Points to the beginning of the list and set the current
	ziterator begin(const ZShadeMap& in) const { return(ziterator(*(in.next_))); }
	
	ziterator first(const ZShadeMap& pKey) const { return(ziterator(*(pKey.first_))); }
	ziterator second(const ZShadeMap& pValue) const { return(ziterator(*(pValue.second_))); }
	
public:
	
	//---------------------------------------- insert methods --------------------------------------------------
	
	ZShadeMap& i_insert(ZShadeMap * inMap)
	{
		if( inMap->next_ == NULL )
		{
			inMap->next_ = inMap;
			
		}
		
		return(*inMap);
	}
	
	
	
	//---------------------------------------- insert methods --------------------------------------------------
	
	//Operator ==
	const bool operator == (const ZShadeMap & inMap) const
	{
		if( (this->key == inMap.key) && (this->value == inMap.value) )
			return(true);
		else
			return(false);
	}
	
	ziterator zmap_iterator() const { return(ziterator(*(this))); }
	ziterator zmap_iterator() { return(ziterator(*(this))); }
	
	
	//Returns the first element
	ziterator begin() const { return(ziterator(*(this->next_))); }
	ziterator begin() { return(ziterator(*(this->next_))); }
	
	//Returns end_of_iterator
	const ziterator& end() const { return(ZShadeMap::ziterator::end_iterator()); }
	
	ziterator first() const { return(ziterator(*(this->first_))); }
	ziterator second() const { return(ziterator(*(this->second_))); }
	
	/*
	Much like destructor with the exception that empty can be called from
	an iterator, calling delete on an iterator will only delete the iterator
	calling empty from an iterator will delete the map it's iterating.
	*
	void clear()
	{
		if( this->_size > 0 )
		{
			
		}
	}
	
	_Key& operator*(int ind) { return(this->_key[ind]); }
	const _Key& operator*(int ind) const { return(this->_key[ind]); }
	_Key& key(int ind) { return(this->_key[ind]); }
	const _Key& key(int ind) const { return(this->_key[ind]); }
	
	/*_Value& operator*(int ind) { return(this->_value[ind]); }
	const _Value& operator*(int ind) const { return(this->_value[ind]); }*
	_Value& value(int ind) { return(this->_value[ind]); }
	const _Value& value(int ind) const { return(this->_value[ind]); }
	
	const _Key& key(int ind, const _Key& inKey) const { return(this->_key[ind] = inKey); }
	const _Value& value(int ind, const _Value& inVal) const { return(this->_value[ind] = inVal); }
	
	
	ziterator push_front(const _Key & pKey, const _Value & pValue)
	{
		ZShadeMap *createdMap = new ZShadeMap(pKey, pValue);
		if( createdMap == NULL )
			throw("allocation failed");
		return(ziterator(i_push_front(createdMap)));
	}
	
	ziterator push_back(const _Key & pKey, const _Value & pValue)
	{
		ZShadeMap *createdMap = new ZShadeMap(pKey, pValue);
		if( createdMap == NULL )
			throw("allocation failed");
		return(ziterator(i_push_back(createdMap)));
	}
	
	ziterator insert(const ziterator& i)
	{
		ZShadeMap *createdMap = new ZShadeMap(i.pKey, i.pValue);
		if( createdMap == NULL )
			throw("allocation failed");
		return(ziterator(i_insert(createdMap)));
	}
	
	ziterator insert(const _Key & pKey, const _Value & pValue)
	{
		ZShadeMap *createdMap = new ZShadeMap(pKey, pValue);
		if( createdMap == NULL )
			throw("allocation failed");
		return(ziterator(i_insert(createdMap)));
	}
	
	//Removes first matching value with key
	bool remove(const _Key & pKey)
	{
		
		for( ziterator i = this->begin(); i != this->end(); ++i )
		{
			if( pKey == i.key_ )
				return(erase(i));
		}
		return(false);
	}
	
	bool erase(const ziterator& i)
	{
		if( this->first_ == i.map_ptr() )
		{
			key_.erase(i);
			value_.erase(i);
			return(true);
		}
		return(false);
	}
	
	
	
	ziterator find(const _Key & pKey) const
	{
		return(find(pKey, ziterator(*this->first_)));
	}
	ziterator find(const _Key & pKey, bool (*obj)(const _Key&, const _Key&)) const
	{
		return(find(pKey, ziterator(*this->first_), obj));
	}
	
	ziterator find(const _Key & pKey, const ziterator& iter) const
	{
		if( ZShadeMap *temp = iter.map_ptr() )
		{
			for( int i = 0; i < key_.size(); i++ )
			{
				if( pKey == temp->key_[i] )
				{
					first_  = temp->key_[i];
					second_ = temp->value_[i];
					return(ziterator(*temp));
				}
			}
		}
		return(ZShadeMap::ziterator::end_iterator());
	}
	ziterator find(const _Key & pKey, const ziterator& iter, bool (*obj)(const _Key&, const _Key&)) const
	{
		if( ZShadeMap *temp = iter.map_ptr() )
		{
			for( int i = 0; i < key_.size(); i++ )
			{
				if( obj(pKey, temp->key_[i]) )
				{
					first_  = temp->key_[i];
					second_ = temp->value_[i];
					return(ziterator(*temp));
				}
			}
		}
		return(ZShadeMap::ziterator::end_iterator());
	}*/
};
//================================================================================================================================
//================================================================================================================================
//================================================================================================================================
//================================================================================================================================
/*template <class _Key, class _Value>
class zmap_iterator : private ZShadeMap<_Key, _Value>
{
	typedef ZShadeMap<_Key, _Value> MapType;
	
	mutable MapType *_current;
	
	static zmap_iterator end_of_iterator;
	
	MapType* operator & ();
	const MapType* operator & () const;
	
public:
	
	MapType* map_ptr() const { return(_current); }
	MapType& map_ref() const { return(*_current); }
	
	//Returns the end_of_iterator
	static const typename ZShadeMap<_Key, _Value>::ziterator& end_iterator() { return(end_of_iterator); }
	
	//Default constructor
	zmap_iterator() : _current(NULL) {}
	
	//Copy constructor for iterators
	zmap_iterator(const zmap_iterator & i) : _current(i._current) {}
	
	//Copy constructor for maps
	zmap_iterator(MapType & map_ref) : _current(&map_ref) {}
	
	//Operator = for iterators
	typename ZShadeMap<_Key, _Value>::ziterator& operator = (const zmap_iterator & iter)
	{
		this->_current = iter._current;
		return(*this);
	}
	//Operator = for iterators
	const typename ZShadeMap<_Key, _Value>::ziterator& operator = (const zmap_iterator & iter) const
	{
		this->_current = iter._current;
		return(*this);
	}
	
	typename ZShadeMap<_Key, _Value>::ziterator operator [] (size_t loc) const
	{
		return((*this->_current)[loc]);
	}
	typename ZShadeMap<_Key, _Value>::ziterator operator [] (size_t loc)
	{
		return((*this->_current)[loc]);
	}
	
	//Operator = for maps
	const typename ZShadeMap<_Key, _Value>::ziterator& operator = (const MapType & rhs) const
	{
		this->_current = &(const_cast<MapType&>(rhs));
		return(*this);
	}
	
	//Destructor
	~zmap_iterator() {};
	
	//Operator equals
	bool operator == (const zmap_iterator & rhs) const
	{
		if( this->_current == rhs._current )
			return(true);
		else
			return(false);
	}
	
	//Operator not equals
	bool operator != (const zmap_iterator & rhs) const
	{
		return(!(*this == rhs));
	}
	
	//Operator++, prefix
	const typename ZShadeMap<_Key, _Value>::ziterator& operator ++ () const
	{
		this->_current = ( const_cast<MapType*>(this->MapType::next_( *_current )) );
		return(*this);
	}
	
	//Operator++, postfix
	typename ZShadeMap<_Key, _Value>::ziterator operator ++ (int) const
	{
		typename ZShadeMap<_Key, _Value>::iterator iTemp = *this;
		++(*this);
		return(iTemp);
	}
	
	//Operator--
	const typename ZShadeMap<_Key, _Value>::ziterator& operator -- () const
	{
		this->_current = ( const_cast<MapType*>(this->MapType::prev_( *_current )) );
		return(*this);
	}
	
	//Begin iteration thru the map
	typename ZShadeMap<_Key, _Value>::ziterator begin() const { return(this->MapType::begin( *_current )); }
	typename ZShadeMap<_Key, _Value>::ziterator begin()       { return(this->MapType::begin( *_current )); }
	
	typename ZShadeMap<_Key, _Value>::ziterator first() const { return(this->MapType::first( *_current )); }
	typename ZShadeMap<_Key, _Value>::ziterator second() const { return(this->MapType::second( *_current )); }
	
	//Are we at the end
	const typename ZShadeMap<_Key, _Value>::ziterator end() { return(this->MapType::end()); }
	
	
	typename ZShadeMap<_Key, _Value>::ziterator push_back(const _Key& k, const _Value& v)
	{
		return(this->_current->MapType::push_back(k,v));
	}
	
	typename ZShadeMap<_Key, _Value>::ziterator push_front(const _Key& k, const _Value& v)
	{
		return(this->_current->MapType::push_front(k,v));
	}
	
	//Insert into the iterator's Map
	typename ZShadeMap<_Key, _Value>::ziterator insert(const _Key& k, const _Value& v)
	{
		return(this->_current->MapType::insert(k,v));
	}
	
	//Insert into the iterator's Map (with a function object)
	typename ZShadeMap<_Key, _Value>::ziterator insert(const _Key& k, const _Value& v, bool (*obj)(const _Key&, const _Value&, const _Key&, const _Value&))
	{
		return(this->_current->MapType::insert(k,v,obj));
	}
	
	
};

//Static iterator initialization
template <class _Key, class _Value>
zmap_iterator<_Key, _Value> zmap_iterator<_Key, _Value>::end_of_iterator;
*/
//}

#endif /* __ZSHADE_MAP_H_ */
